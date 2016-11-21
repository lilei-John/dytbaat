#include <iostream>
#include "../CommunicationService.h"
#include "../DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../Media/IdealAudio/IdealAudio.h"

using namespace std;

int main(){
    int sampleRate = 44100;
    float toneTime = 7; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);
    cout << "samplesPerTone: " << samplesPerTone << endl;

    stringstream ssOut(ios::in|ios::out|ios::app);
    stringstream ssIn(ios::in|ios::out|ios::app);

    StopAndWait outDLL(ssOut);
    StopAndWait inDLL(ssIn);
    AcousticTL outTL(sampleRate, samplesPerTone);
    AcousticTL inTL(sampleRate, samplesPerTone);
    IdealAudio outIA;
    IdealAudio inIA;

    IdealAudio::connect(inIA, outIA, (unsigned int)sampleRate, 2000);

    CommunicationService sender(outDLL, outTL, outIA);
    CommunicationService receiver(inDLL, inTL, inIA);

    string outMes = "abc";
    for (char byte : outMes) ssOut << byte;

    atomic<bool> shouldStop(false);
    receiver.setOnReceive([&](){
        string inMes = ssIn.str();
        cout << "Received: '" << inMes << "'" << endl;
        cout << (inMes == outMes ? "Success!" : "Fail!") << endl;
        shouldStop.store(true);
    });

    cout << "Sending: '" << outMes << "'" << endl;
    sender.transmit();

    while (!shouldStop.load())
        this_thread::yield();

    return 0;
}