#include "../PaWrapper.h"
#include <iostream>
#include "math.h"

using namespace std;

class Sine{
public:
    double sr = 44100;
    double f = 440;
    double a = 0.2;
    double t = 0;

    float next() {
        return (float) (a * sin(t++ / sr * f * 2. * M_PI));
    }
};

int main(){
    Sine sine;
    PaWrapper p(sine.sr);
    p.setOnOutRequest([&sine](vector<float> &out){
        for (auto &val : out) val = sine.next();
    });

    cin.get(); //Keeps playing sinusoidal wave, until user aborts.
    return 0;
}