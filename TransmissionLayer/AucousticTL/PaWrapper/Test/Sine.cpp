#include "../PaWrapper.h"
#include <iostream>
#include "math.h"

using namespace std;

double sr = 44100;
double f = 880;
double t = 0;

void callback(PaCallbackData cbd){
    float *out = (float*) cbd.outputBuffer;
    for (double i = 0; i < cbd.framesPerBuffer; i++){
        *out++ = (float) sin(t++ / sr * f * 2 * M_PI);
    }
}

int main(){
    PaWrapper paWrapper(sr, &callback);
    cin.get(); //Keeps playing sinusoidal wave, until user aborts.
    return 0;
}