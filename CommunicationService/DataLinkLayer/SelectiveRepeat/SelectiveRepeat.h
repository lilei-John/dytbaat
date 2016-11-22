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
    int getMaxFrameSize();

private:
    std::stringstream *stream;                  // pointer to a stream

    unsigned int index = 0;                     // used to index where in the incoming stream we are
    unsigned int frameSize = 10;                // defines the number of bytes taken from the stream
    unsigned int static const totalSeqNo = 126;
    unsigned int const windowSize = totalSeqNo/uint8_t(2);
    unsigned int seqNo = 0;
    unsigned int firstOutstanding = 0;
    unsigned int const frameBlocksize = 4;
    unsigned int lastInBlock = firstOutstanding + frameBlocksize-1;
    unsigned int timerLength = 1500;               //in milliseconds

    std::vector<unsigned char> frame;           // the current frame we are working on
    std::vector<std::vector<unsigned char>> window;
    std::vector<unsigned char> incomingFrames[totalSeqNo];
    bool acknowledgedFrames[totalSeqNo] = {false};

    void getData();
    void makeFrame();
    void storeFrame();
    void sendFrame();

    void frameSplit();                          // splits the frame up, and dumps the header and the CRC checksum

    void incomingACK();        // handles an incoming ACK
    void incomingFrame();

    void startTimer();
    void timer();
    void timeOut();

    void addHeader();
    unsigned int calcCRC();
    void addCRC();

    void clearAll();

    bool isCrcValid();
    bool isStreamEmpty();
    bool isWindowFull(int firstInWindow, int lastInWindow);
    bool expectingACK = false;
    bool isNackNeeded = false;
    bool isSender = false;

    int framesToResend = 0;
    int timerCount = 0;


};