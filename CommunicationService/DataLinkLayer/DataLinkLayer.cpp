#include "../CommunicationService.h"

DataLinkLayer::DataLinkLayer() {

}

void DataLinkLayer::setOnFrameSendCallback(const std::function<bool(std::vector<unsigned char>)> &cb) {
    onFrameSendCallback = cb;
}
