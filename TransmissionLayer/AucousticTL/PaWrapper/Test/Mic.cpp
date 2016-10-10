/*
 * Example of PaWrapper to measure volume of mic input
 * */

#include "../PaWrapper.h"
#include <iostream>

using namespace std;

double sr = 44100;
double frameLength = .1; //seconds
double t = 0;
double sum = 0;

void callback(PaCallbackData cbd){
    float *in = (float*) cbd.inputBuffer;
    for (int i = 0; i < cbd.framesPerBuffer; i++){
        float val = *in++;
        sum += val >= 0. ? val : -val;
    }
    t += cbd.framesPerBuffer;
    if (t / sr > frameLength){
        cout << sum / frameLength << endl;
        sum = t = 0;
    }
}

int main(){
    PaWrapper paWrapper(sr, &callback);
    cin.get(); //Keeps playing sinusoidal wave, until user aborts.
    return 0;
}