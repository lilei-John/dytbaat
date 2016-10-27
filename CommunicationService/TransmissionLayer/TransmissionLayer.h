#pragma once

#include <array>
#include <vector>
#include <queue>
#include <functional>

class TransmissionLayer{
public:
    void setOnFrameReceiveCallback(const std::function<void(std::vector<unsigned char>)> &cb);
    virtual bool sendFrame(std::vector<unsigned char>) = 0;
protected:
    std::function<void(std::vector<unsigned char>)> onFrameReceiveCallback;
};