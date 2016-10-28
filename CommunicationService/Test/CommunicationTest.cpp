#include <iostream>
#include "../CommunicationService.h"
#include "../DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"

using namespace std;

int main(){
    vector<unsigned char> outData;
    vector<unsigned char> inData;
    string data = "This is an test with many white spaces so we can test it, pretty long packet right? :)";

    for(int i = 0; i < data.size(); i++) {
        outData.push_back((unsigned char)data[i]);
    }

    stringstream outStream(ios::in|ios::out|ios::app);
    stringstream inStream(ios::in|ios::out|ios::app);

    StopAndWait outDLL(outStream);
    StopAndWait inDLL(inStream);
    AcousticTL outTL;
    AcousticTL inTL;

    //CommunicationService sender(outDLL, outTL);
    CommunicationService receiver(inDLL, inTL);

    for (auto byte : outData){
        outStream << byte;
    }

    //sender.transmit();

    cout << "Indtast noget i terminalen, når lydene stopper." << endl;
    cin.get();

    unsigned char index0;
    while(inStream >> index0){
        inData.push_back(index0);
        cout << index0;
    }

    cout << boolalpha << endl;
    cout << "Test succeeded: " << (inData == outData) << endl;

    return 0;
}