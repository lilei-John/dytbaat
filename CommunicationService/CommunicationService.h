#pragma once

#include "DataLinkLayer/DataLinkLayer.h"
#include "TransmissionLayer/TransmissionLayer.h"
#include "Media/Media.h"

class CommunicationService{
public:
    CommunicationService(DataLinkLayer&, TransmissionLayer&, Media&);
    void transmit();
    void setOnReceive(const std::function<void()> &onReceive);
    void disable();
    ~CommunicationService();
private:
    DataLinkLayer &dataLinkLayer;
    TransmissionLayer &transmissionLayer;
    Media &media;
    std::function<void()> onReceive;
};