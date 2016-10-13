#include "AcousticTL.h"
#include "DtmfSpec/DtmfSpec.h"
#include <vector>

using namespace std;

AcousticTL::AcousticTL() : paWrapper(SAMPLERATE, &callback) {

}

void AcousticTL::callback(PaCallbackData pcd) {
    float *in = (float*) pcd.inputBuffer;
    float *out = (float*) pcd.outputBuffer;

    if (state == ATLState::transmitting){

        return;
    }

    for (int i = 0; i < pcd.framesPerBuffer; i++){
        incomingSamples.push(*in++);
        *out++ = 0;
    }

    if (state == ATLState::idle) {
        while (incomingSamples.size() >= SAMPLES_PER_SEARCH) {
            sync.receiveNipple(getNextNipple(SAMPLES_PER_SEARCH));
            if (sync.startSequenceReceived()){
                sync.reset();
                state = ATLState::receiving;
                break;
            }
        }
    }

    if (state == ATLState::receiving){
        while(incomingSamples.size() >= SAMPLES_PER_TONE){
            frameReceiver.receiveNipple(getNextNipple(SAMPLES_PER_TONE));
            if (frameReceiver.isWholeFrameReceived()){
                doSomethingWithFrame(frameReceiver.getFrame());
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
    return freqAnalysisToNipple(FreqAnalysis(samples, DtmfSpec::getFreqs()));
}
