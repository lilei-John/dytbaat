#pragma once

#include <vector>
#include <functional>

class DataLinkLayer{
public:
    void setOnFrameSendCallback(const std::function<bool(std::vector<unsigned char>)> &cb);
    void setOnReceive(const std::function<void()> &onReceive);
    virtual void receiveFrame(std::vector<unsigned char>) = 0;
    virtual void transmit() = 0;
    virtual int getMaxFrameSize() = 0;
protected:
    std::function<bool(std::vector<unsigned char>)> onFrameSendReq;
    std::function<void()> onReceive;
};