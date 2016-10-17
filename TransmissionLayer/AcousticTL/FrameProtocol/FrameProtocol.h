#pragma once

#include <vector>

class FrameProtocol {
public:
    FrameProtocol();
    bool isStopByte(unsigned char);
    bool isEscapeByte(unsigned char);

    const std::vector<unsigned char> &getStartBytes() const;
    unsigned char getStopByte() const;
    unsigned char getEscapeByte() const;
private:
    std::vector<unsigned char> startBytes;
    unsigned char stopByte;
    unsigned char escapeByte;
};