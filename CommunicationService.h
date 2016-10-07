#pragma once

#include "DataLinkLayer.h"
#include "TransmissionLayer.h"

class CommunicationService{
public:
    //App interface
    CommunicationService(DataLinkLayer dll, TransmissionLayer tml);

private:
    DataLinkLayer dataLinkLayer;
    TransmissionLayer transmissionLayer;
};