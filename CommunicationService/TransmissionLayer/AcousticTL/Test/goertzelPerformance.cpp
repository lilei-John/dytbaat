#include "../DtmfAnalysis/DtmfAnalysis.h"
#include <chrono>
#include <iostream>

using namespace std;

int main(){
    unsigned int ms = 30;
    unsigned int samplerate = 4000;
    unsigned int blockSize = (unsigned int)(samplerate * (double)ms/1000);
    cout << "Samplerate: " << samplerate << endl;
    cout << "toneLength: " << ms << endl;
    double tonesPrSecond = 1000./ms;
    cout << "Tones pr. second: " << tonesPrSecond << endl;
    float f = 400;
    vector<float> samples(blockSize);
    for (int i = 0; i < blockSize; i++)
        samples[i] = (float) sin(i * f/samplerate * 2 * M_PI);
    DtmfSpec dtmfSpec;
    unsigned long int count = 0;
    chrono::seconds testDuration(5);
    auto start = chrono::system_clock::now();
    while (true){
        DtmfAnalysis(&samples[0], blockSize, dtmfSpec, samplerate);
        if(chrono::system_clock::now() - start >= testDuration) break;
        count++;
    }
    auto analysisPerSecond = (double)count/testDuration.count();
    cout << analysisPerSecond << " dtmf analysis pr second." <<  endl;
    cout << "Max align resolution: " << analysisPerSecond / tonesPrSecond << endl;
    return 0;
}