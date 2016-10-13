//
// Created by Haukur Kristinsson on 13/10/2016.
//

#include <utility>
#include <vector>
#include <math.h>

#ifndef DYTBAAT_HELPERFUNCTIONS_H
#define DYTBAAT_HELPERFUNCTIONS_H

std::vector<std::pair<int, float>> goertzelFilter(std::vector<float> &samples, std::vector<int> freqs, int sampleRate) {

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






#endif //DYTBAAT_HELPERFUNCTIONS_H
