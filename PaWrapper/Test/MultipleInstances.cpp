#include "../PaWrapper.h"
#include <iostream>
#include "math.h"

using namespace std;

class Sine{
public:
    Sine(double sr, float f) : sr(sr), f(f){};
    double sr;
    double f;
    double a = 0.2;
    double t = 0;

    float next() {
        return (float) (a * sin(t++ / sr * f * 2. * M_PI));
    }
};

int main(){
    double sampleRate = 44100;

    Sine sine1(sampleRate, 440);
    PaWrapper p1(sine1.sr);
    p1.setOnOutRequest([&sine1](vector<float> &out){
        for (auto &val : out) val = sine1.next();
    });

    Sine sine2(sampleRate, 880);
    PaWrapper p2(sine2.sr);
    p2.setOnOutRequest([&sine2](vector<float> &out){
        for (auto &val : out) val = sine2.next();
    });

    cin.get(); //Keeps playing sinusoidal wave, until user aborts.
    return 0;
}