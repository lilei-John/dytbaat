#pragma once

#include <vector>
#include <functional>
#include <chrono>

class DataLinkLayer{
public:
    void setOnFrameSendCallback(const std::function<bool(std::vector<unsigned char>)> &cb);
    void setOnReceive(const std::function<void()> &onReceive);

    void setReqMaxTransmissionDuration(
            const std::function<std::chrono::milliseconds(unsigned int, int)> &reqMaxTransmissionDuration);

    virtual void receiveFrame(std::vector<unsigned char>) = 0;
    virtual void transmit() = 0;
    virtual void frameTransmitted() = 0;
    virtual int getMaxFrameSize() = 0;
protected:
    std::function<bool(std::vector<unsigned char>)> onFrameSendReq;
    std::function<std::chrono::milliseconds(unsigned int frameSize, int maxEscapableBytes)> reqMaxTransmissionDuration;
    std::function<void()> onReceive;
};