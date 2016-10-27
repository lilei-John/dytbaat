#include "CommunicationService.h"
#include "DataLinkLayer/DataLinkLayer.h"
#include "TransmissionLayer/TransmissionLayer.h"
#include <vector>

CommunicationService::CommunicationService(DataLinkLayer dl, TransmissionLayer tl) : dataLinkLayer(dl), transmissionLayer(tl){
    // dataLinkLayer = dl;
    // transmissionlayer = tl;
    transmissionLayer.setOnFrameReceiveCallback(
            [&](std::vector<unsigned char> frame){
                dataLinkLayer.receiveFrame(frame);
            }
    );

    dataLinkLayer.setOnFrameSendCallback(
            [&](std::vector<unsigned char> frame) -> bool{
                return transmissionLayer.sendFrame(frame);
            }
    );
}

void CommunicationService::transmit() {
    dataLinkLayer.transmit();
}

