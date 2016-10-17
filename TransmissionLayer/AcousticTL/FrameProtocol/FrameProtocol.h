#pragma once

#include <vector>

class FrameProtocol {
public:
    FrameProtocol();
    bool isStopByte(unsigned char);
    bool isEscapeByte(unsigned char);
private:
    std::vector<unsigned char> startBytes;
    unsigned char stopByte;
    unsigned char escapeByte;
};