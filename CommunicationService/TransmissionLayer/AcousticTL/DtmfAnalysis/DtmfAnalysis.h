#pragma once

#include "../DtmfSpec/DtmfSpec.h"

class DtmfAnalysis {
public:
    DtmfAnalysis(const float *its, int size, const DtmfSpec &dtmfSpec, int sampleRate);
    unsigned char getNibble() const;
    float getCertainty() const;

private:
    unsigned char nibble;
    float certainty;
};

