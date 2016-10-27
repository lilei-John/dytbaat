#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <csignal>
#include <thread>
#include <future>
#include "../DataLinkLayer.h"

class StopAndWait : public DataLinkLayer{

public:
    StopAndWait();
    StopAndWait(std::stringstream &stream);
    virtual ~StopAndWait() {};

    // SENDER
    void transmit();
    void getData();                                             // grabs a amount of bytes from a stream and adds a header
    void makeFrame();                                           // adds header and crcChecksum
    void storeFrame();                                          // saves the current frame
    void startTimer();
    void timer();                                          // starts a timer, so that if no ACK has been returned the frame is resend
    void getNextFrame();                  // getData, makeFrame, storeFrame, timer, return frame
    void incomingACK(std::vector<unsigned char> aFrame);        // handles an incoming ACK, if true call getNextFrame

    //RECEIVER
    bool isFrameValid();                                        // isCrcValid & isHeaderValid
    void incomingFrame(std::vector<unsigned char> aFrame);      // handles an incoming Frame
    std::vector<unsigned char> getACK();

    //TIME_OUT
    void timeOut();                                             // timer & sendPreviousFrame
    void recieveFrame(std::vector<unsigned char>);

private:
    std::stringstream *stream;                  // pointer to a stream
    unsigned char seqNo = 0;           // Initialize to 00000000 (sequence 0). 00000001 = sequence 1.
    unsigned int index = 0;                     // used to index where in the incoming stream we are
    unsigned int timerLength = 3;                     // timer value in seconds
    unsigned int timerCount = 0;
    unsigned int frameSize = 20;
    std::vector<unsigned char> frame;           // the current frame we are working on
    std::vector<unsigned char> storedFrame;     // when the current frame is send, it is saved in the storedFrame if needed to be resend

    int sentFrameCount = 0;
    int ackFrameCount = 0;

    void sendFrame();

    void addHeader();                           // adds a header as the first byte of the frame
    unsigned int calcCRC();                     // sets the remainder to the CRC Checksum
    void addCRC();                              // calculates the CRC checksum and adds it to the array
    bool isCrcValid();                            // using the CRC divisor to check for errors
    bool isHeaderValid();                         // checking if the received frame has the appropriate sequence number
    void frameSplit();                          // splits the frame up, and dumps the header and the CRC checksum
    void seqNoSwap();                           // changes the seqNo between 0 and 1

    bool isExpectingAck();
    bool isStreamEmpty();
};