#include "Media.h"

void Media::setOnInReceived(const std::function<void(std::vector<float> &)> &cb) {
    onInReceive = cb;
}

void Media::setOnOutRequest(const std::function<void(std::vector<float> &)> &cb) {
    onOutRequest = cb;
}
