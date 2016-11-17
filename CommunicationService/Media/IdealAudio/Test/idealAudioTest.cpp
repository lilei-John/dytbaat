#include "../IdealAudio.h"
#include <iostream>

using namespace std;

int main(){
    IdealAudio a;
    IdealAudio b;

    atomic<int> shouldStop;
    shouldStop.store(false);

    a.setOnOutRequest([](vector<float> &out){
        out[0] = 0;
        out[1] = 1;
    });
    b.setOnInReceived([&](vector<float> &in){
        if (in[0] == 0 && in[1] == 1){
            cout << "gotAudioFromA" << endl;
            shouldStop.store(true);
        }
    });
    IdealAudio::connect(a, b, 44100, 2000);

    while (!shouldStop.load())
        this_thread::yield();
}
