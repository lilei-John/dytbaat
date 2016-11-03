#include "FrameReceiver.h"

using namespace std;

FrameReceiver::FrameReceiver(FrameProtocol f) :
        frameProtocol(f)
{}

bool FrameReceiver::isWholeFrameReceived() {
    return wholeFrameReceived;
}

vector<unsigned char> FrameReceiver::getFrame() {
    return frame;
}

void FrameReceiver::receiveNipple(unsigned char nipple) {
    if (highNipple == noHighNipple) {
        highNipple = nipple;
    }else{
        unsigned char byte = highNipple << 4 | nipple;
        highNipple = noHighNipple;
        receiveByte(byte);
    }
}

void FrameReceiver::receiveByte(unsigned char byte) {
    if (shouldEscapeNexByte){
        frame.push_back(byte);
        shouldEscapeNexByte = false;
        return;
    }

    if (frameProtocol.isEscapeByte(byte)){
        shouldEscapeNexByte = true;
        return;
    }

    if (frameProtocol.isStopByte(byte)){
        wholeFrameReceived = true;
        return;
    }

    frame.push_back(byte);
}

void FrameReceiver::reset() {

}
