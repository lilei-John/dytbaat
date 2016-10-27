#pragma once

#include "DataLinkLayer/DataLinkLayer.h"
#include "TransmissionLayer/TransmissionLayer.h"

class CommunicationService{
public:
    //App interface
    CommunicationService(DataLinkLayer, TransmissionLayer);

    void transmit();

private:
    DataLinkLayer dataLinkLayer;
    TransmissionLayer transmissionLayer;
};