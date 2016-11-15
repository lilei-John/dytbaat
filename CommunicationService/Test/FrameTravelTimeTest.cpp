#include <iostream>
#include "../CommunicationService.h"
#include "../DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../Logger/Logger.h"
#include "../Media/RealAudio/RealAudio.h"

using namespace std;

int main(){
    Logger logger("FrameTravelTimeTest");
    vector<unsigned char> outData;
    vector<unsigned char> inData;
    string data = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet .";

    for(int i = 0; i < data.size(); i++) {
        outData.push_back((unsigned char)data[i]);
    }

    stringstream outStream(ios::in|ios::out|ios::app);
    stringstream inStream(ios::in|ios::out|ios::app);

    for (auto byte : outData){
        outStream << byte;
    }

    int sampleRate = 44100;
    int samplesPerTone = 1600;
    int samplesPerSearch = 500;

    StopAndWait outDLL(outStream);
    StopAndWait inDLL(inStream);
    AcousticTL outTL(sampleRate, samplesPerTone, samplesPerSearch);
    AcousticTL inTL(sampleRate, samplesPerTone, samplesPerSearch);
    RealAudio outRA(sampleRate);
    RealAudio inRA(sampleRate);

    CommunicationService sender(outDLL, outTL, outRA);
    CommunicationService receiver(inDLL, inTL, inRA);

    outDLL.setOnTimeout([&](){
       logger.log("TIMEOUT");
    });
    outDLL.setOnCrcFail([&](){
        logger.log("SENDER CRC FAIL");
    });
    outDLL.setOnFlowFail([&](){
        logger.log("SENDER FLOW FAIL");
    });
    inDLL.setOnCrcFail([&](){
        logger.log("RECEIVER CRC FAIL");
    });
    inDLL.setOnFlowFail([&](){
        logger.log("RECEIVER FLOW FAIL");
    });

    long millisec;
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
        logger.log("Frame travel time: " + to_string(millisec));
    });

    sender.transmit();

    cout << "Press enter when the sounds stop for more than 5 seconds..." << endl;
    cin.get();

    unsigned char index0;
    while(inStream >> index0){
        inData.push_back(index0);
    }

    cout << boolalpha << endl;
    cout << "Test succeeded: " << (inData == outData) << endl;
    string result(inData.begin(), inData.end());
    cout << "Received text: " << result << endl;

    return 0;
}