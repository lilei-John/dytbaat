#include "TransmissionLayer.h"

void TransmissionLayer::setOnFrameReceiveCallback(const std::function<void(std::vector<unsigned char>)> &cb) {
    onFrameReceiveCallback = cb;
}

void TransmissionLayer::setOnFrameTransmittedCallback(const std::function<void(void)> &cb) {
    onFrameTransmittedCallback = cb;

}


















