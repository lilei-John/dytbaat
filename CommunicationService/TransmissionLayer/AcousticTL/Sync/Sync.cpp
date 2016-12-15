#include <iostream>
#include <algorithm>
#include "Sync.h"

using namespace std;

Sync::Sync(int samplesPerTone, int sampleRate, DtmfSpec dtmfSpec)
        : samplesPerTone(samplesPerTone),
          sampleRate(sampleRate),
          dtmfSpec(dtmfSpec){
    for (auto nibble : syncNibbles)
        startNibbles.push_back(nibble);
    startNibbles.push_back(paddingNibble);
}

bool Sync::trySync(std::queue<float> &q, std::queue<float> &save) {
    while(q.size() >= realAnalysisSpacing * alignResolution){
        for (int i = 0; i < realAnalysisSpacing; i++){
            samples.push_back(q.front());
            save.push(q.front());
            q.pop();
        }
        while(save.size() > (syncNibbles.size() + 2) * samplesPerTone) save.pop();
        if (samples.size() < samplesPerTone) continue;
        analysis.push_back(DtmfAnalysis(&samples[0], samplesPerTone, dtmfSpec, sampleRate));
        samples.erase(samples.begin(), samples.begin() + realAnalysisSpacing);
        if(analysis.size() < requiredAnalysisSize) continue;
        auto m = match();
        analysis.erase(analysis.begin());
        if (m.matchedNibbles == syncNibbles.size()){
            int maxCertaintyIndex = 0;
            float maxCertainty = m.certainty;
            for (int i = 1; i < alignResolution; i++){
                auto score = match();
                analysis.erase(analysis.begin());
                if (score.matchedNibbles == syncNibbles.size() && score.certainty > maxCertainty){
                    maxCertainty = score.certainty;
                    maxCertaintyIndex = i;
                }
            }
            unsigned int paddingAnalysisLeft = alignResolution - (maxCertaintyIndex + 1);
            unsigned int paddingSamplesLeft = paddingAnalysisLeft * realAnalysisSpacing;
            unsigned int paddingSamplesLeftInSampleVector = (unsigned int) (samples.size() - (samplesPerTone - realAnalysisSpacing));
            paddingSamplesLeft -= paddingSamplesLeftInSampleVector;
            for (int i = 0; i < paddingSamplesLeft; i++){
                save.push(q.front());
                save.pop();
                q.pop();
            }
            samples.clear();
            analysis.clear();
            if(onSyncSuccess) onSyncSuccess(maxCertainty);
            return true;
        }
    }
    return false;
}

SyncMatchScore Sync::match() {
    SyncMatchScore s{};
    for (int nibbleIndex = 0; nibbleIndex < syncNibbles.size(); nibbleIndex++){
        int analysisIndex = (nibbleIndex * samplesPerTone)/realAnalysisSpacing;
        auto &a = analysis[analysisIndex];
        if (a.getNibble() == syncNibbles[nibbleIndex]){
            s.matchedNibbles++;
            s.certainty += a.getCertainty();
        }
    }
    s.certainty /= syncNibbles.size();
    return s;
}

const vector<unsigned char> &Sync::getStartNibbles() const {
    return startNibbles;
}

void Sync::setOnSyncFail(const function<void(float)> &onSyncFail) {
    Sync::onSyncFail = onSyncFail;
}

void Sync::setOnSyncSuccess(const function<void(float)> &onSyncSuccess) {
    Sync::onSyncSuccess = onSyncSuccess;
}

const unsigned char Sync::getPaddingNibble() const {
    return paddingNibble;
}