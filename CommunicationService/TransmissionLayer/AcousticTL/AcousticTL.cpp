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
    if (state == ATLState::transmitting) return;

    for (auto sample : in) incomingSamples.push(sample);

    while (true){
        if (state == ATLState::idle) {
            if (sync.trySync(incomingSamples)){
                state = ATLState::receiving;
                if(onStartSeqReceived) onStartSeqReceived();
            }
        }
        if (state == ATLState::receiving){
            frameReceiver.processInput(incomingSamples);
            if (frameReceiver.getFrameReceiverStatus() == FrameReceiverStatus::frameReceived){
                state = ATLState::idle;
                auto frame = frameReceiver.getFrame();
                frameReceiver.reset();
                onFrameReceived(frame);
            }else if(frameReceiver.getFrameReceiverStatus() == FrameReceiverStatus::frameError){
                state = ATLState::idle;
                frameReceiver.reset();
                continue;
            }
        }
        break;
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
    nibbleFrame.insert(nibbleFrame.begin(), sync.getPaddingNibble());
    nibbleFrame.push_back(sync.getPaddingNibble());
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

FrameReceiver &AcousticTL::getFrameReceiver(){
    return frameReceiver;
}

void AcousticTL::setMaxFrameSize(int size) {
    frameReceiver.setMaxFrameSize(size);
}

const FrameProtocol &AcousticTL::getFrameProtocol() const {
    return frameProtocol;
}


chrono::milliseconds AcousticTL::getMaxTransmissionDuration(unsigned int frameSize, int maxEscapableBytes = -1) {
    if (maxEscapableBytes == -1) maxEscapableBytes = frameSize;
    frameSize += maxEscapableBytes;
    frameSize += 1; //stopByte
    int toneCount = frameSize * 2;
    toneCount += sync.getSyncNibbles().size();
    toneCount += 2; //paddingNibbles
    return chrono::milliseconds((toneCount * samplesPerTone * 1000) / sampleRate);
}
