#include "IdealAudio.h"

using namespace std;

void IdealAudio::connect(IdealAudio &a, IdealAudio &b, unsigned int sampleRate, unsigned int bufferSize) {
    thread([&, sampleRate, bufferSize](){
        vector<float> abuf(bufferSize);
        vector<float> bbuf(bufferSize);
        auto sleepTime = (bufferSize * 1000) / sampleRate;
        while (true){
            this_thread::sleep_for(chrono::milliseconds(sleepTime));
            if (a.onOutRequest) a.onOutRequest(abuf);
            if (b.onOutRequest) b.onOutRequest(bbuf);
            if (a.onInReceive) a.onInReceive(bbuf);
            if (b.onInReceive) b.onInReceive(abuf);
        }
    }).detach();
}
