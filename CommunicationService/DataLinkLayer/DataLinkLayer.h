#pragma once

#include <vector>
#include <functional>

class DataLinkLayer{
public:
    void setOnFrameSendCallback(const std::function<bool(std::vector<unsigned char>)> &cb);
    virtual void receiveFrame(std::vector<unsigned char>) = 0;
    virtual void transmit() = 0;
    virtual void frameTransmitted() = 0;
protected:
    std::function<bool(std::vector<unsigned char>)> onFrameSendCallback;
};