/*
 * Example of PaWrapper to measure volume of mic input
 * */

#include "../PaWrapper.h"
#include <iostream>
#include <math.h>

using namespace std;

int main(){
    double sr = 44100;
    double frameLength = .5; //seconds
    double t = 0;
    double sum = 0;

    PaWrapper p(sr);
    p.setOnInReceived([](vector<float> &in){
        for (auto val : in){
            sum += fabsf(val);
            if (++t / sr >= frameLength) {
                auto vol = (sum / frameLength);
                cout << vol << endl;
                sum = t = 0;
            }
        }
    });
    cin.get(); //Keeps playing sinusoidal wave, until user aborts.
    return 0;
}