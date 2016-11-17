#pragma once

#include <thread>
#include "../Media.h"

class IdealAudio : public Media{
public:
    static void connect(IdealAudio &a, IdealAudio &b, unsigned int sampleRate, unsigned int bufferSize);
};