#include "CommunicationService.h"

using namespace std;

CommunicationService::CommunicationService(
        DataLinkLayer &dl,
        TransmissionLayer &tl,
        Media &m) :
        dataLinkLayer(dl),
        transmissionLayer(tl),
        media(m){
    dataLinkLayer.setOnReceive(
            [&](){
                if (onReceive) onReceive();
            }
    );
    transmissionLayer.setOnFrameTransmitted(
            [&](void){
                dataLinkLayer.frameTransmitted();
            }
    );
    dataLinkLayer.setOnFrameSendCallback(
            [&](vector<unsigned char> frame) -> bool{
                return transmissionLayer.sendFrame(frame);
            }
    );
    transmissionLayer.setOnFrameReceived(
            [&](std::vector<unsigned char> frame) {
                dataLinkLayer.receiveFrame(frame);
            }
    );
    transmissionLayer.setMaxFrameSize(
            dataLinkLayer.getMaxFrameSize()
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

void CommunicationService::setOnReceive(const std::function<void()> &onReceive) {
    CommunicationService::onReceive = onReceive;
}

void CommunicationService::disable() {
    media.setOnOutRequest(nullptr);
    media.setOnInReceived(nullptr);
    transmissionLayer.setOnFrameReceived([](std::vector<unsigned char>){});
    dataLinkLayer.setOnFrameSendCallback([](std::vector<unsigned char>){return false;});
    dataLinkLayer.setOnReceive([](){});
}

CommunicationService::~CommunicationService() {
    disable();
}

