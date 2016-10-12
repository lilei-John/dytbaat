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

std::vector<std::pair<int, float>> DtmfSpec::goertzelFilter(std::vector<float> &samples, std::vector<int> freqs, int sampleRate) {

    int blockSize = (int)samples.size();
    std::vector<std::pair<int, float>> returnAmpFreq;

    for (int i = 0; i < 8; ++i) {
        double k = 0.5 + ((blockSize*freqs[i])/(sampleRate));
        double w = (2 * M_PI / blockSize) * k;
        double cosine = cos(w);
        double sine = sin(w);
        double coeff = 2 * cosine;
        double Q0 = 0;
        double Q1 = 0;
        double Q2 = 0;

        for (int j = 0; j < blockSize; j++) {
            Q0 = coeff * Q1 - Q2 + samples[j];
            Q2 = Q1;
            Q1 = Q0;
        }

        double real = Q1 - Q2 * cosine;
        double imag = Q2 * sine;

        returnAmpFreq[i].first = freqs[i];
        returnAmpFreq[i].second = sqrtf((float)((real*real)+(imag*imag)));
    }

    return returnAmpFreq;
}

const std::vector<int> DtmfSpec::getDTMFFreq() const {
    return DTMFFreq;
}


