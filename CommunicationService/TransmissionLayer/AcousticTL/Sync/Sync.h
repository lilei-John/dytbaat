#pragma once

#include <functional>
#include <queue>
#include "../DtmfAnalysis/DtmfAnalysis.h"

class Sync {
public:
    Sync(int samplesPerTone, int sampleRate, DtmfSpec dtmfSpec);
    bool trySync(std::queue<float> &);
    const std::vector<unsigned char> &getSyncNibbles() const;

private:
    const DtmfSpec dtmfSpec;
    const int sampleRate;
    const int samplesPerTone;

    std::vector<float> recSyncSamples;

    const std::vector<unsigned char> matchRegions = {0xF, 0xA, 0x5, 0x0};
    std::vector<unsigned char> recSyncNibbles;
    const int tonesPerMatchRegion = 3;
    const float matchPercentage = .33;
    bool doesMatch();

    const std::vector<unsigned char> confNibs = {0xF, 0xA, 0x5, 0x0, 0x6, 0x9};
    const int alignResolution = 5;
    const unsigned char paddingNibble = 0x0;
    int confirmAndAlign();

    std::vector<unsigned char> syncNibbles;
};
