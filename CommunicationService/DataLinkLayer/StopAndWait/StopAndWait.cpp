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
        stream->seekg(index) >> byte;                        // seekg grabber and removes the object from the stream. and -> insted of a . because it is a pointer
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
    return calcCRC() == 0;
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
    if(timerCount == 1) {
        onFrameSendCallback(storedFrame);
        startTimer();
    }
}

bool StopAndWait::isFrameValid() {
    return isCrcValid() && isHeaderValid();
}

void StopAndWait::incomingFrame(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    if(isFrameValid()){
        frameSplit();
        for (auto byte : frame){
            *stream << byte;
        }
        frame = getACK();
        seqNoSwap();
        onFrameSendCallback(frame);
    }else{
        //Vi gør ikke noget. Et timeout på afsendersiden vil gensende tidligere frame.
    }
}

void StopAndWait::incomingACK(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    if (isFrameValid()){
        seqNoSwap();
        ackFrameCount++;
        if (!isStreamEmpty()) {
            getNextFrame();
            sendFrame();
        }
    }else{
        //Vi gør ikke noget. Et timeout vil gensende tidligere frame, hvis ack ikke modtages.
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

void StopAndWait::recieveFrame(std::vector<unsigned char> frame) {
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
    onFrameSendCallback(frame);
}

void StopAndWait::startTimer() {
    return;
    std::thread(&StopAndWait::timer, this).detach();
}

void StopAndWait::transmit() {
    if (!isStreamEmpty()){
        getNextFrame();
        sendFrame();
    }
}