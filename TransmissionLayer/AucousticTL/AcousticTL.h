#pragma once

#include "../TransmissionLayer.h"
#include "PaWrapper/PaWrapper.h"
#include "FrameReceiver/FrameReceiver.h"
#include "Sync/Sync.h"
#include <queue>

const enum ATLState{
    idle,
    transmitting,
    receiving
};

class AcousticTL : public TransmissionLayer{
public:
    AcousticTL();
    static const int SAMPLERATE = 44100;
    static const int SAMPLES_PER_TONE = 5000;
    static const int SAMPLES_PER_SEARCH = 500;

private:
    ATLState state = ATLState::idle;
    std::queue<float> incomingSamples;

    void callback(PaCallbackData);
    unsigned char getNextNipple(int sampleCount);

    PaWrapper paWrapper;
    Sync sync = Sync(SAMPLES_PER_TONE / SAMPLES_PER_SEARCH);
    FrameReceiver frameReceiver;
};