#pragma once

#include "../TransmissionLayer.h"
#include "helperFunctions.h"
#include "DtmfSpec.h"

class AucousticTL : public TransmissionLayer{
public:
    AucousticTL();
    ~AucousticTL();

    int getSamplesPerTone() const;

    int getSampleRate() const;

private:
    int sampleRate = 44100;
    int samplesPerTone = 5000;

};