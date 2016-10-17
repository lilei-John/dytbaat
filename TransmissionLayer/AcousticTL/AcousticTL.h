#pragma once

#include "../TransmissionLayer.h"
#include "PaWrapper/PaWrapper.h"
#include "FrameReceiver/FrameReceiver.h"
#include "Sync/Sync.h"
#include "FreqGeneration/FreqGeneration.h"
#include <queue>

enum ATLState{
    idle,
    transmitting,
    receiving
};

class AcousticTL : public TransmissionLayer{
public:
    AcousticTL();
    AcousticTL(const int sampleRate, const int samplesPerTone, const int samplesPerSearch);

    void sendFrame(std::vector<unsigned char>);
    void callback(PaCallbackData);

    const int sampleRate;
    const int samplesPerTone;
    const int samplesPerSearch;
private:
    ATLState state = ATLState::idle;
    std::queue<float> incomingSamples;
    std::queue<float> outgoingSamples;

    unsigned char getNextNipple(int sampleCount);

    FrameProtocol frameProtocol;
    Sync sync = Sync(frameProtocol, samplesPerTone / samplesPerSearch);
    FrameReceiver frameReceiver;
    FreqGeneration freqGeneration;
    PaWrapper paWrapper;
};