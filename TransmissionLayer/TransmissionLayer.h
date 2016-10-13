#pragma once

#include <array>
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
    unsigned char DTMFValues[4][4] = {
            {0b0000, 0b0001, 0b0010, 0b0011},
            {0b0100, 0b0101, 0b0110, 0b0111},
            {0b1000, 0b1001, 0b1010, 0b1011},
            {0b1100, 0b1101, 0b1110, 0b1111}
    };
    std::vector<int> DTMFRowFreq = {697, 770, 852, 941};
    std::vector<int> DTMFColFreq = {1209, 1336, 1477, 1633};
};