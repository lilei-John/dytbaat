#pragma once

#include "../TransmissionLayer.h"
#include "FrameReceiver/FrameReceiver.h"
#include "Sync/Sync.h"
#include "FreqGeneration/FreqGeneration.h"
#include "DtmfAnalysis/DtmfAnalysis.h"
#include <queue>

enum ATLState{
    idle,
    transmitting,
    receiving
};

class AcousticTL : public TransmissionLayer{
public:
    AcousticTL(const int sampleRate, const int samplesPerTone, const int samplesPerSearch);

    bool sendFrame(const std::vector<unsigned char> &);
    void processInput(const std::vector<float> &);
    void setOutput(std::vector<float> &);

    Sync &getSync();

    const int sampleRate;
    const int samplesPerTone;
    const int samplesPerSearch;

    void setOnStartSeqReceived(const std::function<void()> &onStartSeqReceived);

private:
    ATLState state = ATLState::idle;
    std::queue<float> incomingSamples;
    std::queue<float> outgoingSamples;

    unsigned char getNextNipple(int sampleCount);

    FrameProtocol frameProtocol;
    Sync sync = Sync(frameProtocol, samplesPerTone / samplesPerSearch);

    FrameReceiver frameReceiver = FrameReceiver(frameProtocol);

    DtmfSpec dtmfSpec;
    FreqGeneration freqGeneration = FreqGeneration(dtmfSpec);

    std::function<void(void)> onStartSeqReceived;
};