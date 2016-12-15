#include <iostream>
#include <fstream>
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
            if (sync.trySync(incomingSamples, samplesToBeSaved)){
                state = ATLState::receiving;
                if(onStartSeqReceived) onStartSeqReceived();
            }
        }
        if (state == ATLState::receiving){
            frameReceiver.processInput(incomingSamples, samplesToBeSaved);
            if (frameReceiver.getFrameReceiverStatus() == FrameReceiverStatus::frameReceived){
                state = ATLState::idle;
                auto frame = frameReceiver.getFrame();
                frameReceiver.reset();
                ofstream sf("savedfile" + to_string(saveCount++) + ".csv");
                sf << sampleRate << endl;
                sf << samplesPerTone << endl;
                while (samplesToBeSaved.size() > 0){
                    sf << samplesToBeSaved.front() << endl;
                    samplesToBeSaved.pop();
                }
                sf.close();
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
            out[i++] = currentDtmfTone.next();
            if (currentDtmfTone.isDone()){
                if (outgoingNibbles.size() == 0){
                    state = ATLState::idle;
                    if(onFrameTransmitted) onFrameTransmitted();
                    if (state != ATLState::transmitting)
                        break;
                    else
                        continue;
                }else{
                    auto nibble = outgoingNibbles.front();
                    outgoingNibbles.pop();
                    Fade fade = outgoingNibbles.size() == 0 ? Fade::fadeOut : Fade::noFade;
                    auto f = dtmfSpec.nibbleToFreqs(nibble);
                    currentDtmfTone = DtmfTone(f.first, f.second,
                                               currentDtmfTone.getP1(),
                                               currentDtmfTone.getP2(),
                                               fade, sampleRate, samplesPerTone);
                }
            }
        }
    }
    while (i < out.size())
        out[i++] = 0;
}

bool AcousticTL::sendFrame(const std::vector<unsigned char> &bytes) {
    if (outgoingNibbles.size() != 0 || !currentDtmfTone.isDone()) return false;
    auto byteFrame = bytes;
    frameProtocol.escapeByteFrame(byteFrame);
    byteFrame.push_back(frameProtocol.getStopByte());
    auto nibbleFrame = byteFrameToNibbleFrame(byteFrame);
    nibbleFrame.insert(nibbleFrame.begin(), sync.getStartNibbles().begin(), sync.getStartNibbles().end());
    nibbleFrame.insert(nibbleFrame.begin(), sync.getPaddingNibble());
    nibbleFrame.push_back(sync.getPaddingNibble());
    for (auto nibble : nibbleFrame)
        outgoingNibbles.push(nibble);
    auto nibble = outgoingNibbles.front();
    outgoingNibbles.pop();
    auto f = dtmfSpec.nibbleToFreqs(nibble);
    currentDtmfTone = DtmfTone(f.first, f.second, 0, 0, Fade::fadeIn, sampleRate, samplesPerTone);
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
    toneCount += sync.getStartNibbles().size();
    toneCount += 2; //paddingNibbles
    return chrono::milliseconds((toneCount * samplesPerTone * 1000) / sampleRate);
}
