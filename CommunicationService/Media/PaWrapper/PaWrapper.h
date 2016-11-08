#pragma once

#include "PaLifeHandler.h"
#include "PaCallBackData.h"
#include <functional>

class PaWrapper {
public:
    PaWrapper(double sampleRate, std::function<void(PaCallbackData)> callback);
private:
    double sampleRate;
    PaLifeHandler paLifeHandler;
    PaStream *paStream;
    std::function<void(PaCallbackData)> userCallback;
    static int paCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *objectLink);
    PaStreamCallbackResult callback(PaCallbackData);
    void open();
};