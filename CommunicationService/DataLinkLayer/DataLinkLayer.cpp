#include "../CommunicationService.h"
#include "StopAndWait/StopAndWait.h"

void DataLinkLayer::setOnFrameSendCallback(const std::function<bool(std::vector<unsigned char>)> &cb) {
    onFrameSendReq = cb;
}

void DataLinkLayer::setOnReceive(const std::function<void()> &onReceive) {
    DataLinkLayer::onReceive = onReceive;
}
