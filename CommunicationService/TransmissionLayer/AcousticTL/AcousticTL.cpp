#include "AcousticTL.h"

using namespace std;
using namespace placeholders;

AcousticTL::AcousticTL() :
        frameReceiver(frameProtocol),
        paWrapper(sampleRate, bind(&AcousticTL::callback, this, _1)),
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
        paWrapper(sampleRate, bind(&AcousticTL::callback, this, _1))
{}

void AcousticTL::callback(PaCallbackData pcd) {
    float *in = (float*) pcd.inputBuffer;
    float *out = (float*) pcd.outputBuffer;

    if (state == ATLState::transmitting){
        for (int i = 0; i < pcd.framesPerBuffer; i++){
            if (outgoingSamples.size() > 0){
                float val = outgoingSamples.front();
                *out++ = outgoingSamples.front();
                outgoingSamples.pop();
            }else{
                *out++ = 0;
            }
        }
        if (outgoingSamples.size() == 0) {
            state = ATLState::idle;
            onFrameTransmittedCallback();
        }
            return;
    }

    for (int i = 0; i < pcd.framesPerBuffer; i++){
        incomingSamples.push(*in++);
        *out++ = 0;
    }

    if (state == ATLState::idle) {
        while (incomingSamples.size() >= samplesPerSearch) {
            sync.receiveNipple(getNextNipple(samplesPerSearch));
            if (sync.startSequenceReceived()){
                sync.reset();
                if(onStartSeqReceived) onStartSeqReceived();
                state = ATLState::receiving;
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
                onFrameReceiveCallback(frameReceiver.getFrame());
                frameReceiver = FrameReceiver(frameProtocol);
            }
        }
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

bool AcousticTL::sendFrame(std::vector<unsigned char> byteFrame) {
    if (outgoingSamples.size() != 0) return false;
    frameProtocol.packFrame(byteFrame);
    vector<float> samples = freqGeneration.byteFrameToSamples(byteFrame, sampleRate, samplesPerTone);
    for (float f : samples){
        outgoingSamples.push(f);
    }
    state = ATLState::transmitting;
    return true;
}

Sync &AcousticTL::getSync(){
    return sync;
}

void AcousticTL::setOnStartSeqReceived(const function<void()> &onStartSeqReceived) {
    AcousticTL::onStartSeqReceived = onStartSeqReceived;
}
