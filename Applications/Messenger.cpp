#include <iostream>
#include "../CommunicationService/CommunicationService.h"
#include "../CommunicationService/DataLinkLayer/StopAndWait/StopAndWait.h"
#include "../CommunicationService/TransmissionLayer/AcousticTL/AcousticTL.h"
#include "../CommunicationService/Logger/Logger.h"
#include "../CommunicationService/Media/RealAudio/RealAudio.h"

using namespace std;

int main(){
    Logger logger("FrameTravelTimeTest");
    vector<unsigned char> outData;
    vector<unsigned char> inData;
    //string data = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Typi non habent claritatem insitam; est usus legentis in iis qui facit eorum claritatem. Investigationes demonstraverunt lectores legere me lius quod ii legunt saepius. Claritas est etiam processus dynamicus, qui sequitur mutationem consuetudium lectorum. Mirum est notare quam littera gothica, quam nunc putamus parum claram, anteposuerit litterarum formas humanitatis per seacula quarta decima et quinta decima. Eodem modo typi, qui nunc nobis videntur parum clari, fiant sollemnes in futurum.";
    string data = "";
    for(int i = 0; i < data.size(); i++) {
        outData.push_back((unsigned char)data[i]);
    }

    int sampleRate = 96000;
    float toneTime = 20; //ms
    int samplesPerTone = (int)((float)sampleRate / 1000 * toneTime);

    cout << "Samples per tone: " << samplesPerTone << endl;

    /*
    stringstream testStream(ios::in|ios::out|ios::app);
    StopAndWait* testDLL = new StopAndWait(testStream);
    AcousticTL* testTL = new AcousticTL(sampleRate, samplesPerTone);
    RealAudio* testRA = new RealAudio(sampleRate);
    CommunicationService* test = new CommunicationService(*testDLL, *testTL, *testRA);
    */
    bool open = true;

    AcousticTL* clientTL = new AcousticTL(sampleRate, samplesPerTone);
    stringstream clientStream(ios::in|ios::out|ios::app);
    RealAudio* clientRA = new RealAudio(sampleRate);
    StopAndWait* clientDLL = new StopAndWait(clientStream);
    CommunicationService* client = new CommunicationService(*clientDLL, *clientTL, *clientRA);

    clientDLL->setOnTimeout([&](){
        logger.log("TIMEOUT");
    });
    clientDLL->setOnCrcFail([&](){
        logger.log("SENDER CRC FAIL");
    });
    clientDLL->setOnFlowFail([&](){
        logger.log("SENDER FLOW FAIL");
    });
    clientDLL->setOnCrcFail([&](){
        logger.log("RECEIVER CRC FAIL");
    });
    clientDLL->setOnFlowFail([&](){
        logger.log("RECEIVER FLOW FAIL");
    });

    long millisec;
    clientDLL->setOnFrameSendTime([&](){
        chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch()
        );
        millisec = ms.count();
    });
    clientDLL->setOnAckReceiveTime([&](){
        chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
                chrono::system_clock::now().time_since_epoch()
        );
        millisec = ms.count() - millisec;
        logger.log("Frame travel time: " + to_string(millisec));
    });


    while(open) {
        data = "";
        cout << "Enter your message: ";
        getline(cin, data);
        if(data == "q") {
            open = false;
        }
        else if (data == "") {
            string result = clientStream.str();
            cout << "Received message: " << result << endl;
            clientStream.str("");
        } else {
            for (auto byte : data){
                clientStream << (unsigned char) byte;
            }
            client->transmit();
        }

    }

    /*client.setOnReceive([](){
        unsigned char index0;
        while(clientStream >> index0){
            inData.push_back(index0);
        }
        string result(inData.begin(), inData.end());
        cout << "Received message: " << result << endl;
    });*/

    return 0;
}