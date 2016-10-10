//
// Created by Haukur Kristinsson on 06/10/2016.
//

#include "../CommunicationService.h"

TransmissionLayer::TransmissionLayer() {

}

unsigned char TransmissionLayer::samplesToNibble(std::vector<float> &samples) {

    int sampleRate = SAMPLE_RATE;
    int blockSize = (int)samples.size();
    int freq[8] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
    float amp[4];

    for (int i = 0; i < 8; ++i) {
        double k = 0.5 + ((blockSize*freq[i])/(sampleRate));
        double w = (2 * M_PI / blockSize) * k;
        double cosine = cos(w);
        double sine = sin(w);
        double coeff = 2 * cosine;
        double Q0 = 0;
        double Q1 = 0;
        double Q2 = 0;

        for (int j = 0; j < blockSize; j++) {
            Q0 = coeff * Q1 - Q2 + samples[j];
            Q2 = Q1;
            Q1 = Q0;
        }

        double real = Q1 - Q2 * cosine;
        double imag = Q2 * sine;

        amp[i] = sqrtf((float)((real*real)+(imag*imag)));
    }

    int highCol = 0;
    float ampCol = .0;
    int highRow = 0;
    float ampRow = .0;

    float aRow[4] = {amp[0], amp[1],amp[2],amp[3],};
    float aCol[4] = {amp[4], amp[5],amp[6],amp[7],};

    for (int i = 0; i < 4; i++) {
        if (aRow[i] > ampRow) {
            highRow = i;
            ampRow = aRow[i];
        }
        if (aCol[i] > ampCol) {
            highCol = i;
            ampCol = aCol[i];
        }
    }
    return ampRowCol[highRow][highCol];;

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
            if(ampRowCol[i][j] == byte) {
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

void TransmissionLayer::byteFrameToNibbleFrame(std::vector<unsigned char> &byteFrame,
                                               std::vector<unsigned char> &nibbleFrame) {
    for (int i = 0; i < byteFrame.size(); ++i) {
        unsigned char lowNibble = byteFrame[i];
        lowNibble = lowNibble &(unsigned char) 0b00001111;
        unsigned char highNibble = byteFrame[i];
        highNibble = highNibble >> 4;
        highNibble = highNibble &(unsigned char) 0b00001111;
        nibbleFrame.push_back(highNibble);
        nibbleFrame.push_back(lowNibble);
    }
}

void TransmissionLayer::nibbleFrameToByteFrame(std::vector<unsigned char> &nibbleFrame,
                                               std::vector<unsigned char> &byteFrame) {
    for (int i = 0; i < nibbleFrame.size(); i+=2) {
        unsigned char highNibble = nibbleFrame[i];
        unsigned char lowNibble = nibbleFrame[i+1];
        highNibble = highNibble << 4;
        byteFrame.push_back(highNibble | lowNibble);
    }
}


















