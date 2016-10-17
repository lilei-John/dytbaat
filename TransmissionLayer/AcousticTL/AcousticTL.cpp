#include "AcousticTL.h"
#include "DtmfSpec/DtmfSpec.h"
#include <vector>

using namespace std;
using namespace placeholders;

AcousticTL::AcousticTL() :
        frameReceiver(frameProtocol),
        paWrapper(sampleRate, bind(&AcousticTL::callback, this, _1)),
        sampleRate(44100),
        samplesPerTone(5000),
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
                *out++ = outgoingSamples.front();
                outgoingSamples.pop();
            }else{
                *out++ = 0;
            }
        }
        if (outgoingSamples.size() == 0) state = ATLState::idle;
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
                state = ATLState::receiving;
                break;
            }
        }
    }

    if (state == ATLState::receiving){
        while(incomingSamples.size() >= samplesPerTone){
            frameReceiver.receiveNipple(getNextNipple(samplesPerTone));
            if (frameReceiver.isWholeFrameReceived()){
                //doSomethingWithFrame(frameReceiver.getFrame());
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
    return 0b0;//freqAnalysisToNipple(FreqAnalysis(samples, DtmfSpec::getFreqs()));
}

void AcousticTL::sendFrame(std::vector<unsigned char> byteFrame) {
    //Pak frame først
    vector<float> samples = freqGeneration.byteFrameToSamples(byteFrame, sampleRate, samplesPerTone);
    for (float f : samples){
        outgoingSamples.push(f);
    }
    state = ATLState::transmitting;
}