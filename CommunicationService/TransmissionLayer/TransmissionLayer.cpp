#include "TransmissionLayer.h"

void TransmissionLayer::setOnFrameReceiveCallback(const std::function<void(std::vector<unsigned char>)> &cb) {
    onFrameReceiveCallback = cb;
}


















