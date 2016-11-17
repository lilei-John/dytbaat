#include <iostream>
#include "../CommunicationService.h"
#include "../DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../Media/IdealAudio/IdealAudio.h"

using namespace std;

int main(){
    int sampleRate = 44100;
    float toneTime = 10; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);

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

    string outMessage = "abc";
    for (char byte : outMessage)
        ssOut << byte;
    sender.transmit();

    atomic<bool> shouldStop(false);

    receiver.setOnReceive([&](){
        cout << ssIn.str() << endl;
        shouldStop.store(true);
    });

    while (!shouldStop.load())
        this_thread::yield();

    return 0;
}