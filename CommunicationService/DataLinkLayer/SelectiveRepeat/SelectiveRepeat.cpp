//
// Created by quist on 11/8/16.
//

#include "SelectiveRepeat.h"
using namespace std;

SelectiveRepeat::SelectiveRepeat() {

}

SelectiveRepeat::SelectiveRepeat(std::stringstream &inStream) {
    stream = &inStream;
}

void SelectiveRepeat::getData() {
    frame.clear();
    unsigned int i = index;
    while (index <  i +frameSize and !isStreamEmpty()){      // * pointer that takes the content that is pointed at
        unsigned char byte;
        stream->seekg(index) >> noskipws >> byte;                        // seekg grabber and removes the object from the stream. and -> insted of a . because it is a pointer
        frame.push_back(byte);
        index++;
    }
}

bool SelectiveRepeat::isStreamEmpty() {
    unsigned char index0;
    return !(bool)(*stream >> index0);
}

void SelectiveRepeat::addHeader() {
    frame.insert(frame.begin(), seqNo);
//  frame.insert(frame.begin(), firstOutstanding);
}

unsigned int SelectiveRepeat::calcCRC() {
#define poly 0x11021
    unsigned int remainder = 0;
    for (unsigned int num = 0; num < frame.size(); num++) {
        remainder ^= (frame[num] << 8);                 //XOR into crc top-byte
        for (int i = 0; i < 8; i++) {
            if (remainder & (1<<15)) {                  //Bit 15 is set
                remainder = (remainder << 1) ^ poly;    //Rotate and XOR with poly
            }
            else {                                      //Bit 15 is clear
                remainder = remainder << 1;             //Rotate one left
            }
        }
    }
    return remainder;
}

void SelectiveRepeat::addCRC() {
    unsigned int crcChecksum = calcCRC();               // sets the remainder to the CRC Checksum
    unsigned char crcHigh = (unsigned char)(crcChecksum >> 8);
    unsigned char crcLow = crcChecksum & 0xFF;
    frame.push_back(crcHigh);
    frame.push_back(crcLow);
}

bool SelectiveRepeat::isCrcValid() {
    return calcCRC()==0;
}

void SelectiveRepeat::frameTransmitted() {
    transmit();
}

void SelectiveRepeat::transmit() {
    if(framesToResend > 0){
        frame = window[window.size()-framesToResend];
        sendFrame();
        framesToResend--;
    }else{
        if(!isStreamEmpty() && window.size() < frameBlocksize && !isWindowFull(firstOutstanding, seqNo)){
            getData();
            makeFrame();
            storeFrame();
            sendFrame();
            seqNo = (seqNo++)%totalSeqNo;
        }
        else{
            expectingReply = true;
            startTimer();
        }
    }
}

void SelectiveRepeat::makeFrame() {
    addHeader();
    addCRC();
}

void SelectiveRepeat::storeFrame() {
    window.push_back(frame);
}

void SelectiveRepeat::sendFrame() {
    onFrameSendCallback(frame);
//  onFrameSendTime();
}

void SelectiveRepeat::startTimer() {
    std::thread(&SelectiveRepeat::timer, this).detach();
}

void SelectiveRepeat::timer() {
    timerCount++;
    std::this_thread::sleep_for(std::chrono::seconds(window.size()*timerLength));
    timeOut();
    timerCount--;
}

void SelectiveRepeat::timeOut() {
    if(expectingReply && timerCount == 1) {
        startTimer();
 //     if(onTimeout) onTimeout();
    }
}

void SelectiveRepeat::receiveFrame(std::vector<unsigned char> aFrame) {
    if(expectingReply){
        incomingACK(aFrame);
    }else{
        incomingFrame(aFrame);
    }

}

void SelectiveRepeat::incomingACK(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    if(isCrcValid()){
        expectingReply = false;
        firstOutstanding = frame[0];
        if (firstOutstanding==seqNo){
            window.clear();
            transmit();
        }else{
            for(int i = 0, j = 0; j < window.size(); ){ // Continue through all NAK's in frame
                if(frame[i]!=window[j][0]){         // If i'th NAK != j'th frame-seqNo in window
                    window.erase(window.begin()+j); // Delete j'th element in window (no need to resend)
                }else{                              // Keep in window (need to resend)
                    j++;
                    if(i<frame.size()-3){
                        i++;
                    }
                }
            }

            framesToResend = window.size();
            transmit();
        }
    }else{
//      onCrcFail();
    }

}

bool SelectiveRepeat::isWindowFull(int firstInWindow, int lastInWindow) {
    return (firstInWindow<=windowSize&&!(0>=(lastInWindow-firstInWindow)<windowSize) ||
            (firstInWindow>windowSize&&(0>(lastInWindow-firstInWindow)<=-windowSize)));
}

void SelectiveRepeat::incomingFrame(std::vector<unsigned char> aFrame) {
    frame = aFrame;
/*    for (auto i : frame) {
        cout << i;
    }
    cout << endl;*/
    if(isCrcValid()) {
        uint8_t incomingSeqNo = frame[0];

        if (acknowledgedFrames[incomingSeqNo]) {  // If frame is already received
            resendNak();                        // Resend NAK
        } else {
            acknowledgedFrames[incomingSeqNo] = true;   // Mark frame as received
            frameSplit();
            incomingFrames[incomingSeqNo] = frame;      // Copy frame to incomingFrames array
        }

        if (incomingSeqNo == firstOutstanding) {    // If received frame equals firstOutstanding
            for (uint8_t i = firstOutstanding; acknowledgedFrames[i]; i = (i++) % totalSeqNo) {
                for (auto byte : incomingFrames[i]) {
                    *stream << noskipws << byte;    // Send to stream
                }
            }

        }

        if (incomingSeqNo == lastOutstanding) { // If received frame equals lastOutstanding
            frame.clear();                  // Clear frame
            for (uint8_t i = firstOutstanding;
                 i != (lastOutstanding + 1) % totalSeqNo; i = (i++) % totalSeqNo) { // Make NAK frame
                if (acknowledgedFrames[i]) {  // If i'th frame is alright
                    firstOutstanding = (firstOutstanding++) % totalSeqNo; // Increment firstOutstanding
                } else {
                    frame.push_back(i); // If not: add seqNo to NAK frame
                }
            }
            if (frame.size() == 0) {    // If no NAK's have been added (resulting in NAK frame still empty)
                frame.push_back(firstOutstanding);  // Add the new window's firstOutstanding to frame
            }

            // Make new receive window
            lastOutstanding = firstOutstanding; // Worst case: lastOutstanding = firstOutstanding
            for (int i = 1; i < frameBlocksize; i++) {    // Create new "best case" full size window
                do {
                    lastOutstanding = (lastOutstanding++) % totalSeqNo;   // Increment lastOutstanding
                } while (acknowledgedFrames[lastOutstanding]); // While acknowledged
            }

            while (isWindowFull(firstOutstanding, lastOutstanding)) { // Decrease window while full
                do {
                    lastOutstanding = (lastOutstanding--) % totalSeqNo;   // Decrement lastOutstanding
                } while (acknowledgedFrames[lastOutstanding]); // While acknowledged
            }

            sendNak();
        }

    }

/*
        if(isHeaderValid()){
            frameSplit();
            for (auto byte : frame){
                *stream << noskipws << byte;
            }
            frame = getACK();
            storedFrame = frame;
            seqNoSwap();
            onFrameSendCallback(frame);
        }else{
            onFrameSendCallback(storedFrame);
            onFlowFail();
        }
    }else{
        onCrcFail();
    }
    */
}
