#include "../CommunicationService.h"
#include "StopAndWait/StopAndWait.h"

void DataLinkLayer::setOnFrameSendCallback(const std::function<bool(std::vector<unsigned char>)> &cb) {
    onFrameSendCallback = cb;
}
