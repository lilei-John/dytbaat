#pragma once

#include <vector>
#include <functional>

class DataLinkLayer{
public:
    DataLinkLayer();
    void setOnFrameSendCallback(const std::function<bool(std::vector<unsigned char>)> &onFrameSendCallback);
    virtual void receiveFrame(std::vector<unsigned char>){};
protected:
    std::function<bool(std::vector<unsigned char>)> onFrameSendCallback;
};