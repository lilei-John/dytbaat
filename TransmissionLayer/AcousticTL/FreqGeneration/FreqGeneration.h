#pragma once

#include <utility>
#include <vector>
#include "../helperFunctions.h"
#include <iostream>
#include "../DtmfSpec/DtmfSpec.h"


class FreqGeneration {
public:
    FreqGeneration(DtmfSpec);
    void freqToSamples(
            std::vector<float> &samples,
            const std::pair<int, int> &freqs,
            std::pair<double, double> &sineState,
            const int sampleRate,
            const int samplesPerTone);
    std::vector<float> byteFrameToSamples(
            std::vector<unsigned char> frame,
            int sampleRate,
            int samplesPerTone);
private:
    DtmfSpec dtmfSpec;
};

