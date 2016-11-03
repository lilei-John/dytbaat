#include "FreqGeneration.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif


using namespace std;

FreqGeneration::FreqGeneration(DtmfSpec dtmf) :
    dtmfSpec(dtmf)
{}

void FreqGeneration::freqToSamples(
        vector<float> &samples,
        const pair<int, int> &freqs,
        pair<double, double> &sineState,
        const int sampleRate,
        const int samplesPerTone) {

    double r = 2. * M_PI / sampleRate;
    for(int i = 0; i < samplesPerTone; i++ ) {
        sineState.first += freqs.first * r;
        sineState.second += freqs.second * r;
        samples.push_back(
                (float) (
                        sin(sineState.first) +
                        sin(sineState.second)
                ) / 2
        );
    }
}

vector<float> FreqGeneration::byteFrameToSamples(vector<unsigned char> frame, int sampleRate, int samplesPerTone) {
    vector<float> samples;
    frame = byteFrameToNibbleFrame(frame);
    pair<double, double> sineState = {0, 0};
    for (unsigned char n : frame) {
        pair<int, int> freqs = dtmfSpec.nibbleToFreqs(n);
        freqToSamples(samples, freqs, sineState, sampleRate, samplesPerTone);
    }
    return samples;
}
