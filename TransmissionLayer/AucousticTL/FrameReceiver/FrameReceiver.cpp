#include "FrameReceiver.h"
#include "../Escaping/Escaping.h"

#include <vector>
#include <queue>

using namespace std;

FrameReceiver::FrameReceiver() {}

bool FrameReceiver::isWholeFrameReceived() {
    return wholeFrameReceived;
}

vector<unsigned char> FrameReceiver::getFrame() {
    return frame;
}

void FrameReceiver::receiveNipple(unsigned char nipple) {
    if (highNipple == nullptr) {
        highNipple = nipple;
    }else{
        unsigned char byte = highNipple << 4 | nipple;
        highNipple = nullptr;
        receiveByte(byte);
    }
}

void FrameReceiver::receiveByte(unsigned char byte) {
    if (shouldEscapeNexByte){
        frame.push_back(byte);
        shouldEscapeNexByte = false;
        return;
    }

    if (Escaping::isEscapeByte(byte)){
        shouldEscapeNexByte = true;
        return;
    }

    if (Escaping::isStopByte(byte)){
        wholeFrameReceived = true;
        return;
    }

    frame.push_back(byte);
}