#pragma once

#include <utility>
#include <vector>
#include <math.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

inline double hammingWindow(int N, int n) {
    return 0.54-0.46*cos((2*M_PI*n)/(N-1));
}

inline std::vector<std::pair<int, float>> goertzelFilter(const float *samples, int N, const std::vector<int> &freqs, const int sampleRate) {
    std::vector<std::pair<int, float>> returnAmpFreq(freqs.size());
    for (int i = 0; i < freqs.size(); ++i) {
        //double k = 0.5 + ((blockSize*freqs[i])/(sampleRate));
        //double w = (2 * M_PI / blockSize) * k;
        double w = 2 * M_PI * freqs[i] / sampleRate;
        double cosine = cos(w);
        double sine = sin(w);
        double coeff = 2 * cosine;
        double Q0 = 0;
        double Q1 = 0;
        double Q2 = 0;

        for (int j = 0; j < N; j++) {
            Q0 = coeff * Q1 - Q2 + (samples[j] * hammingWindow(N,j));
            Q2 = Q1;
            Q1 = Q0;
        }

        double real = Q1 - Q2 * cosine;
        double imag = Q2 * sine;

        returnAmpFreq[i] = std::make_pair(
                freqs[i],
                sqrtf((float)((real*real)+(imag*imag)))
        );
    }
    return returnAmpFreq;
}

inline std::vector<std::pair<int, float>> goertzelFilter(const std::vector<float> &samples, const std::vector<int> &freqs, const int sampleRate){
    return goertzelFilter(&samples[0], (int)samples.size(), freqs, sampleRate);
};

inline std::vector<unsigned char> byteFrameToNibbleFrame(std::vector<unsigned char> byteFrame) {

    std::vector<unsigned char> nibbleFrame;
    for (int i = 0; i < byteFrame.size(); ++i) {
        unsigned char lowNibble = byteFrame[i];
        lowNibble = lowNibble &(unsigned char) 0b00001111;
        unsigned char highNibble = byteFrame[i];
        highNibble = highNibble >> 4;
        highNibble = highNibble &(unsigned char) 0b00001111;
        nibbleFrame.push_back(highNibble);
        nibbleFrame.push_back(lowNibble);
    }
    return nibbleFrame;
}

inline std::vector<unsigned char> nibbleFrameToByteFrame(std::vector<unsigned char> &nibbleFrame) {

    std::vector<unsigned char> byteFrame;
    for (int i = 0; i < nibbleFrame.size(); i+=2) {
        unsigned char highNibble = nibbleFrame[i];
        unsigned char lowNibble = nibbleFrame[i+1];
        highNibble = highNibble << 4;
        byteFrame.push_back(highNibble | lowNibble);
    }
    return byteFrame;
}