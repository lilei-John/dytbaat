/*
 * This class wraps Portaudio in an easy to use interface.
 * */

#pragma once

#include "PaLifeHandler.h"
#include "PaCallBackData.h"

class PaWrapper {
public:
    PaWrapper(double sampleRate, void(*onCB)(PaCallbackData));
private:
    double sampleRate;
    PaLifeHandler paLifeHandler;
    PaStream *paStream;
    void (*userCallback)(PaCallbackData);
    static int paCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *objectLink);
    PaStreamCallbackResult callback(PaCallbackData);
    void open();
};