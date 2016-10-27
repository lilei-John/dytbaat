//
// Created by mikke on 06-10-2016.
//
#include <unistd.h>
#include "StopAndWait.h"
using namespace std;

StopAndWait::StopAndWait() {
}

StopAndWait::StopAndWait(stringstream &inStream) {
    stream = &inStream;
}

void StopAndWait::getData() {
    vector<unsigned char> aFrame;
    unsigned int i = index;
    unsigned char index0;
    while (index <  i +frameSize and (*stream >> index0)){      // * pointer that takes the content that is pointed at
        stream->seekg(index) >> index0;                         // seekg grabber and removes the object from the stream. and -> insted of a . because it is a pointer
        aFrame.push_back(index0);
        index++;
    }
    frame = aFrame;
}

void StopAndWait::addHeader() {
    frame.insert(frame.begin(), seqNo);
    seqNoSwap();
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


bool StopAndWait::crcCheck() {
    if (calcCRC() == 0){
        return true;
    }
    return false;
 }

void StopAndWait::makeFrame() {
    addHeader();
    addCRC();
}

void StopAndWait::storeFrame() {
    storedFrame = frame;
}

void StopAndWait::startTimer() {

/*        std::thread([d](){
            std::this_thread::sleep_for(std::chrono::seconds(d));
            timeOut();
        }).detach();*/
    timerCount++;
    std::this_thread::sleep_for(std::chrono::seconds(timer));
    timeOut();
    timerCount--;

 }

std::vector<unsigned char> StopAndWait::getNextFrame() {
    if (readyToSend) {
        getData();                                          //if SENDER set empty vector here!
        makeFrame();
        storeFrame();
        std::thread(&StopAndWait::startTimer,this).detach();

        return frame;
    }
    else{
        std::vector<unsigned char> zero;
        return zero;
    }
}

bool StopAndWait::flowControl() {
    if (frame.at(0) == seqNo){
        seqNoSwap();
    return true;
}
    return false;
}

void StopAndWait::frameSplit() {
     frame.erase(frame.begin() );
     for (int i = 0; i < 2; ++i) {
         frame.erase(frame.end()-1);
     }
}

std::vector<unsigned char> StopAndWait::getPreviousFrame() {
    return storedFrame;
}

void StopAndWait::timeOut() {
    if(timerCount==1) {
        cout << "TIMEOUT" << endl;
    }
}

bool StopAndWait::frameControl() {                          // ###########################################################################################
    return crcCheck() && !flowControl();                    // REMEMBER THE ! IS ONLY FOR TESTS!   #######################################################
                                                            // ###########################################################################################

}

void StopAndWait::incomingFrame(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    readyToSend = frameControl();
    if(frameControl()){
        frameSplit();
    }

}

void StopAndWait::incomingACK(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    unsigned char index0;
    if (*stream >> index0) {
        readyToSend = frameControl();
    }
    else{
        readyToSend = false;
    }
}


void StopAndWait::seqNoSwap() {
    if (seqNo == 0){
        seqNo = 0b00000001;
    }
    else{
        seqNo = 0b00000000;
    }
}

std::vector<unsigned char> StopAndWait::getACK() {
    vector<unsigned char> emptyACK;
    if (readyToSend){
        frame = emptyACK;
        makeFrame();
        return frame;
    }
    else {
        return emptyACK;
    }
}