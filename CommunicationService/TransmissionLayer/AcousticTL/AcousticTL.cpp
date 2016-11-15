#include "AcousticTL.h"
#include "helperFunctions.h"

using namespace std;

AcousticTL::AcousticTL(
        const int sampleRate,
        const int samplesPerTone)
        :
        sampleRate(sampleRate),
        samplesPerTone(samplesPerTone),
        frameReceiver(frameProtocol, dtmfSpec, samplesPerTone, sampleRate)
{}

void AcousticTL::processInput(const std::vector<float> &in) {
    if (state != ATLState::transmitting) {
        for (auto sample : in) incomingSamples.push(sample);
    }
    if (state == ATLState::idle) {
        if (sync.trySync(incomingSamples)){
            state = ATLState::receiving;
            if(onStartSeqReceived) onStartSeqReceived();
        }
    }
    if (state == ATLState::receiving){
        frameReceiver.processInput(incomingSamples);
        if (frameReceiver.isWholeFrameReceived()){
            state = ATLState::idle;
            auto frame = frameReceiver.getFrame();
            frameReceiver.reset();
            onFrameReceived(frame);
        }
    }
}

void AcousticTL::setOutput(std::vector<float> &out) {
    int i = 0;
    if (state == ATLState::transmitting){
        while (i < out.size()){
            if (outgoingSamples.size() == 0){
                state = ATLState::idle;
                if(onFrameTransmitted) onFrameTransmitted();
                if (state != ATLState::transmitting) break;
            }
            out[i++] = outgoingSamples.front();
            outgoingSamples.pop();
        }
    }
    while (i < out.size()){
        out[i++] = 0;
    }
}

bool AcousticTL::sendFrame(const std::vector<unsigned char> &bytes) {
    if (outgoingSamples.size() != 0) return false;
    auto byteFrame = bytes;
    frameProtocol.escapeByteFrame(byteFrame);
    byteFrame.push_back(frameProtocol.getStopByte());
    auto nibbleFrame = byteFrameToNibbleFrame(byteFrame);
    nibbleFrame.insert(nibbleFrame.begin(), sync.getSyncNibbles().begin(), sync.getSyncNibbles().end());
    nibbleFrame.insert(nibbleFrame.begin(), 0x0);
    nibbleFrame.push_back(0x0);
    auto samples = freqGeneration.nibbleFrameToSamples(nibbleFrame, sampleRate, samplesPerTone);
    for (auto sample : samples) outgoingSamples.push(sample);
    state = ATLState::transmitting;
    return true;
}

void AcousticTL::setOnStartSeqReceived(const function<void()> &onStartSeqReceived) {
    AcousticTL::onStartSeqReceived = onStartSeqReceived;
}

Sync &AcousticTL::getSync(){
    return sync;
}