#include <iostream>
#include "RealAudio.h"

RealAudio::RealAudio(double sampleRate) : paWrapper(sampleRate){
    paWrapper.setOnInReceived([&](std::vector<float> &in){
        if(onInReceive) onInReceive(in);
    });
    paWrapper.setOnOutRequest([&](std::vector<float> &out){
        if(onOutRequest) onOutRequest(out);
    });
};