#include <iostream>
#include "../DtmfAnalysis.h"

using namespace std;

int main(){
    DtmfSpec dtmfSpec;
    const int sampleRate = 44100;
    const int sampleCount = 5000;
    const int f1 = dtmfSpec.getFreqCol()[0];
    const int f2 = dtmfSpec.getFreqRow()[0];
    vector<float> samples;
    for (int i = 0; i < sampleCount; i++){
        samples.push_back(
                (float)(sin((double)i / sampleRate * f1 * 2 * M_PI) +
                sin((double)i / sampleRate * f2 * 2 * M_PI)) / 2
        );
    }

    DtmfAnalysis dtmfAnalysis(samples, dtmfSpec, sampleRate);

    cout << dtmfAnalysis.getNipple() << endl;
    return 0;
}