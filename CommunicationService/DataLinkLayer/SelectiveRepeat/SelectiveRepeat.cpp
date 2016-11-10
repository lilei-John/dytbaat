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

void SelectiveRepeat::transmit() {
    if(isRetransmitting){
        frame = window[0];
        sendFrame();
        window.erase(window.begin()); //det er noget crap
        if(window.size()==0){
            isRetransmitting = false;
        }
    }
    if(!isStreamEmpty()&&window.size()< windowSize){
        getData();
        makeFrame();
        storeFrame();
        sendFrame();
        if(seqNo>=totalSeqNo)
        {
            seqNo = 0;
        }else{
            seqNo++;
        }
    }
    else{
        expectingAck = true;
        startTimer();
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
    std::this_thread::sleep_for(std::chrono::seconds(timerLength));
    timeOut();
    timerCount--;
}

void SelectiveRepeat::timeOut() {
    if(expectingAck && timerCount == 1) {
        startTimer();
 //     if(onTimeout) onTimeout();
    }
}

void SelectiveRepeat::receiveFrame(std::vector<unsigned char> aFrame) {
    if(expectingAck){
        incomingACK(aFrame);
    }else{
        incomingFrame(aFrame);
    }

}

void SelectiveRepeat::incomingACK(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    if(isCrcValid()){
        if (frame[0]==seqNo){
            window.clear();
            transmit();
        }else{
            for(auto i = 0, j = 0, k = window.size(); i < k; ){
                if(frame[i]!=window[j][0]){
                    j++;
                    i++;
                }else{
                    window.erase(window.begin()+j);
                }
            }
            isRetransmitting = true;
            transmit();

        }
        checkAckHeader();


        if(isHeaderValid()){
//          onAckReceiveTime();
            expectingAck = false;
            seqNoSwap();
            ackFrameCount++;
            if (!isStreamEmpty()) {
                getNextFrame();
                sendFrame();
            }
        }else{
//          onFlowFail();
        }
    }else{
//      onCrcFail();
    }

}

void SelectiveRepeat::retransmit() {
    frame = window[0];
    sendFrame();
}
