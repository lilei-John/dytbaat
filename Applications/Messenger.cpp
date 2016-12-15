#include <iostream>
#include "../CommunicationService/CommunicationService.h"
#include "../CommunicationService/DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../CommunicationService/TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../CommunicationService/Logger/Logger.h"
#include "../CommunicationService/Media/RealAudio/RealAudio.h"
#include "../CommunicationService/DataLinkLayer/SelectiveRepeat/SelectiveRepeat.h"

using namespace std;

int main(){
    Logger logger("FrameTravelTimeTest");

    int sampleRate = 44100;
    float toneTime = 30; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);

    AcousticTL clientTL(sampleRate, samplesPerTone);
    stringstream clientStream(ios::in|ios::out|ios::app);
    RealAudio clientRA(sampleRate);
    SelectiveRepeat clientDLL(clientStream);
    //StopAndWait* clientDLL = new StopAndWait(clientStream);
    CommunicationService client(clientDLL, clientTL, clientRA);

    /*clientDLL->setOnTimeout([&](){
        logger.log("TIMEOUT");
    });
    clientDLL->setOnCrcFail([&](){
        logger.log("SENDER CRC FAIL");
    });
    clientDLL->setOnFlowFail([&](){
        logger.log("SENDER FLOW FAIL");
    });
    clientDLL->setOnCrcFail([&](){
        logger.log("RECEIVER CRC FAIL");
    });
    clientDLL->setOnFlowFail([&](){
        logger.log("RECEIVER FLOW FAIL");
    });

    long millisec;
    clientDLL->setOnFrameSendTime([&](){
        chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch()
        );
        millisec = ms.count();
    });
    clientDLL->setOnAckReceiveTime([&](){
        chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch()
        );
        millisec = ms.count() - millisec;
        logger.log("Frame travel time: " + to_string(millisec));
    });*/

    unsigned char end_delim = 3;
    string enterMessage = "Enter your message: ";
    string data = "";
    client.setOnReceive([&](){
        unsigned char in;
        while (clientStream >> in){
            if (in == end_delim){
                cout << endl << "Received message: " << data << endl << enterMessage;
                clientStream.str("");
                data = "";
            } else{
                data += in;
            }
        }
        clientStream.clear();
    });

    while(true) {
        string data = "";
        cout << enterMessage;
        getline(cin, data);
        if(data == "q") {
            break;
        } else {
            for (auto byte : data)
                clientStream << (unsigned char) byte;
            clientStream << end_delim;
            client.transmit();
        }
    }

    return 0;
}