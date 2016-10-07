//
// Created by Haukur Kristinsson on 06/10/2016.
//

#include "CommunicationService.h"
#include <math.h>

TransmissionLayer::TransmissionLayer() {

}

double TransmissionLayer::calculateFreq(int freq, float *samples) {
    int sampleRate = SAMPLE_RATE;
    int blockSize = FRAMES_PER_BUFFER;
    double k = 0.5 + ((blockSize*freq)/(sampleRate));
    double w = (2 * M_PI / blockSize) * k;
    double cosine = cos(w);
    double sine = sin(w);
    double coeff = 2 * cosine;
    double Q0 = 0;
    double Q1 = 0;
    double Q2 = 0;

    for (int i = 0; i < FRAMES_PER_BUFFER; i++) {
        Q0 = coeff * Q1 - Q2 + samples[i];
        Q2 = Q1;
        Q1 = Q0;
    }

    double real = Q1 - Q2 * cosine;
    double imag = Q2 * sine;
    double magnitude = sqrtf((real*real)+(imag*imag));

    return magnitude;

}