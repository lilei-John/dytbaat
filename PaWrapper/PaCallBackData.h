#pragma once

#include "portaudio.h"

struct PaCallbackData{
    const float *inputBuffer;
    float *outputBuffer;
    unsigned long framesPerBuffer;
    const PaStreamCallbackTimeInfo* timeInfo;
    PaStreamCallbackFlags statusFlags;
};