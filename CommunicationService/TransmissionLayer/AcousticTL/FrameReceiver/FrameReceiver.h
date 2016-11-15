#pragma once

#include <queue>
#include <vector>
#include "../FrameProtocol/FrameProtocol.h"
#include "../DtmfAnalysis/DtmfAnalysis.h"

const unsigned char noHighNipple = 0xFF;

class FrameReceiver {
public:
    FrameReceiver(FrameProtocol, DtmfSpec, int samplesPerTone, int sampleRate);

    void processInput(std::queue<float> &);
    void receiveNipple(unsigned char);
    bool isWholeFrameReceived();
    std::vector<unsigned char> getFrame();

    void reset();

private:
    FrameProtocol frameProtocol;
    DtmfSpec dtmfSpec;

    const int samplesPerTone;
    const int sampleRate;

    std::vector<unsigned char> frame;
    unsigned char highNipple = noHighNipple;
    bool shouldEscapeNexByte = false;
    bool wholeFrameReceived = false;

    void receiveByte(unsigned char);
};
