#pragma once

#include <vector>
#include <functional>

class Media {
public:
    void setOnInReceived(const std::function<void(std::vector<float> &)> &onInReceive);
    void setOnOutRequest(const std::function<void(std::vector<float> &)> &onOutRequest);
protected:
    std::function<void(std::vector<float> &)> onInReceive;
    std::function<void(std::vector<float> &)> onOutRequest;
};