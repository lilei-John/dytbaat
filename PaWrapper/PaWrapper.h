#pragma once

#include "PaLifeHandler.h"
#include "PaCallBackData.h"
#include <functional>
#include <vector>

class PaWrapper {
public:
    PaWrapper(double sampleRate);
    void setOnInReceived(const std::function<void(std::vector<float> &)> &onInReceived);
    void setOnOutRequest(const std::function<void(std::vector<float> &)> &onOutRequest);

private:
    double sampleRate;
    PaLifeHandler paLifeHandler;
    PaStream *paStream;
    std::function<void(std::vector<float> &)> onInReceived;
    std::function<void(std::vector<float> &)> onOutRequest;
    static int paCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *objectLink);
    PaStreamCallbackResult callback(PaCallbackData);
    void open();
};