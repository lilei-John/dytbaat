#pragma once

#include "../FrameProtocol/FrameProtocol.h"

class Sync {
public:
    Sync(FrameProtocol, float fpf);
    void receiveNipple(unsigned char);
    bool startSequenceReceived();
    void reset();

    void setOnSyncFailed(const std::function<void(int, const std::vector<unsigned char> &, unsigned char)> &onSyncFailed);

private:
    FrameProtocol frameProtocol;
    const float searchPerTone;
    const int allowedMissedSearchFrames = 1;

    int confSearch = 0;
    int missedSearchFrames = 0;
    int confStartNip = 0;

    unsigned char getStartNipple(int);

    std::function<void(
            int startSequenceFailIndex,
            const std::vector<unsigned char> &startSequence,
            unsigned char receivedByte
    )> onSyncFailed;
};
