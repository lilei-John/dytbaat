#pragma once

#include <vector>
#include <functional>

class DataLinkLayer{
public:
    void setOnFrameSendCallback(const std::function<bool(std::vector<unsigned char>)> &cb);
    virtual void receiveFrame(std::vector<unsigned char>){};
    virtual void transmit(){};
protected:
    std::function<bool(std::vector<unsigned char>)> onFrameSendCallback;
};