#include <iostream>
#include "Sync.h"

using namespace std;

Sync::Sync(int samplesPerTone, int sampleRate, DtmfSpec dtmfSpec)
        : samplesPerTone(samplesPerTone),
          sampleRate(sampleRate),
          dtmfSpec(dtmfSpec){
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
        if (matches < tonesPerMatchRegion * matchPercentage)
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
    sort(alignScores.begin(), alignScores.end(), [](AlignScore &a, AlignScore &b) -> bool{
        if (a.confNibCount != b.confNibCount) return a.confNibCount > b.confNibCount;
        return a.certainty > b.certainty;
    });
    if (alignScores[0].confNibCount != confNibs.size()) {
        //cout << "Failed sync! Mathced: " << alignScores[0].confNibCount << " of " << confNibs.size() << endl;
        return -1;
    }
    //cout << "Succeeded sync!" << alignScores[0].certainty << endl;
    return (int)(alignScores[0].startIndex + (confNibs.size() + tonesPerMatchRegion) * samplesPerTone);
}

const vector<unsigned char> &Sync::getSyncNibbles() const {
    return syncNibbles;
}