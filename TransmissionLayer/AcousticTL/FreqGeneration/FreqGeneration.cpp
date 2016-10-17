//
// Created by Haukur Kristinsson on 13/10/2016.
//

#include "FreqGeneration.h"


void FreqGeneration::freqToSamples(std::vector<float> &samples, std::pair<int, int> rowCol, int sampleRate, int samplesPerTone) {

    for( int i = 0; i < samplesPerTone; i++ ) {
        samples.push_back((float) (sin( (double)i * rowCol.first * 2. * M_PI / sampleRate)
                                   + sin( (double)i * rowCol.second * 2. * M_PI / sampleRate))/2);
    }

}

std::vector<float> FreqGeneration::byteFrameToSamples(std::vector<unsigned char> frame, int sampleRate, int samplesPerTone) {

    std::vector<float> samples;
    frame = byteFrameToNibbleFrame(frame);
    for (unsigned char n : frame) {
        std::pair<int, int> freqs = dtmf.nibbleToFreqs(n);
        freqToSamples(samples,freqs,sampleRate,samplesPerTone);
    }
    return samples;

}

