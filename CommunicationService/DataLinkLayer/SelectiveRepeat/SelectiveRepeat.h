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
private:
    // SETTINGS
    unsigned int const FRAME_SIZE = 20;                     // the number of data bytes in a frame
    unsigned int static const TOTAL_SEQ_NO = 128;           // the size of the sequence numbering
    unsigned int const FRAMEBLOCK_SIZE = 10;                // the number of frames transmitted before awaiting an ACK/NAK

public:
    SelectiveRepeat();
    SelectiveRepeat(std::stringstream &inStream);
    void transmit();                                        //
    void frameTransmitted();                                //
    void receiveFrame(std::vector<unsigned char>);          //
    int getMaxFrameSize();                                  //

    //Test functions
    void setOnTimeout(std::function<void(void)> callback);
    void setOnCrcFail(std::function<void(std::vector<unsigned char>)> callback);
    void setOnFrameSendTime(std::function<void(std::vector<unsigned char>)> callback);
    void setOnAckReceiveTime(std::function<void(std::vector<unsigned char>)> callback);
    void setOnFrameReceive(std::function<void(int)> callback);

    int getTotalSeqNo();
    int getFrameBlockSize();

private:
    // Variables
    unsigned int index = 0;                                 // used to index where in the incoming stream we are
    uint8_t seqNo = 0;                                      //
    uint8_t firstOutstanding = 0;                           // first outstanding sequence number (first in the window)
    unsigned int lastInBlock = firstOutstanding + FRAMEBLOCK_SIZE-1;    // Last in block of transmitted frames
    int framesToResend = 0;                                 // number of  old frames need to be resend
    int timerCount = 0;                                     // number of aktive timers. a timeout occurs only if timerCount is 1
    unsigned int const WINDOW_SIZE = TOTAL_SEQ_NO/2;        // the window of "out of order" frames
   // Vectors and arrays
    std::stringstream *stream;                              //
    std::vector<unsigned char> frame;                       //
    std::vector<std::vector<unsigned char>> window;         //
    std::vector<unsigned char> incomingFrames[TOTAL_SEQ_NO];//
    bool acknowledgedFrames[TOTAL_SEQ_NO] = {false};        //

    // SENDER
    void getData();                                         // takes the bytes from the stream and places them in the frame vector
    void makeFrame();                                       // adds the header and CRC checksum
    void storeFrame();                                      // saves the current frame in the window
    void sendFrame();                                       // sends the frame to the Transmission layer
    void startTimer();                                      // starts the timer when the last frame in the block is transmitted
    void timer();                                           //
    void timeOut();                                         //
    void incomingACK();                                     // SENDER:      handles an incoming ACK
    // RECEIVER
    void incomingFrame();                                   // RECEIVER:    handles an incoming frame
    void frameSplit();                                      // splits the frame up, and dumps the header and the CRC checksum
    void makeNak();                                         // create the NAK
    void adjustWindow();                                    // adjust the RECEIVER window
    // BOTH
    void clearAll();                                        // clears all variables
    void addHeader();                                       // adds the header to the frame
    void addCRC();                                          // adds the CRC checksum to the frame
    unsigned int calcCRC();                                 // calculates the CRC checksum
    // BOOLIANS
    bool isCrcValid();
    bool isStreamEmpty();
    bool isWindowFull(int firstInWindow, int lastInWindow);
    bool expectingACK = false;
    bool isNackNeeded = false;
    bool isSender = false;
    // TEST callbacks
    std::function<void(void)> onTimeout;
    std::function<void(std::vector<unsigned char>)> onCrcFail;
    std::function<void(std::vector<unsigned char>)> onFrameSend;
    std::function<void(std::vector<unsigned char>)> onAckReceiveTime;
    std::function<void(int)> onFrameReceive;

};