#include "CommunicationService.h"

CommunicationService::CommunicationService(DataLinkLayer &dl, TransmissionLayer &tl) : dataLinkLayer(dl), transmissionLayer(tl){
    transmissionLayer.setOnFrameReceiveCallback(
            [&](std::vector<unsigned char> frame){
                dataLinkLayer.receiveFrame(frame);
            }
    );
    transmissionLayer.setOnFrameTransmittedCallback(
            [&](void){
                dataLinkLayer.frameTransmitted();
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

