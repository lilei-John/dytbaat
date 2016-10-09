#pragma once

#include <functional>
#include <vector>
#include "PaLifeHandler.h"

struct PaCallbackData{
    const void *inputBuffer;
    void *outputBuffer;
    unsigned long framesPerBuffer;
    const PaStreamCallbackTimeInfo* timeInfo;
    PaStreamCallbackFlags statusFlags;
};

class PaWrapper {
public:
    PaWrapper();
    constexpr static double SAMPLE_RATE {44100};
    bool open();
    void setOnCallback(PaStreamCallbackResult(*onCB)(PaCallbackData));
private:
    PaLifeHandler paLifeHandler;
    static int paCallback(const void *inputBuffer,
                                      void *outputBuffer,
                                      unsigned long framesPerBuffer,
                                      const PaStreamCallbackTimeInfo* timeInfo,
                                      PaStreamCallbackFlags statusFlags,
                                      void *userData);
    PaStreamCallbackResult callback(PaCallbackData);
    PaStreamCallbackResult (*onCallback)(PaCallbackData) = nullptr;
    PaStream *paStream = nullptr;
};