#pragma once

#include "portaudio.h"

struct PaCallbackData{
    const void *inputBuffer;
    void *outputBuffer;
    unsigned long framesPerBuffer;
    const PaStreamCallbackTimeInfo* timeInfo;
    PaStreamCallbackFlags statusFlags;
};