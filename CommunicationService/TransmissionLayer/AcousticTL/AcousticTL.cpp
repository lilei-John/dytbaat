#include "AcousticTL.h"

using namespace std;
using namespace placeholders;

AcousticTL::AcousticTL() :
        frameReceiver(frameProtocol),
        sampleRate(44100),
        samplesPerTone(1600),
        samplesPerSearch(500)
{}

AcousticTL::AcousticTL(
        const int sampleRate,
        const int samplesPerTone,
        const int samplesPerSearch) :
        frameReceiver(frameProtocol),
        sampleRate(sampleRate),
        samplesPerTone(samplesPerTone),
        samplesPerSearch(samplesPerSearch),
{}

void AcousticTL::processInput(const std::vector<float> &in) {
    if (state != ATLState::transmitting) {
        for (auto sample : in) incomingSamples.push(sample);
    }
    if (state == ATLState::idle) {
        while (incomingSamples.size() >= samplesPerSearch) {
            sync.receiveNipple(getNextNipple(samplesPerSearch));
            if (sync.startSequenceReceived()){
                sync.reset();
                state = ATLState::receiving;
                if(onStartSeqReceived) onStartSeqReceived();
                break;
            }
        }
    }
    if (state == ATLState::receiving){
        while(incomingSamples.size() >= samplesPerTone){
            for (int i = 0; i < samplesPerSearch; i++) incomingSamples.pop();
            frameReceiver.receiveNipple(getNextNipple(samplesPerTone - 2 * samplesPerSearch));
            for (int i = 0; i < samplesPerSearch; i++) incomingSamples.pop();
            if (frameReceiver.isWholeFrameReceived()){
                state = ATLState::idle;
                onFrameReceived(frameReceiver.getFrame());
                frameReceiver = FrameReceiver(frameProtocol);
            }
        }
    }
}

void AcousticTL::setOutput(std::vector<float> &out) {
    int i = 0;
    if (state == ATLState::transmitting){
        while (i < out.size() && outgoingSamples.size() > 0){
            out[i++] = outgoingSamples.front();
            outgoingSamples.pop();
        }
        if (outgoingSamples.size() == 0) state = ATLState::idle;
    }
    while (i < out.size()){
        out[i++] = 0;
    }
}

unsigned char AcousticTL::getNextNipple(int sampleCount) {
    vector<float> samples;
    for (int i = 0; i < sampleCount; i++){
        samples.push_back(incomingSamples.front());
        incomingSamples.pop();
    }
    DtmfAnalysis dtmfAnalysis(samples, dtmfSpec, sampleRate);
    return dtmfAnalysis.getNipple();
}

bool AcousticTL::sendFrame(const std::vector<unsigned char> &bytes) {
    vector<unsigned char> byteFrame = bytes;
    if (outgoingSamples.size() != 0) return false;
    frameProtocol.packFrame(byteFrame);
    vector<float> samples = freqGeneration.byteFrameToSamples(byteFrame, sampleRate, samplesPerTone);
    for (float f : samples){
        outgoingSamples.push(f);
    }
    state = ATLState::transmitting;
    return true;
}

void AcousticTL::setOnStartSeqReceived(const function<void()> &onStartSeqReceived) {
    AcousticTL::onStartSeqReceived = onStartSeqReceived;
}

Sync &AcousticTL::getSync(){
    return sync;
}