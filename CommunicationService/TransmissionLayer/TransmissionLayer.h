#pragma once

#include <array>
#include <vector>
#include <queue>

class TransmissionLayer{
public:
    void setOnFrameReceiveCallback(const std::function<void(std::vector<unsigned char>)> &cb);
    virtual bool sendFrame(std::vector<unsigned char>){return false;};
protected:
    std::function<void(std::vector<unsigned char>)> onFrameReceiveCallback;
};