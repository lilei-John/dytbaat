#pragma once

#include <vector>
#include <array>
#include "AudioLibrary.h"
#include <vector>
#include <queue>

class TransmissionLayer{
public:
    TransmissionLayer();
    //CommService interface

    //Functions
    char samplesToChar(std::vector<float> &samples);
    void charToSamples(char, std::vector<float> &samples);
    std::pair<int,int> charToFreq(char);
    void frameToSamples(std::vector<float> &samples, std::vector<char> &frame);

    // Vector<char> til og fra datalinklag
private:
};