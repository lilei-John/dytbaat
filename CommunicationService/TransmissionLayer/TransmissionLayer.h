#pragma once

#include <array>
#include <vector>
#include <queue>
#include <functional>

class TransmissionLayer{
public:
    void setOnFrameReceived(const std::function<void(std::vector<unsigned char>)> &cb);
    virtual bool sendFrame(const std::vector<unsigned char> &) = 0;
    virtual void processInput(const std::vector<float> &) = 0;
    virtual void setOutput(std::vector<float> &) = 0;
protected:
    std::function<void(std::vector<unsigned char>)> onFrameReceived;
};