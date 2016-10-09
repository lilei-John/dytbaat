#include "../PaWrapper.h"
#include <iostream>
#include "math.h"

using namespace std;

double PI = M_PI;
double f = 697;
double amp = .06;
double sr = PaWrapper::SAMPLE_RATE;

double t = 0;

PaStreamCallbackResult callback(PaCallbackData paCBData){
    float *out = (float*) paCBData.outputBuffer;
    for (double i = 0; i < paCBData.framesPerBuffer; i++){
        *out++ = (float)(amp * sin(t++/sr * f * 2 * PI));
    }
    return paContinue;
}

int main(){
    PaWrapper paWrapper;
    paWrapper.setOnCallback(&callback);
    paWrapper.open();
    cin.get(); //Keeps playing sinusoidal wave, until user aborts.
    return 0;
}