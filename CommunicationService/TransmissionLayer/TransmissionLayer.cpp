#include "TransmissionLayer.h"

void TransmissionLayer::setOnFrameReceived(const std::function<void(std::vector<unsigned char>)> &cb) {
    onFrameReceived = cb;
}


















