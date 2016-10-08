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
    unsigned char samplesToNibble(std::vector<float> &samples);
    void nibbleToSamples(unsigned char, std::vector<float> &samples);
    std::pair<int,int> charToFreq(unsigned char);
    void frameToSamples(std::vector<float> &samples, std::vector<unsigned char> &frame);
    void byteFrameToNibbleFrame(std::vector<unsigned char> &byteFrame, std::vector<unsigned char> &nibbleFrame);
    void nibbleFrameToByteFrame(std::vector<unsigned char> &nibbleFrame, std::vector<unsigned char> &byteFrame);

    // Vector<char> til og fra datalinklag
private:
    unsigned char ampRowCol[4][4] = {
            {0b00000000,0b00000001, 0b00000010, 0b00000011},
            {0b00000100,0b00000101, 0b00000110, 0b00000111},
            {0b00001000,0b00001001, 0b00001010, 0b00001011},
            {0b00001100,0b00001101, 0b00001110, 0b00001111}
    };
};