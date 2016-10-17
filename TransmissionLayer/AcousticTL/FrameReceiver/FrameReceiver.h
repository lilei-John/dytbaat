#pragma once

#include <queue>
#include <vector>

const unsigned char noHighNipple = 0xFF;

class FrameReceiver {
public:
    FrameReceiver();

    void receiveNipple(unsigned char);
    bool isWholeFrameReceived();
    std::vector<unsigned char> getFrame();

private:
    std::vector<unsigned char> frame;
    unsigned char highNipple = noHighNipple;
    bool shouldEscapeNexByte = false;
    bool wholeFrameReceived = false;

    void receiveByte(unsigned char);
};
