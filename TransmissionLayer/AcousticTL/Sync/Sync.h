#pragma once

#include "../FrameProtocol/FrameProtocol.h"

class Sync {
public:
    Sync(FrameProtocol, float fpf);
    void receiveNipple(unsigned char);
    bool startSequenceReceived();
    void reset();
private:
    FrameProtocol frameProtocol;
    const float searchFramesPerToneFrame;
    const int allowedMissedSearchFrames = 1;

    int searchFramesConfirmed = 0;
    int missedHammingSearchFrames = 0;
    int startSequenceNipplesConfirmed = 0;

    unsigned char nextExpectedStartSequenceNipple();
};
