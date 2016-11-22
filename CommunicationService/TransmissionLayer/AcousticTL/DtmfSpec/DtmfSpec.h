#pragma once
#include <utility>
#include <vector>
#include <math.h>

class DtmfSpec {
public:
    std::pair<int, int> nibbleToFreqs(unsigned char);
    unsigned char freqsToNibble(std::pair<int, int>);

    const std::vector<int> &getFreqRow() const;
    const std::vector<int> &getFreqCol() const;

private:
    unsigned char DTMFValues[4][4] = {
            {0b0000, 0b0001, 0b0010, 0b0011},
            {0b0100, 0b0101, 0b0110, 0b0111},
            {0b1000, 0b1001, 0b1010, 0b1011},
            {0b1100, 0b1101, 0b1110, 0b1111}
    };
    std::vector<int> FreqRow = {697, 770, 852, 941};
    std::vector<int> FreqCol = {1209, 1336, 1477, 1633};
};
