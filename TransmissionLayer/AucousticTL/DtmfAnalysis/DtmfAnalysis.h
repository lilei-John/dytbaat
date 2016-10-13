#pragma once

#include "../DtmfSpec/DtmfSpec.h"
#include <vector>

class DtmfAnalysis {
public:
    DtmfAnalysis(const std::vector<float> &samples, DtmfSpec dtmfSpec, int sampleRate);
    unsigned char getNipple();
    float getCertainty();
private:
    std::vector<std::pair<int, float>> rowFreqAmp;
    std::vector<std::pair<int, float>> colFreqAmp;
    unsigned char nipple;
    float certainty;
};

