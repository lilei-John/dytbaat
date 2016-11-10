#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <csignal>
#include <thread>
#include <future>
#include "../DataLinkLayer.h"


class SelectiveRepeat : public DataLinkLayer{
public:
    SelectiveRepeat();
    SelectiveRepeat(std::stringstream &inStream);
    void transmit();
    void receiveFrame(std::vector<unsigned char>);

private:
    std::stringstream *stream;                  // pointer to a stream

    unsigned int index = 0;                     // used to index where in the incoming stream we are
    unsigned int frameSize = 20;                // defines the number of bytes taken from the stream
    uint8_t totalSeqNo = 8;
    uint8_t windowSize = totalSeqNo/uint8_t(2);
    uint8_t seqNo = 0;
    uint8_t firstOutstanding = 0;

    std::vector<unsigned char> frame;           // the current frame we are working on
    std::vector<std::vector<unsigned char>> window;
    std::vector<std::vector<int>> whatToSendNext;

    void getData();
    void makeFrame();
    void storeFrame();
    void sendFrame();

    void incomingACK(std::vector<unsigned char> aFrame);        // handles an incoming ACK
    void retransmit();

    void startTimer();
    void timer();
    void timeOut();

    void addHeader();
    unsigned int calcCRC();
    void addCRC();
    bool isCrcValid();
    bool isStreamEmpty();
    bool expectingAck = false;
    bool isRetransmitting = false;

    int timerCount = 0;


};