#include "FrameProtocol.h"

FrameProtocol::FrameProtocol() :
    startBytes({0x05, 0xAF}),
    stopByte(0xAF),
    escapeByte(0x9E)
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

bool FrameProtocol::shouldEscape(unsigned char byte) const {
    return byte == stopByte || byte == escapeByte;
}

void FrameProtocol::packFrame(std::vector<unsigned char> &byteFrame) {
    for (int i = 0; i < byteFrame.size(); i++){
        if (shouldEscape(byteFrame[i])){
            byteFrame.insert(byteFrame.begin() + i, escapeByte);
        }
    }
    byteFrame.insert(byteFrame.begin(), startBytes.begin(), startBytes.end());
    byteFrame.insert(byteFrame.end(), stopByte);
}
