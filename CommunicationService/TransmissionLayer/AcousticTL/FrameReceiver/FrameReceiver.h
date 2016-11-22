#pragma once

#include <queue>
#include <vector>
#include <climits>
#include "../FrameProtocol/FrameProtocol.h"
#include "../DtmfAnalysis/DtmfAnalysis.h"

enum class FrameReceiverStatus{
    receiving,
    frameReceived,
    frameError
};

const unsigned char noHighNipple = 0xFF;

class FrameReceiver {
public:
    FrameReceiver(FrameProtocol, DtmfSpec, int samplesPerTone, int sampleRate);

    void processInput(std::queue<float> &);
    void receiveNipple(unsigned char);
    std::vector<unsigned char> getFrame();

    FrameReceiverStatus getFrameReceiverStatus() const;

    void setMaxFrameSize(int maxFrameSize);
    void reset();

private:
    FrameProtocol frameProtocol;
    DtmfSpec dtmfSpec;
    FrameReceiverStatus frameReceiverStatus = FrameReceiverStatus::receiving;

    const int samplesPerTone;
    const int sampleRate;
    int maxFrameSize = INT_MAX;

    std::vector<unsigned char> frame;
    unsigned char highNipple = noHighNipple;
    bool shouldEscapeNexByte = false;

    void receiveByte(unsigned char);
};
