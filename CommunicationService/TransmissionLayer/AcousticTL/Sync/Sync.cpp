#include <iostream>
#include "Sync.h"

using namespace std;

Sync::Sync(FrameProtocol f, float fpf) :
        frameProtocol(f),
        searchPerTone(fpf)
{}

void Sync::receiveNipple(unsigned char nipple) {
    auto wantedNipple = getStartNipple(confStartNip);
    if (nipple == wantedNipple){
        confSearch++;
        if(confSearch >= searchPerTone){
            confStartNip++;
            confSearch = 0;
            missedSearchFrames = 0;
        }
    }else{
        if (confSearch >= searchPerTone - allowedMissedSearchFrames){
            confStartNip++;
            if (startSequenceReceived()) return;
            if (nipple == getStartNipple(confStartNip + 1)){
                confSearch = 1;
            }else{
                confSearch = 0;
            }
            missedSearchFrames = 0;
        }else if (confStartNip > 0 &&
            confSearch == 0 &&
            missedSearchFrames < allowedMissedSearchFrames){
            missedSearchFrames++;
        }else{
            if (onSyncFailed) onSyncFailed(confStartNip, frameProtocol.getStartBytes(), nipple);
            reset();
        }
    }
}

bool Sync::startSequenceReceived() {
    return confStartNip == frameProtocol.getStartBytes().size() * 2;
}

void Sync::reset() {
    confSearch = 0;
    confStartNip = 0;
    missedSearchFrames = 0;
}

unsigned char Sync::getStartNipple(int i) {
    auto byte = frameProtocol.getStartBytes()[i / 2];
    if (i % 2 == 0){
        return byte >> 4;
    }else{
        return byte & (unsigned char)0x0F;
    }
}
// her er eksemplet at arbejde ud fra
void Sync::setOnSyncFailed(const function<void(int, const vector<unsigned char> &, unsigned char)> &cb) {
    onSyncFailed = cb;
}
