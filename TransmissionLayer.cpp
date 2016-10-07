//
// Created by Haukur Kristinsson on 06/10/2016.
//

#include "CommunicationService.h"

TransmissionLayer::TransmissionLayer() {

}

float TransmissionLayer::calculateFreq(int freq, float samples[FRAMES_PER_BUFFER]) {
    int sampleRate = SAMPLE_RATE;
    int blockSize = FRAMES_PER_BUFFER;
    float k = 0.5 + ((blockSize*freq)/(sampleRate));
    float w = (2 * M_PI / blockSize) * k;
    float cosine = cos(w);
    float sine = sin(w);
    float coeff = 2 * cosine;
    float Q0 = 0;
    float Q1 = 0;
    float Q2 = 0;

    for (int i = 0; i < FRAMES_PER_BUFFER; i++) {
        Q0 = coeff * Q1 - Q2 + samples[i];
        Q2 = Q1;
        Q1 = Q0;
    }

    float real = Q1 - Q2 * cosine;
    float imag = Q2 * sine;
    float magnitude = sqrtf((real*real)+(imag*imag));

    return magnitude;

}