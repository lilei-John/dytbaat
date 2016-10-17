#include "FrameProtocol.h"

FrameProtocol::FrameProtocol() :
    startBytes({0x0F, 0x0A}),
    stopByte(0x0F),
    escapeByte(0x5A)
{}

bool FrameProtocol::isStopByte(unsigned char byte) {
    return byte == stopByte;
}

bool FrameProtocol::isEscapeByte(unsigned char byte) {
    return byte == escapeByte;
}

const std::vector<unsigned char> &FrameProtocol::getStartBytes() const {
    return startBytes;
}

unsigned char FrameProtocol::getStopByte() const {
    return stopByte;
}

unsigned char FrameProtocol::getEscapeByte() const {
    return escapeByte;
}
