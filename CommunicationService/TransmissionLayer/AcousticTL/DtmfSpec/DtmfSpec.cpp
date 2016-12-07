#include "DtmfSpec.h"

std::pair<int, int> DtmfSpec::nibbleToFreqs(unsigned char nibble) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if(DTMFValues[i][j] == nibble) {
                return std::pair<int, int>(FreqRow[i],FreqCol[j]);
            }
        }
    }
    throw("No such dtmf tone");
}

unsigned char DtmfSpec::freqsToNibble(std::pair<int, int> freqs) const{
    int col = -1, row = -1;
    for (int i = 0; i < 4; ++i) {
        if (freqs.first == FreqRow[i]) {
            row = i;
        }
        if (freqs.second == FreqCol[i]) {
            col = i;
        }
    }
    if (col == -1 || row == -1) throw("No such dtmf tone");
    return DTMFValues[row][col];
}

const std::vector<int> &DtmfSpec::getFreqRow() const {
    return FreqRow;
}

const std::vector<int> &DtmfSpec::getFreqCol() const {
    return FreqCol;
}

unsigned char DtmfSpec::getDTMFNibble(int r, int c) {
    if (r < 0 || 3 < r || c < 0 || 3 < c) throw("No such nibble");
    return DTMFValues[r][c];
}