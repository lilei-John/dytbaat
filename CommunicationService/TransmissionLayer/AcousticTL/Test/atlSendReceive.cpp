#include <iostream>
#include "../AcousticTL.h"

using namespace std;

const int sampleRate = 44100;
const int samplesPerTone = 1600;
const int samplesPerSearch = 500;

int main(){
    AcousticTL sender(sampleRate, samplesPerTone, samplesPerSearch);
    AcousticTL receiver(sampleRate, samplesPerTone, samplesPerSearch);

    string receivedMessage;
    receiver.setOnFrameReceived([&](vector<unsigned char> receivedFrame) {
        receivedMessage = "";
        for (auto c : receivedFrame) {
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
    return 0;
}