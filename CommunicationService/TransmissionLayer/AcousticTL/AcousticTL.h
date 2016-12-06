#pragma once

#include "../TransmissionLayer.h"
#include "FrameReceiver/FrameReceiver.h"
#include "Sync/Sync.h"
#include "FreqGeneration/FreqGeneration.h"
#include "DtmfAnalysis/DtmfAnalysis.h"
#include <queue>

enum class ATLState{
    idle,
    transmitting,
    receiving
};

class AcousticTL : public TransmissionLayer{
public:
    AcousticTL(const int sampleRate, const int samplesPerTone);

    bool sendFrame(const std::vector<unsigned char> &);
    void processInput(const std::vector<float> &);
    void setOutput(std::vector<float> &);
    void setMaxFrameSize(int size);

    Sync &getSync();
    FrameReceiver &getFrameReceiver();
    const FrameProtocol &getFrameProtocol() const;

    const int sampleRate;
    const int samplesPerTone;

    void setOnStartSeqReceived(const std::function<void()> &onStartSeqReceived);

private:
    ATLState state = ATLState::idle;
    std::queue<float> incomingSamples;
    std::queue<float> outgoingSamples;

    DtmfSpec dtmfSpec;
    Sync sync = Sync(samplesPerTone, sampleRate, dtmfSpec);
    FreqGeneration freqGeneration = FreqGeneration(dtmfSpec);

    FrameProtocol frameProtocol;
    FrameReceiver frameReceiver;

    std::function<void(void)> onStartSeqReceived;
};