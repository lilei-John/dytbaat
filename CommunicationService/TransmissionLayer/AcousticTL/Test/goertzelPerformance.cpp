#include "../helperFunctions.h"
#include <math.h>
#include <chrono>
#include <iostream>

using namespace std;

int main(){
    unsigned int blockSize = 120;
    unsigned int samplerate = 4000;
    float f = 400;
    vector<float> samples(blockSize);
    for (int i = 0; i < blockSize; i++)
        samples[i] = (float) sin(i * f/samplerate * 2 * M_PI);
    unsigned long int count = 0;
    chrono::seconds testDuration(5);
    auto start = chrono::system_clock::now();
    while (true){
        goertzelFilter(&samples[0], blockSize, {1, 2, 3, 4}, samplerate);
        goertzelFilter(&samples[0], blockSize, {1, 2, 3, 4}, samplerate);
        if(chrono::system_clock::now() - start >= testDuration) break;
        count++;
    }
    cout << count/testDuration.count() << " dtmf analysis pr second." <<  endl;
    return 0;
}