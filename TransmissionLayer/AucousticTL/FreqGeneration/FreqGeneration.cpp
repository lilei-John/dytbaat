//
// Created by Haukur Kristinsson on 13/10/2016.
//

#include "FreqGeneration.h"
#include <math.h>


std::vector<float> FreqGeneration::freqToSamples(std::pair<int, int> rowCol, int sampleRate, int samplesPerTone) {

    std::vector<float> samples;
    for( int i = 0; i < samplesPerTone; i++ ) {
        samples.push_back((float) (sin( (double)i * rowCol.first * 2. * M_PI / sampleRate)
                                   + sin( (double)i * rowCol.second * 2. * M_PI / sampleRate))/2);
    }
    return samples;

}

std::vector<float> FreqGeneration::byteFrameToSamples(std::vector<unsigned char> frame, int sampleRate, int samplesPerTone) {

    std::vector<float> samples;
    frame = byteFrameToNibbleFrame(frame);
    for (unsigned char n : frame) {
        if (n == 0b1000) {
            std::cout << "0b1000" << std::endl;
        }
    }
    return samples;
}

