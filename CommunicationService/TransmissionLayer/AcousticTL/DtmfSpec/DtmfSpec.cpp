//
// Created by Haukur Kristinsson on 12/10/2016.
//

#include "DtmfSpec.h"

DtmfSpec::DtmfSpec() {

}

std::pair<int, int> DtmfSpec::nibbleToFreqs(unsigned char byte) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if(DTMFValues[i][j] == byte) {
                return std::pair<int, int>(DTMFRowFreq[i],DTMFColFreq[j]);
            }
        }
    }
    return std::pair<int, int>(0,0);
}

unsigned char DtmfSpec::freqsToNibble(std::pair<int, int> freqs) {

    int col = 10, row = 10;
    for (int i = 0; i < 4; ++i) {
        if (freqs.first == DTMFRowFreq[i]) {
            row = i;
        }
        if (freqs.second == DTMFColFreq[i]) {
            col = i;
        }
    }

    return DTMFValues[row][col];
}

const std::vector<int> DtmfSpec::getFreqRow() {
    return DTMFRowFreq;
}

const std::vector<int> DtmfSpec::getFreqCol() {
    return DTMFColFreq;
}

const std::vector<int> DtmfSpec::getDTMFFreq(){
    return DTMFFreq;
}


