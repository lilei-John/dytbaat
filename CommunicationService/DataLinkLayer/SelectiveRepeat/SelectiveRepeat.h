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
    void frameTransmitted();
    void receiveFrame(std::vector<unsigned char>);

private:
    std::stringstream *stream;                  // pointer to a stream

    unsigned int index = 0;                     // used to index where in the incoming stream we are
    unsigned int frameSize = 20;                // defines the number of bytes taken from the stream
    uint8_t const totalSeqNo = 8;
    uint8_t const windowSize = totalSeqNo/uint8_t(2);
    uint8_t seqNo = 0;
    uint8_t firstOutstanding = 0;
    unsigned int const frameBlocksize = 4;
    unsigned int lastOutstanding = firstOutstanding + frameBlocksize;

    std::vector<unsigned char> frame;           // the current frame we are working on
    std::vector<std::vector<unsigned char>> window;
    std::vector<std::vector<int>> whatToSendNext;
    std::vector<unsigned char> incomingFrames[totalSeqNo];
    bool acknowledgedFrames[totalSeqNo] = {false};

    void getData();
    void makeFrame();
    void storeFrame();
    void sendFrame();

    void incomingACK(std::vector<unsigned char> aFrame);        // handles an incoming ACK
    void incomingFrame(std::vector<unsigned char> aFrame);

    void startTimer();
    void timer();
    void timeOut();

    void addHeader();
    unsigned int calcCRC();
    void addCRC();
    bool isCrcValid();
    bool isStreamEmpty();
    bool isWindowFull(int firstInWindow, int lastInWindow);
    bool expectingReply = false;

    int framesToResend = 0;
    int timerCount = 0;


};