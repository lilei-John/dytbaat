#pragma once

#include <vector>
#include <array>

class TransmissionLayer{
public:
    TransmissionLayer();
    //CommService interface

    //Functions
    char samplesToChar(float *samples);
    void charToSamples(char, float *samples, int);
    std::pair<int,int> charToFreq(char);

    // Vector<char> til og fra datalinklag
private:

};