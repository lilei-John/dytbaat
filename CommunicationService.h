#pragma once

#include "DataLinkLayer.h"
#include "TransmissionLayer/TransmissionLayer.h"

class CommunicationService{
public:
    //App interface
    CommunicationService(DataLinkLayer, TransmissionLayer);

private:
    DataLinkLayer dataLinkLayer;
    TransmissionLayer transmissionLayer;
};