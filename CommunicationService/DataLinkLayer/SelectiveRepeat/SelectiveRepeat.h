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



private:
    std::stringstream *stream;                  // pointer to a stream

    unsigned int index = 0;                     // used to index where in the incoming stream we are
    unsigned int frameSize = 20;                // defines the number of bytes taken from the stream
    unsigned int seqNo = 0;
    unsigned int windowIndex = 0;

    std::vector<unsigned char> frame;           // the current frame we are working on
    std::vector<std::vector<unsigned char>> window;
    std::vector<std::vector<unsigned char>> framesToBeSend;

    void getData();
    void addHeader();
    unsigned int calcCRC();
    void addCRC();
    bool isCrcValid();
    bool isStreamEmpty();






};