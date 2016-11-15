#pragma once

#include "../DtmfSpec/DtmfSpec.h"
#include <vector>

class DtmfAnalysis {
public:
    DtmfAnalysis(const float *its, int size, DtmfSpec dtmfSpec, int sampleRate);
    unsigned char getNipple();
    float getCertainty();
private:
    std::vector<std::pair<int, float>> rowFreqAmp;
    std::vector<std::pair<int, float>> colFreqAmp;
    unsigned char nipple;
    float certainty;
};

