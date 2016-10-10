#include <stdio.h>
#include <iostream>
#include <future>
#include "CommunicationService.h"

using namespace std;

int main(void)
{
    std::vector<float> samples;
    std::vector<unsigned char> frame = {0b10001000,0b10001000,0b10001000,0b10001000};
    //std::vector<unsigned char> nibbleFrame = {0b00000011, 0b00001011, 0b00001000, 0b00001111};
    //std::vector<unsigned char> frame;
    TransmissionLayer tml;
    //tml.frameToSamples(samples, frame);
    //cout << tml.samplesToNibble(samples) << endl;


    tml.frameToSamples(samples,frame);

    if (tml.samplesToNibble(samples) == 0b00001001) {
        cout << "Success!"<< endl;
    }
    cout << tml.samplesToNibble(samples) << endl;
    cout << samples.size() << endl;

    //pair<int, int> rowCol = tml.charToFreq('a');
    //cout << rowCol.first << " " << rowCol.second << endl;

    return 0;
}
