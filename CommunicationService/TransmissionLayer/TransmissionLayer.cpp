#include "TransmissionLayer.h"

void TransmissionLayer::setOnFrameReceived(const std::function<void(std::vector<unsigned char>)> &cb) {
    onFrameReceived = cb;
}

void TransmissionLayer::setOnFrameTransmitted(const std::function<void()> &cb) {
    onFrameTransmitted = cb;
}
















