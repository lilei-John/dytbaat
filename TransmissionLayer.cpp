//
// Created by Haukur Kristinsson on 06/10/2016.
//

#include "CommunicationService.h"
#include <math.h>

TransmissionLayer::TransmissionLayer() {

}

char TransmissionLayer::samplesToChar(float *samples) {

    int sampleRate = SAMPLE_RATE;
    int blockSize = FRAMES_PER_BUFFER;
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

        for (int j = 0; j < FRAMES_PER_BUFFER; j++) {
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
    char ampRowCol[4][4] = {
            {'1', '2', '3', 'a'},
            {'4','5', '6', 'b'},
            {'7','8', '9', 'c'},
            {'*','0', '#', 'd'}
    };

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

void TransmissionLayer::charToSamples(char byte, float *samples, int numberOfSamples) {

    std::pair<int, int> rowCol = charToFreq(byte);

    for( int i = 0; i < numberOfSamples; i++ ) {
        samples[i] = (float) (sin( (double)i * rowCol.first * 2. * M_PI / SAMPLE_RATE)
                            + sin( (double)i * rowCol.second * 2. * M_PI / SAMPLE_RATE))/2;
        }


}

std::pair<int, int> TransmissionLayer::charToFreq(char byte) {

    std::pair<int, int> rowCol;
    char ampRowCol[4][4] = {
            {'1','2', '3', 'a'},
            {'4','5', '6', 'b'},
            {'7','8', '9', 'c'},
            {'*','0', '#', 'd'}
    };

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
}




















