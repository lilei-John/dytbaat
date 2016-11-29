#pragma once

#include <vector>

class FrameProtocol {
public:
    FrameProtocol();
    bool isStopByte(unsigned char);
    bool isEscapeByte(unsigned char);

    void escapeByteFrame(std::vector<unsigned char> &);

    unsigned char getStopByte() const;
    unsigned char getEscapeByte() const;
    bool shouldEscape(unsigned char) const;
private:
    unsigned char stopByte;
    unsigned char escapeByte;
};