#pragma once

#include "DataLinkLayer/DataLinkLayer.h"
#include "TransmissionLayer/TransmissionLayer.h"
#include "Media/Media.h"

class CommunicationService{
public:
    CommunicationService(DataLinkLayer&, TransmissionLayer&, Media&);
    void transmit();
private:
    DataLinkLayer &dataLinkLayer;
    TransmissionLayer &transmissionLayer;
    Media &media;
};