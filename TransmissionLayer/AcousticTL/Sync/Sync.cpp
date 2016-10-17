#include "Sync.h"
#include "../FrameProtocol/FrameProtocol.h"
#include <vector>

using namespace std;

Sync::Sync(float fpf) : searchFramesPerToneFrame(fpf) {}

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
    return startSequenceNipplesConfirmed == FrameProtocol::startSequence.size();
}

void Sync::reset() {
    searchFramesConfirmed = 0;
    startSequenceNipplesConfirmed = 0;
    missedHammingSearchFrames = 0;
}

unsigned char Sync::nextExpectedStartSequenceNipple() {
    return FrameProtocol::startSequence[startSequenceNipplesConfirmed];
}