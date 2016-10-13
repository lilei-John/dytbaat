//
// Created by Haukur Kristinsson on 12/10/2016.
//

#ifndef DYTBAAT_DTMFSPEC_H
#define DYTBAAT_DTMFSPEC_H

#include <utility>
#include <vector>
#include <math.h>


class DtmfSpec {
public:
    DtmfSpec();
    std::pair<int, int> nibbleToFreqs(unsigned char);
    unsigned char freqsToNibble(std::pair<int, int>);
    const std::vector<int> getFreqRow();
    const std::vector<int> getFreqCol();
    const std::vector<int> getDTMFFreq();

private:
    unsigned char DTMFValues[4][4] = {
            {0b0000, 0b0001, 0b0010, 0b0011},
            {0b0100, 0b0101, 0b0110, 0b0111},
            {0b1000, 0b1001, 0b1010, 0b1011},
            {0b1100, 0b1101, 0b1110, 0b1111}
    };
    std::vector<int> DTMFRowFreq = {697, 770, 852, 941};
    std::vector<int> DTMFColFreq = {1209, 1336, 1477, 1633};
    std::vector<int> DTMFFreq = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
};


#endif //DYTBAAT_DTMFSPEC_H
