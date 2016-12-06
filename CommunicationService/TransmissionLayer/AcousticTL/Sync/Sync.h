#pragma once

#include <functional>
#include <queue>
#include "../DtmfAnalysis/DtmfAnalysis.h"

class Sync {
public:
    Sync(int samplesPerTone, int sampleRate, DtmfSpec dtmfSpec);
    bool trySync(std::queue<float> &);
    const std::vector<unsigned char> &getSyncNibbles() const;

    void setOnSyncFail(const std::function<void(float)> &onSyncFail);
    void setOnSyncSuccess(const std::function<void(float)> &onSyncSuccess);
    const unsigned char getPaddingNibble() const;

private:
    const DtmfSpec dtmfSpec;
    const int sampleRate;
    const int samplesPerTone;

    std::vector<float> recSyncSamples;

    //match - efficient searching
    const std::vector<unsigned char> matchRegions = {0xF, 0xA, 0x0};
    std::vector<unsigned char> recSyncNibbles;
    const int tonesPerMatchRegion = 2;
    const float reqMatchPercentage = .5;
    bool doesMatch();

    //confim and align - processor heavy alignment
    //load depends linearly on samplerate, confNibs.size() and alignResolution
    const std::vector<unsigned char> confNibs = {0xF, 0xA, 0x5};
    const int alignResolution = 5;
    int confirmAndAlign();

    const unsigned char paddingNibble = 0x0;
    std::vector<unsigned char> syncNibbles;

    std::function<void(float confNibPercentage)> onSyncFail;
    std::function<void(float certainty)> onSyncSuccess;
};
