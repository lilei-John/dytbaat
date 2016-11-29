#include "FrameProtocol.h"

FrameProtocol::FrameProtocol() :
    stopByte(0xAF),
    escapeByte(0x9E)
{}

bool FrameProtocol::isStopByte(unsigned char byte) {
    return byte == stopByte;
}

bool FrameProtocol::isEscapeByte(unsigned char byte) {
    return byte == escapeByte;
}

unsigned char FrameProtocol::getStopByte() const {
    return stopByte;
}

unsigned char FrameProtocol::getEscapeByte() const {
    return escapeByte;
}

bool FrameProtocol::shouldEscape(unsigned char byte) const {
    return byte == stopByte || byte == escapeByte;
}

void FrameProtocol::escapeByteFrame(std::vector<unsigned char> &byteFrame) {
    for (int i = 0; i < byteFrame.size(); i++){
        if (shouldEscape(byteFrame[i])){
            byteFrame.insert(byteFrame.begin() + i, escapeByte);
            i++;
        }
    }
}
