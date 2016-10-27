//
// Created by Rasmus Haugaard on 27/10/2016.
//

#include <iostream>
#include "../CommunicationService.h"
#include "../DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"

using namespace std;


int main(){

    vector<unsigned char> outData;
    vector<unsigned char> inData;
    unsigned char newByte = 0b00100001;
    for(int i = 0; i < 256; i++) {
        outData.push_back(newByte);
        if (newByte == 0b01111110){
            newByte = 0b00100000;
        }
        newByte++;
    }

    stringstream outStream(ios::in|ios::out|ios::app);
    stringstream inStream(ios::in|ios::out|ios::app);

    CommunicationService sender((StopAndWait(outStream)), AcousticTL());
    //CommunicationService receiver((StopAndWait(inStream)), AcousticTL());

    for (auto byte : outData){
        outStream << byte;
    }

    //sender.transmit();

    unsigned char index0;
    while(inStream >> index0){
        cout << "I0: " << index0 << endl;
        inData.push_back(index0);
    }

    cout << boolalpha;
    cout << "Test succeeded: " << (inData == outData) << endl;

    return 0;

    /*string receivedMessage;
    receiver.setOnFrameReceiveCallback([&](vector<unsigned char> receivedFrame){
        receivedMessage = "";
        for (auto c : receivedFrame){
            receivedMessage += c;
        }
        cout << "Received: " << receivedMessage << endl;
    });

    string message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."; // 56 Bytes
    vector<unsigned char> sendFrame;
    for (auto c : message){
        sendFrame.push_back((unsigned char) c);
    }

    Pa_Sleep(200);
    sender.sendFrame(sendFrame);
    Pa_Sleep(((sendFrame.size() + 3) * 2 * samplesPerTone * 1000) / sampleRate + 1000);
    cout << (receivedMessage == message ? "Test succeeded!" : "Test failed!") << endl;
    return 0;*/
}