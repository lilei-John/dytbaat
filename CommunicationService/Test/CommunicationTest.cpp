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

    StopAndWait outDLL(outStream);
    StopAndWait inDLL(inStream);
    AcousticTL outTL;
    AcousticTL inTL;

    CommunicationService sender(outDLL, outTL);
    CommunicationService receiver(inDLL, inTL);

    for (auto byte : outData){
        outStream << byte;
    }

    sender.transmit();

    Pa_Sleep(3000);

    unsigned char index0;
    while(inStream >> index0){
        cout << "I0: " << index0 << endl;
        inData.push_back(index0);
    }

    cout << boolalpha;
    cout << "Test succeeded: " << (inData == outData) << endl;

    return 0;
}