#include <iostream>
#include <algorithm>
#include "Sync.h"

using namespace std;

Sync::Sync(int samplesPerTone, int sampleRate, DtmfSpec dtmfSpec)
        : samplesPerTone(samplesPerTone),
          sampleRate(sampleRate),
          dtmfSpec(dtmfSpec){
    auto d = [&](int r, int c){
        return dtmfSpec.getDTMFNibble(r, c);
    };
    matchRegions = {d(0,3), d(0,2), d(0,1)};
    confNibs = {d(0,2), d(0,3), d(0,2)};
    paddingNibble = d(0,0);

    for (auto nibble : matchRegions)
        for (int i = 0; i < tonesPerMatchRegion; i++)
            syncNibbles.push_back(nibble);
    for (auto nibble : confNibs)
        syncNibbles.push_back(nibble);
    for (int i = 0; i < tonesPerMatchRegion; i++)
        syncNibbles.push_back(paddingNibble);
}

bool Sync::trySync(std::queue<float> &samples) {
    while(samples.size() >= samplesPerTone * (tonesPerMatchRegion + 1)){
        for (int i = 0; i < samplesPerTone; i++){
            recSyncSamples.push_back(samples.front());
            samples.pop();
        }
        DtmfAnalysis dtmf(&recSyncSamples[recSyncSamples.size() - samplesPerTone], samplesPerTone, dtmfSpec, sampleRate);
        recSyncNibbles.push_back(dtmf.getNipple());
        if (recSyncNibbles.size() < syncNibbles.size()) continue;
        if (recSyncNibbles.size() > syncNibbles.size()){
            recSyncNibbles.erase(recSyncNibbles.begin());
            recSyncSamples.erase(recSyncSamples.begin(), recSyncSamples.begin() + samplesPerTone);
        }
        if(!doesMatch()) continue;
        int firstSampleInMessage = confirmAndAlign();
        if (firstSampleInMessage == -1) continue;
        int paddingToBeRemoved = firstSampleInMessage - (int)recSyncSamples.size();
        for (int i = 0; i < paddingToBeRemoved; i++) samples.pop();
        recSyncSamples.clear();
        recSyncNibbles.clear();
        return true;
    }
    return false;
}

bool Sync::doesMatch(){
    for (int m = 0; m < matchRegions.size(); m++){
        int matches = 0;
        for (int i = 0; i < tonesPerMatchRegion; i++){
            int index = m * tonesPerMatchRegion + i;
            if (recSyncNibbles[index] == syncNibbles[index])
                matches++;
        }
        if (matches < tonesPerMatchRegion * reqMatchPercentage)
            return false;
    }
    return true;
}

float lerp(float a, float b, float t){
    return a + (b - a) * t;
}

struct AlignScore{
    int confNibCount;
    float certainty;
    int startIndex;
    vector<int> received;
};

int Sync::confirmAndAlign(){
    int first = (int) matchRegions.size() * tonesPerMatchRegion * samplesPerTone;
    int last = (int) (matchRegions.size() + 1) * tonesPerMatchRegion * samplesPerTone;
    vector<AlignScore> alignScores;
    for (int alignIndex = 0; alignIndex < alignResolution; alignIndex++){
        int start = (int) roundf(lerp(first, last, (float) alignIndex / (alignResolution - 1)));
        AlignScore score{0, 0, start};
        for (int i = 0; i < confNibs.size(); i++){
            DtmfAnalysis dtmf(&recSyncSamples[start + i * samplesPerTone], samplesPerTone, dtmfSpec, sampleRate);
            if (dtmf.getNipple() == confNibs[i]){
                score.confNibCount++;
                score.certainty += dtmf.getCertainty() / confNibs.size();
            }
            score.received.push_back(dtmf.getNipple());
        }
        alignScores.push_back(score);
    }
    sort(alignScores.begin(), alignScores.end(), [](const AlignScore &a, const AlignScore &b){
        if (a.confNibCount != b.confNibCount) return a.confNibCount > b.confNibCount;
        return a.certainty > b.certainty;
    });
    auto bestAlign = alignScores[0];
    if (bestAlign.confNibCount != confNibs.size()) {
        if(onSyncFail) onSyncFail((float)bestAlign.confNibCount / confNibs.size());
        return -1;
    }
    if(onSyncSuccess) onSyncSuccess(bestAlign.certainty);
    return (int)(bestAlign.startIndex + (confNibs.size() + tonesPerMatchRegion) * samplesPerTone);
}

const vector<unsigned char> &Sync::getSyncNibbles() const {
    return syncNibbles;
}

void Sync::setOnSyncFail(const function<void(float)> &onSyncFail) {
    Sync::onSyncFail = onSyncFail;
}

void Sync::setOnSyncSuccess(const function<void(float)> &onSyncSuccess) {
    Sync::onSyncSuccess = onSyncSuccess;
}
