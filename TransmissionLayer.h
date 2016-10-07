#pragma once

#include <vector>

class TransmissionLayer{
public:
    TransmissionLayer();
    //CommService interface

    //Functions
    float calculateFreq(int, float samples[FRAMES_PER_BUFFER]);


    // Vector<char> til og fra datalinklag
private:

};