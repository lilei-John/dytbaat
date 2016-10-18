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
    const float searchPerTone;
    const int allowedMissedSearchFrames = 1;

    int confSearch = 0;
    int missedSearchFrames = 0;
    int confStartNip = 0;

    unsigned char getStartNipple(int);
};
