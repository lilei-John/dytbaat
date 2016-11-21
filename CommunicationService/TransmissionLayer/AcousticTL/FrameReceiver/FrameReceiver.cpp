#include <iostream>
#include "FrameReceiver.h"

using namespace std;

FrameReceiver::FrameReceiver(FrameProtocol f, DtmfSpec dtmfSpec, int samplesPerTone, int sampleRate) :
        frameProtocol(f),
        dtmfSpec(dtmfSpec),
        samplesPerTone(samplesPerTone),
        sampleRate(sampleRate)
{}

bool FrameReceiver::isWholeFrameReceived() {
    return wholeFrameReceived;
}

vector<unsigned char> FrameReceiver::getFrame() {
    return frame;
}

void FrameReceiver::processInput(std::queue<float> &samples) {
    while(samples.size() >= samplesPerTone && !wholeFrameReceived){
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
    frame.clear();
    wholeFrameReceived = false;
    shouldEscapeNexByte = false;
    highNipple = noHighNipple;
}
