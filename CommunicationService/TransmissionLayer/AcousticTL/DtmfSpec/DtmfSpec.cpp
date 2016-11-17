#include "DtmfSpec.h"

std::pair<int, int> DtmfSpec::nibbleToFreqs(unsigned char byte) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if(DTMFValues[i][j] == byte) {
                return std::pair<int, int>(FreqRow[i],FreqCol[j]);
            }
        }
    }
    return std::pair<int, int>(0,0);
}

unsigned char DtmfSpec::freqsToNibble(std::pair<int, int> freqs) {

    int col = 10, row = 10;
    for (int i = 0; i < 4; ++i) {
        if (freqs.first == FreqRow[i]) {
            row = i;
        }
        if (freqs.second == FreqCol[i]) {
            col = i;
        }
    }

    return DTMFValues[row][col];
}

const std::vector<int> &DtmfSpec::getFreqRow() const {
    return FreqRow;
}

const std::vector<int> &DtmfSpec::getFreqCol() const {
    return FreqCol;
}



