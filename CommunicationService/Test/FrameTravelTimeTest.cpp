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
    string data = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet . Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet . Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet . Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet . Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet .";
    for(int i = 0; i < data.size(); i++)
        outData.push_back((unsigned char)data[i]);

    cout << "(r)eceiver, (t)ransmitter or (b)oth ?" << endl;
    string response;
    getline(cin, response);
    bool isReceiver = response == "r" || response == "b";
    bool isTransmitter = response == "t" || response == "b";
    if (!isReceiver && !isTransmitter){
        cout << "Didn't understand command." << endl;
        return 0;
    }

    int sampleRate = 4000;
    float toneTime = 15; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);

    cout << "Samples per tone: " << samplesPerTone << endl;

    stringstream outStream(ios::in|ios::out|ios::app);
    stringstream inStream(ios::in|ios::out|ios::app);

    StopAndWait outDLL(outStream);
    StopAndWait inDLL(inStream);
    AcousticTL outTL(sampleRate, samplesPerTone);
    AcousticTL inTL(sampleRate, samplesPerTone);
    RealAudio outRA(sampleRate);
    RealAudio inRA(sampleRate);

    CommunicationService sender(outDLL, outTL, outRA);
    CommunicationService receiver(inDLL, inTL, inRA);

    if (isReceiver){
        inTL.getSync().setOnSyncFail([](float p){
            cout << "Receiver sync fail. Match percentage: " << p << endl;
        });
        inTL.getSync().setOnSyncSuccess([](float c){
            cout << "Receiver sync success. Certainty: " << c << endl;
        });
        inTL.getFrameReceiver().setOnFrameError([&](vector<unsigned char> frame, unsigned char byte){
            cout << "Receiver frame size exceeded."
                 << " Last byte: " << bitset<8>(byte)
                 << " should be stop byte: "
                 << bitset<8>(inTL.getFrameProtocol().getStopByte())
                 << " if it's a full frame."
                 << endl;
        });
        inDLL.setOnCrcFail([&](){
            logger.log("RECEIVER CRC FAIL");
        });
        inDLL.setOnFlowFail([&](){
            logger.log("RECEIVER FLOW FAIL");
        });
    }else{
        receiver.disable();
    }

    long millisec;
    if (isTransmitter){
        for (auto byte : outData)
            outStream << byte;
        outDLL.setOnTimeout([&](){
            logger.log("SENDER TIMEOUT");
        });
        outDLL.setOnCrcFail([&](){
            logger.log("SENDER CRC FAIL");
        });
        outDLL.setOnFlowFail([&](){
            logger.log("SENDER FLOW FAIL");
        });
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
        logger.startTimer();
    }else{
        sender.disable();
    }

    cout << "Press enter to stop.." << endl;
    cin.get();

    if (isReceiver){
        unsigned char index0;
        while(inStream >> index0)
            inData.push_back(index0);
        cout << boolalpha << endl;
        cout << "Test succeeded: " << (inData == outData) << endl;
        string result(inData.begin(), inData.end());
        cout << "Received text: " << result << endl;
    }

    return 0;
}