/* #include <iostream>
#include "../CommunicationService.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../Logger/Logger.h"
#include "../DataLinkLayer/SelectiveRepeat/SelectiveRepeat.h"
#include "../Media/RealAudio/RealAudio.h"

using namespace std;

int main(){
    Logger logger("FrameTravelTimeTest");
    vector<unsigned char> outData;
    vector<unsigned char> inData;
    string data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    for(int i = 0; i < data.size(); i++) {
        outData.push_back((unsigned char)data[i]);
    }

    stringstream outStream(ios::in|ios::out|ios::app);
    stringstream inStream(ios::in|ios::out|ios::app);

    for (auto byte : outData){
        outStream << byte;
    }

    int sampleRate = 44100;
    float toneTime = 30; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);

    cout << "Samples per tone: " << samplesPerTone << endl;

    SelectiveRepeat outDLL(outStream);
    SelectiveRepeat inDLL(inStream);
    AcousticTL outTL(sampleRate, samplesPerTone);
    AcousticTL inTL(sampleRate, samplesPerTone);
    RealAudio outRA(sampleRate);
    RealAudio inRA(sampleRate);

    CommunicationService sender(outDLL, outTL, outRA);
    CommunicationService receiver(inDLL, inTL, inRA);

    inTL.setOnFrameReceived([](vector<unsigned char> frame){
        cout << "received frame" << endl;
    });
*/
/*    outDLL.setOnTimeout([&](){
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
        logger.log("Frame travel time: " + to_string(millisec));
    });
*/


/*
 //   std::this_thread::sleep_for(std::chrono::seconds(2));
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
}*/

#include <iostream>
#include "../CommunicationService.h"
#include "../DataLinkLayer/SelectiveRepeat/SelectiveRepeat.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../Logger/Logger.h"
#include "../Media/RealAudio/RealAudio.h"

using namespace std;

int main(){
    Logger logger("SelectiveRepeatTest");
    vector<unsigned char> outData;
    vector<unsigned char> inData;
    //string data = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet.";
    //string data = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet . Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet . Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet . Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet . Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet .";
    string data = "Text messaging, or texting, is the act of composing and sending electronic messages, typically consisting of alphabetic and numeric characters, between two or more users of mobile phones, fixed devices (e.g., desktop computers) or portable devices (e.g., tablet computers or smartphones). While text messages are usually sent over a phone network, due to the convergence between the telecommunication and broadcasting industries in the 2000s, text messages may also be sent via a cable network or Local Area Network. The term originally referred to messages sent using the Short Message Service (SMS). It has grown beyond alphanumeric text to include multimedia messages (known as MMS) containing digital images, videos, and sound content, as well as ideograms known as emoji (happy faces and other icons).";
    for(int i = 0; i < data.size(); i++)
        outData.push_back((unsigned char)data[i]);
    cout << "Choose a Logger ID, the ID is supposed to be the same on the transmitter and receiver side: " << endl;
    string response;
    getline(cin, response);
    logger.log("ID: " + response);
    cout << "(r)eceiver, (t)ransmitter or (b)oth ?" << endl;
    getline(cin, response);
    bool isReceiver = response == "r" || response == "b";
    bool isTransmitter = response == "t" || response == "b";
    if (!isReceiver && !isTransmitter){
        cout << "Command not understood!" << endl;
        return 0;
    }

    int sampleRate = 44100;
    float toneTime = 30; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);

    logger.log("Sample Rate: " + to_string(sampleRate));
    logger.log("Tone Time: " + to_string(toneTime));

    cout << "Samples per tone: " << samplesPerTone << endl;

    stringstream outStream(ios::in|ios::out|ios::app);
    stringstream inStream(ios::in|ios::out|ios::app);

    SelectiveRepeat outDLL(outStream);
    SelectiveRepeat inDLL(inStream);
    AcousticTL outTL(sampleRate, samplesPerTone);
    AcousticTL inTL(sampleRate, samplesPerTone);
    RealAudio outRA(sampleRate);
    RealAudio inRA(sampleRate);

    CommunicationService sender(outDLL, outTL, outRA);
    CommunicationService receiver(inDLL, inTL, inRA);

    logger.log("Frame Size: " + to_string(inDLL.getMaxFrameSize()-3));
    logger.log("Frame Block Size: " + to_string(inDLL.getFrameBlockSize()));
    logger.log("Total Sequence Numbers: " + to_string(inDLL.getTotalSeqNo()));
    logger.log("Data [" + to_string(data.length()) + " bytes]: " + data );
    logger.log("-----------------------");

    if (isReceiver){
        inDLL.setOnCrcFail([&](vector<unsigned char> frame){
            string sFrame = "";
            for(auto c : frame) {
                bitset<8> my_bset=bitset<8>(c);
                sFrame += my_bset.to_string() + " ";
            }
            logger.log("RECEIVER CRC FAIL\n" + sFrame);
        });
        inDLL.setOnFrameReceive([&](int seqNo){
            logger.log("FRAME RECEIVED  |  " + to_string(seqNo));
        });
        inTL.getFrameReceiver().setOnFrameError([&](vector<unsigned char> frame, unsigned char byte){
            cout << "Receiver frame size exceeded."
                 << " Last byte: " << bitset<8>(byte)
                 << " should be stop byte: "
                 << bitset<8>(inTL.getFrameProtocol().getStopByte())
                 << " if it's a full frame."
                 << endl;
        });
    }else{
        receiver.disable();
    }

    long millisec;
    if (isTransmitter){
        for (auto byte : outData)
            outStream << byte;
         outDLL.setOnTimeout([&](){
            logger.log("TIMEOUT");
        });
        outDLL.setOnCrcFail([&](vector<unsigned char> frame){
            string sFrame = "";
            for(auto c : frame) {
                bitset<8> my_bset=bitset<8>(c);
                sFrame += my_bset.to_string() + " ";
            }
            logger.log("SENDER CRC FAIL\n" + sFrame);
        });
        outDLL.setOnFrameSendTime([&](vector<unsigned char> frame){
            string sFrame = "";
            int seqNo = (frame[0]&~(1<<7));
            for(auto c : frame) {
                bitset<8> my_bset=bitset<8>(c);
                sFrame += my_bset.to_string() + " ";
            }
            logger.log("Frame Sent  |  " + to_string(seqNo) + "\n" + sFrame);
        });
        outDLL.setOnAckReceiveTime([&](std::vector<unsigned char> frame){
            string sFrame = "";
            for(int i = 0; i < frame.size()-2; i++) {
                sFrame += to_string((int)frame[i]) + " ";
            }
            logger.log("NAK Received  |  " + sFrame);
            cout << "NAK Received" << endl;
        });
        logger.startTimer();
        sender.transmit();
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