#include <iostream>
#include "../CommunicationService/CommunicationService.h"
#include "../CommunicationService/DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../CommunicationService/TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../CommunicationService/Media/RealAudio/RealAudio.h"
#include "../CommunicationService/DataLinkLayer/SelectiveRepeat/SelectiveRepeat.h"

using namespace std;

int main(){
    int sampleRate = 4000;
    float toneTime = 15; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);

    AcousticTL clientTL(sampleRate, samplesPerTone);
    stringstream clientStream(ios::in|ios::out|ios::app);
    RealAudio clientRA(sampleRate);
    //SelectiveRepeat clientDLL(clientStream);
    // Uncomment to use StopAndWait instead
    StopAndWait clientDLL(clientStream);
    CommunicationService client(clientDLL, clientTL, clientRA);

    unsigned char end_delim = 3;
    string enterMessage = "Enter your message: ";
    string inData = "";
    client.setOnReceive([&](){
        unsigned char in;
        while (clientStream >> in){
            if (in == end_delim){
                cout << endl << "Received message: " << inData << endl << enterMessage;
                clientStream.str("");
                inData = "";
            } else{
                inData += in;
            }
        }
        clientStream.clear();
    });

    while(true) {
        string outData = "";
        cout << enterMessage;
        getline(cin, outData);
        if(outData == "q") {
            break;
        } else {
            for (auto byte : outData)
                clientStream << (unsigned char) byte;
            clientStream << end_delim;
            client.transmit();
        }
    }

    return 0;
}