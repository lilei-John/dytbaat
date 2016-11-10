#include "../RealAudio.h"
#include <math.h>
#include <vector>
#include <iostream>

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
    RealAudio ra(sine.sr);
    ra.setOnOutRequest([&](vector<float> &out){
        for (auto &val : out) val = sine.next();
    });
    cin.get();
}
