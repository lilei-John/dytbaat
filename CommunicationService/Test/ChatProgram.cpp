#include <iostream>
#include "../CommunicationService.h"
#include "../DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"

using namespace std;

int main(){
    vector<unsigned char> outData;
    vector<unsigned char> inData;
    string data = "";

    stringstream clientStream(ios::in|ios::out|ios::app);

    StopAndWait clientDLL(clientStream);
    AcousticTL clientTL;

    CommunicationService client(clientDLL, clientTL);

    while (true) {
        cout << "Indtast din besked: " << endl;
        cin >> data;
        if(data == "") {
            unsigned char index0;
            while(clientStream >> index0){
                inData.push_back(index0);
                cout << index0;
            }
        } else {
            for (auto byte : data){
                clientStream << byte;
            }

            client.transmit();
        }
    }
    return 0;
}