#include "CommunicationService.h"

CommunicationService::CommunicationService(DataLinkLayer &dl, TransmissionLayer &tl, Media &m) : dataLinkLayer(dl), transmissionLayer(tl), media(m){
    dataLinkLayer.setOnFrameSendCallback(
            [&](std::vector<unsigned char> frame) -> bool{
                return transmissionLayer.sendFrame(frame);
            }
    );
    transmissionLayer.setOnFrameReceived(
            [&](std::vector<unsigned char> frame) {
                dataLinkLayer.receiveFrame(frame);
            }
    );
    media.setOnInReceived(
            [&](std::vector<float> &in) {
                transmissionLayer.processInput(in);
            }
    );
    media.setOnOutRequest(
            [&](std::vector<float> &out){
                transmissionLayer.setOutput(out);
            }
    );
}

void CommunicationService::transmit() {
    dataLinkLayer.transmit();
}

