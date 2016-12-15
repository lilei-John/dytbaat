#include <iostream>
#include "../CommunicationService/CommunicationService.h"
#include "../CommunicationService/DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../CommunicationService/TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../CommunicationService/Logger/Logger.h"
#include "../CommunicationService/Media/RealAudio/RealAudio.h"
#include "../CommunicationService/DataLinkLayer/SelectiveRepeat/SelectiveRepeat.h"

using namespace std;

int main(){
    int sampleRate = 4000;
    float toneTime = 15; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);

    cout << "Samples per tone: " << samplesPerTone << endl;

    AcousticTL* clientTL = new AcousticTL(sampleRate, samplesPerTone);
    stringstream clientStream(ios::in|ios::out|ios::app);
    RealAudio* clientRA = new RealAudio(sampleRate);
    SelectiveRepeat* clientDLL = new SelectiveRepeat(clientStream);
    // Uncomment to use StopAndWait instead
    //StopAndWait* clientDLL = new StopAndWait(clientStream);
    CommunicationService* client = new CommunicationService(*clientDLL, *clientTL, *clientRA);

    unsigned char end_delim = 3;
    string enterMessage = "Enter your message: ";
    string data = "";
    client->setOnReceive([&](){
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
            client->transmit();
        }
    }

    return 0;
}