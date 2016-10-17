#include "FreqGeneration.h"

using namespace std;

void FreqGeneration::freqToSamples(vector<float> &samples, pair<int, int> rowCol, int sampleRate, int samplesPerTone) {
    for(double i = 0; i < samplesPerTone; i++ ) {
        samples.push_back(
                (float) (
                        sin(i * rowCol.first * 2. * M_PI / sampleRate) +
                        sin(i * rowCol.second * 2. * M_PI / sampleRate)
                ) / 2
        );
    }
}

vector<float> FreqGeneration::byteFrameToSamples(vector<unsigned char> frame, int sampleRate, int samplesPerTone) {
    vector<float> samples;
    frame = byteFrameToNibbleFrame(frame);
    for (unsigned char n : frame) {
        pair<int, int> freqs = dtmf.nibbleToFreqs(n);
        freqToSamples(samples, freqs, sampleRate, samplesPerTone);
    }
    return samples;
}

