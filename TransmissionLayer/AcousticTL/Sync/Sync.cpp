#include "Sync.h"

using namespace std;

Sync::Sync(FrameProtocol f, float fpf) :
        frameProtocol(f),
        searchFramesPerToneFrame(fpf)
{}

void Sync::receiveNipple(unsigned char nipple) {
    if (nipple == nextExpectedStartSequenceNipple()){
        if(++searchFramesConfirmed >= searchFramesPerToneFrame - allowedMissedSearchFrames){
            startSequenceNipplesConfirmed++;
            searchFramesConfirmed = 0;
            missedHammingSearchFrames = 0;
        }
    }else{
        if (searchFramesConfirmed == 0 && missedHammingSearchFrames < allowedMissedSearchFrames){
            missedHammingSearchFrames++;
        }else{
            reset();
        }
    }
}

bool Sync::startSequenceReceived() {
    return startSequenceNipplesConfirmed == frameProtocol.getStartBytes().size();
}

void Sync::reset() {
    searchFramesConfirmed = 0;
    startSequenceNipplesConfirmed = 0;
    missedHammingSearchFrames = 0;
}

unsigned char Sync::nextExpectedStartSequenceNipple() {
    return frameProtocol.getStartBytes()[startSequenceNipplesConfirmed];
}