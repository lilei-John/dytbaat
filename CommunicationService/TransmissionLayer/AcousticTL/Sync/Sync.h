#pragma once

#include <functional>
#include <queue>
#include "../DtmfAnalysis/DtmfAnalysis.h"

struct SyncMatchScore{
    int matchedNibbles = 0;
    float certainty = 0;
};

class Sync {
public:
    Sync(int samplesPerTone, int sampleRate, DtmfSpec dtmfSpec);
    bool trySync(std::queue<float> &);
    const std::vector<unsigned char> &getStartNibbles() const;

    void setOnSyncFail(const std::function<void(float)> &onSyncFail);
    void setOnSyncSuccess(const std::function<void(float)> &onSyncSuccess);
    const unsigned char getPaddingNibble() const;

private:
    const DtmfSpec dtmfSpec;
    const int sampleRate;
    const int samplesPerTone;

    const unsigned char paddingNibble = dtmfSpec.getDTMFNibble(0,0);
    const std::vector<unsigned char> syncNibbles = { //TODO: Vælg en sekvens, der ikke ville kunne høres inde i et frame pga. escaping
            dtmfSpec.getDTMFNibble(0,1),
            dtmfSpec.getDTMFNibble(0,3),
            dtmfSpec.getDTMFNibble(0,3),
            dtmfSpec.getDTMFNibble(0,2),
            dtmfSpec.getDTMFNibble(0,1)
    };
    const unsigned int alignResolution = 10;

    std::vector<unsigned char> startNibbles;
    const double idealAnalysisSpacing = samplesPerTone / alignResolution;
    const unsigned int realAnalysisSpacing = (unsigned int)ceil(idealAnalysisSpacing);
    const unsigned int requiredAnalysisSize = (unsigned int) ceil(((double)samplesPerTone * syncNibbles.size())/realAnalysisSpacing);
    const double realAlignResolution = samplesPerTone / realAnalysisSpacing;

    std::vector<float> samples;
    std::vector<DtmfAnalysis> analysis;

    SyncMatchScore match();

    std::function<void(float confNibPercentage)> onSyncFail;
    std::function<void(float certainty)> onSyncSuccess;
};
