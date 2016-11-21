#pragma once

#include <array>
#include <vector>
#include <queue>
#include <functional>

class TransmissionLayer{
public:
    virtual bool sendFrame(const std::vector<unsigned char> &) = 0;
    virtual void processInput(const std::vector<float> &) = 0;
    virtual void setOutput(std::vector<float> &) = 0;
    virtual void setMaxFrameSize(int) = 0;

    void setOnFrameReceived(const std::function<void(std::vector<unsigned char>)> &cb);
    void setOnFrameTransmitted(const std::function<void()> &cb);
protected:
    std::function<void(std::vector<unsigned char>)> onFrameReceived;
    std::function<void(void)> onFrameTransmitted;
};