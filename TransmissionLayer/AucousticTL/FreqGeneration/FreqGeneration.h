//
// Created by Haukur Kristinsson on 13/10/2016.
//

#ifndef DYTBAAT_FREQGENERATION_H
#define DYTBAAT_FREQGENERATION_H

#include <utility>
#include <vector>


class FreqGeneration {
public:
    std::vector<float> freqToSamples(std::pair<int, int>, int sampleRate, int samplesPerTone);
    std::vector<float> nibbleFrameToSamples(std::vector<unsigned char> &frame, int sampleRate, int samplesPerTone);

private:
};


#endif //DYTBAAT_FREQGENERATION_H
