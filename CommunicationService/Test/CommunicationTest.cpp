#include <iostream>
#include "../CommunicationService.h"
#include "../DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"
#include <ctime>
#include <iomanip>
using namespace std;

void printVector(vector<unsigned char> b){
    cout << "|";
    for (int i = 0; i < b.size(); ++i) {
        cout << b.at(i);
    }
    cout << "|" << endl;

}

int main(){
    vector<unsigned char> outData;
    vector<unsigned char> inData;
    string data = "IT WORKS! Oh happy days! IT WOOOOOOOOORRRKSSS";

    for(int i = 0; i < data.size(); i++) {
        outData.push_back((unsigned char)data[i]);
    }

    stringstream outStream(ios::in|ios::out|ios::app);
    stringstream inStream(ios::in|ios::out|ios::app);

    StopAndWait outDLL(outStream);
    StopAndWait inDLL(inStream);
    AcousticTL outTL;
    AcousticTL inTL;

    outDLL.setOnTimeout([](){
       cout << "TIMEOUT" << endl;
    });
    outDLL.setOnCrcFail([](){
        cout << "SENDER CRC FAIL" << endl;
    });
    outDLL.setOnFlowFail([](){
        cout << "SENDER FLOW FAIL" << endl;
    });
    inDLL.setOnCrcFail([](){
        cout << "RECEIVER CRC FAIL" << endl;
    });
    inDLL.setOnFlowFail([](){
        cout << "RECEIVER FLOW FAIL" << endl;
    });
    long millisec = 0;
    outDLL.setOnFrameSendTime([&](){
        chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch()
        );
        millisec = ms.count();
    });
    outDLL.setOnAckReceiveTime([&](){
        chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
         chrono::system_clock::now().time_since_epoch()
    );
        millisec = ms.count() - millisec;
        cout << "Frame travel time: " << millisec<< endl;
    });

    CommunicationService sender(outDLL, outTL);
    CommunicationService receiver(inDLL, inTL);

    for (auto byte : outData){
        outStream << byte;
    }

    sender.transmit();

    cout << "Indtast noget i terminalen, når lydene stopper." << endl;
    cin.get();

    unsigned char index0;
    while(inStream >> index0){
        inData.push_back(index0);
    }
    printVector(outData);
    printVector(inData);



    cout << boolalpha << endl;
    cout << "Test succeeded: " << (inData == outData) << endl;

    return 0;
}