#pragma once

#include <queue>
#include <vector>
#include "../FrameProtocol/FrameProtocol.h"

const unsigned char noHighNipple = 0xFF;

class FrameReceiver {
public:
    FrameReceiver(FrameProtocol);

    void receiveNipple(unsigned char);
    bool isWholeFrameReceived();
    std::vector<unsigned char> getFrame();

    void reset();

private:
    FrameProtocol frameProtocol;
    std::vector<unsigned char> frame;
    unsigned char highNipple = noHighNipple;
    bool shouldEscapeNexByte = false;
    bool wholeFrameReceived = false;

    void receiveByte(unsigned char);
};
