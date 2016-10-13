//
// Created by Haukur Kristinsson on 13/10/2016.
//

#include <iostream>
#include <vector>
#include "../FreqGeneration.h"

using namespace std;


int main() {

    FreqGeneration freqGen;
    pair<int, int> freqs(697,1477);
    vector<float> samples = freqGen.freqToSamples(freqs,44100,5000);

    cout << samples.size() << endl;


    return 0;
}