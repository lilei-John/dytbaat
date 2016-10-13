//
// Created by Haukur Kristinsson on 06/10/2016.
//

#include "../CommunicationService.h"
#include "math.h"

TransmissionLayer::TransmissionLayer() {

}


void TransmissionLayer::nibbleToSamples(unsigned char byte, std::vector<float> &samples) {

    std::pair<int, int> rowCol = charToFreq(byte);

    for( int i = 0; i < SAMPLES_PER_TONE; i++ ) {
        samples.push_back((float) (sin( (double)i * rowCol.first * 2. * M_PI / SAMPLE_RATE)
                            + sin( (double)i * rowCol.second * 2. * M_PI / SAMPLE_RATE))/2);
        }

}

std::pair<int, int> TransmissionLayer::charToFreq(unsigned char byte) {

    std::pair<int, int> rowCol;

    int row[] = {697, 770, 852, 941};
    int col[] = {1209, 1336, 1477, 1633};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if(DTMFValues[i][j] == byte) {
                rowCol.first = row[i];
                rowCol.second = col[j];
                return rowCol;
            }
        }
    }
    return rowCol;
}

void TransmissionLayer::frameToSamples(std::vector<float> &samples, std::vector<unsigned char> &frame) {

    samples.clear();
    std::vector<unsigned char> nibbleFrame;
    byteFrameToNibbleFrame(frame, nibbleFrame);
    for (int i = 0; i < nibbleFrame.size(); ++i) {
        nibbleToSamples(nibbleFrame[i],samples);
    }
}


















