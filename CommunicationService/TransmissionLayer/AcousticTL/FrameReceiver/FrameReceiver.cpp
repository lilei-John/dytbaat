#include <iostream>
#include "FrameReceiver.h"

using namespace std;

FrameReceiver::FrameReceiver(FrameProtocol f, DtmfSpec dtmfSpec, int samplesPerTone, int sampleRate) :
        frameProtocol(f),
        dtmfSpec(dtmfSpec),
        samplesPerTone(samplesPerTone),
        sampleRate(sampleRate)
{}

vector<unsigned char> FrameReceiver::getFrame() {
    return frame;
}

void FrameReceiver::processInput(std::queue<float> &samples) {
    while(samples.size() >= samplesPerTone && frameReceiverStatus == FrameReceiverStatus::receiving){
        vector<float> toneSamples((unsigned long)samplesPerTone);
        for (int i = 0; i < samplesPerTone; i++){
            toneSamples[i] = samples.front();
            samples.pop();
        }
        DtmfAnalysis dtmf(&toneSamples[0], samplesPerTone, dtmfSpec, sampleRate);
        receiveNipple(dtmf.getNipple());
    }
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
    if (frame.size() == maxFrameSize && !frameProtocol.isStopByte(byte)){
        frameReceiverStatus = FrameReceiverStatus::frameError;
        if (onFrameError){
            int rln = byte & 0xF;
            int rhn = byte >> 4;
            auto sb = frameProtocol.getStopByte();
            int sln = sb & 0xF;
            int shn = sb >> 4;
            onFrameError(
                    "FrameError: StopByte expected. Received: " +
                    to_string(rhn) + ":" + to_string(rln) +
                    ", StopByte: " +
                    to_string(shn) + ":" + to_string(sln) + "."
            );
        }
    }else if (shouldEscapeNexByte){
        frame.push_back(byte);
        shouldEscapeNexByte = false;
    }else if (frameProtocol.isEscapeByte(byte)){
        shouldEscapeNexByte = true;
    }else if (frameProtocol.isStopByte(byte)){
        frameReceiverStatus = FrameReceiverStatus::frameReceived;
    }else {
        frame.push_back(byte);
    }
}

void FrameReceiver::reset() {
    frame.clear();
    frameReceiverStatus = FrameReceiverStatus::receiving;
    shouldEscapeNexByte = false;
    highNipple = noHighNipple;
}

void FrameReceiver::setMaxFrameSize(int maxFrameSize) {
    FrameReceiver::maxFrameSize = maxFrameSize;
}

FrameReceiverStatus FrameReceiver::getFrameReceiverStatus() const {
    return frameReceiverStatus;
}

void FrameReceiver::setOnFrameError(const function<void(string)> &onFrameError) {
    FrameReceiver::onFrameError = onFrameError;
}
