#pragma once

#include <array>
#include <vector>
#include <queue>
#include <functional>

class TransmissionLayer{
public:
    void setOnFrameReceiveCallback(const std::function<void(std::vector<unsigned char>)> &cb);
    void setOnFrameTransmittedCallback(const std::function<void(void)> &cb);
    virtual bool sendFrame(std::vector<unsigned char>) = 0;
protected:
    std::function<void(std::vector<unsigned char>)> onFrameReceiveCallback;
    std::function<void(void)> onFrameTransmittedCallback;
};