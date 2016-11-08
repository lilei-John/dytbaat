#include "../PaWrapper.h"
#include <iostream>
#include <math.h>

using namespace std;
using namespace placeholders;

class Sine{
public:
    double sr = 44100;
    double f = 440;
    double a = 0.2;
    double t = 0;

    void callback(PaCallbackData cbd){
        float *out = (float*) cbd.outputBuffer;
        for (int i = 0; i < cbd.framesPerBuffer; i++){
            *out++ = (float)(a * sin(t++ / sr * f * 2. * M_PI));
        }
    }
};

int main(){
    Sine sine1;
    PaWrapper paw1(sine1.sr, bind(&Sine::callback, sine1, _1));
    Sine sine2;
    sine2.f = 880;
    PaWrapper paw2(sine2.sr, bind(&Sine::callback, sine2, _1));

    cin.get(); //Keeps playing sinusoidal wave, until user aborts.
    return 0;
}