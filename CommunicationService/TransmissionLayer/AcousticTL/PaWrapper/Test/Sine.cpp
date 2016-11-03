#include "../PaWrapper.h"
#include <iostream>
#include "math.h"

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
    Sine sine;
    PaWrapper p(sine.sr, bind(&Sine::callback, sine, _1));

    cin.get(); //Keeps playing sinusoidal wave, until user aborts.
    return 0;
}