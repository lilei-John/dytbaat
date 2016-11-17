#include <unistd.h>
#include "StopAndWait.h"
using namespace std;

StopAndWait::StopAndWait() {
}

StopAndWait::StopAndWait(stringstream &inStream) {
    stream = &inStream;
}

void StopAndWait::getData() {
    frame.clear();
    unsigned int i = index;
    while (index <  i +frameSize and !isStreamEmpty()){      // * pointer that takes the content that is pointed at
        unsigned char byte;
        stream->seekg(index) >> noskipws >> byte;                        // seekg grabber and removes the object from the stream. and -> insted of a . because it is a pointer
        frame.push_back(byte);
        index++;
    }
}

void StopAndWait::addHeader() {
    frame.insert(frame.begin(), seqNo);
}

unsigned int StopAndWait::calcCRC() {
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

void StopAndWait::addCRC() {
    unsigned int crcChecksum = calcCRC();               // sets the remainder to the CRC Checksum
    unsigned char crcHigh = (unsigned char)(crcChecksum >> 8);
    unsigned char crcLow = crcChecksum & 0xFF;
    frame.push_back(crcHigh);
    frame.push_back(crcLow);
}

bool StopAndWait::isCrcValid() {
    return calcCRC()==0;
}

void StopAndWait::makeFrame() {
    addHeader();
    addCRC();
}

void StopAndWait::storeFrame() {
    storedFrame = frame;
}

void StopAndWait::timer() {
    timerCount++;
    std::this_thread::sleep_for(std::chrono::seconds(timerLength));
    timeOut();
    timerCount--;
}

void StopAndWait::getNextFrame() {
    getData();
    makeFrame();
    storeFrame();
    startTimer();
}

bool StopAndWait::isHeaderValid() {
    return seqNo == frame.at(0);
}

void StopAndWait::frameSplit() {
     frame.erase(frame.begin());
     for (int i = 0; i < 2; ++i) {
         frame.erase(frame.end()-1);
     }
}

void StopAndWait::timeOut() {
    if(isExpectingAck() && timerCount == 1) {
        onFrameSendReq(storedFrame);
        startTimer();
        if(onTimeout) onTimeout();
    }
}

void StopAndWait::incomingFrame(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    if(isCrcValid()){
        if(isHeaderValid()){
            frameSplit();
            for (auto byte : frame){
                *stream << noskipws << byte;
            }
            if(onReceive) onReceive();
            frame = getACK();
            storedFrame = frame;
            seqNoSwap();
            onFrameSendReq(frame);
        }else{
            onFrameSendReq(storedFrame);
            if(onFlowFail) onFlowFail();
        }
    }else{
        if(onCrcFail) onCrcFail();
    }
}

void StopAndWait::incomingACK(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    if(isCrcValid()){
        if(isHeaderValid()){
            onAckReceiveTime();
            seqNoSwap();
            ackFrameCount++;
            if (!isStreamEmpty()) {
                getNextFrame();
                sendFrame();
            }
        }else{
            if(onFlowFail) onFlowFail();
        }
    }else{
        if(onFlowFail) onCrcFail();
    }
}

void StopAndWait::seqNoSwap() {
    if (seqNo == 0){
        seqNo = 1;
    }else{
        seqNo = 0;
    }
}

std::vector<unsigned char> StopAndWait::getACK() {
    frame.clear();
    makeFrame();
    return frame;
}

void StopAndWait::receiveFrame(std::vector<unsigned char> frame) {
    if(isExpectingAck()){
        incomingACK(frame);
    }else{
        incomingFrame(frame);
    }
}

bool StopAndWait::isExpectingAck() {
    return ackFrameCount < sentFrameCount;
}

bool StopAndWait::isStreamEmpty() {
    unsigned char index0;
    return !(bool)(*stream >> index0);
}

void StopAndWait::sendFrame() {
    sentFrameCount++;
    if (onFrameSendReq(frame)){
        if (onFrameSend) onFrameSend();
    }
}

void StopAndWait::startTimer() {
    std::thread(&StopAndWait::timer, this).detach();
}

void StopAndWait::transmit() {
    if (!isStreamEmpty()){
        getNextFrame();
        sendFrame();
    }
}

void StopAndWait::setOnTimeout(std::function<void(void)> callback) {
    onTimeout = callback;
}

void StopAndWait::setOnCrcFail(std::function<void(void)> callback) {
    onCrcFail = callback;
}

void StopAndWait::setOnFlowFail(std::function<void(void)> callback) {
    onFlowFail = callback;
}

void StopAndWait::setOnFrameSendTime(std::function<void(void)> callback) {
    onFrameSend = callback;
}

void StopAndWait::setOnAckReceiveTime(std::function<void(void)> callback) {
    onAckReceiveTime = callback;
}
