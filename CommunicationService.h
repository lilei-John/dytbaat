#pragma once

#define FRAMES_PER_BUFFER (500)
#define SAMPLE_RATE (44100)
#define SAMPLES_PER_TONE (500)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#include "DataLinkLayer.h"
#include "TransmissionLayer.h"

class CommunicationService{
public:
    //App interface
    CommunicationService(DataLinkLayer, TransmissionLayer);

private:
    DataLinkLayer dataLinkLayer;
    TransmissionLayer transmissionLayer;
};