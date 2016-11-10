#pragma once

#include "../Media.h"
#include "../../../PaWrapper/PaWrapper.h"

class RealAudio : public Media {
public:
    RealAudio(double sampleRate);
private:
    PaWrapper paWrapper;
};
