#include "DtmfAnalysis.h"
#include "../helperFunctions.h"
#include <algorithm>
#include <iostream>

using namespace std;

bool freqAmpSort(const pair<int, float> &a, const pair<int, float> &b){
    return a.second > b.second;
}

float calcCertainty(vector<pair<int, float>> &v){
    if (v[0].second == 0) return 0;
    return (v[0].second - v[1].second) / v[0].second;
}

DtmfAnalysis::DtmfAnalysis(const float *its, int size, const DtmfSpec &dtmfSpec, int sampleRate) {
    auto rowFreqAmp = goertzelFilter(its, size, dtmfSpec.getFreqRow(), sampleRate);
    auto colFreqAmp = goertzelFilter(its, size, dtmfSpec.getFreqCol(), sampleRate);

    sort(rowFreqAmp.begin(), rowFreqAmp.end(), freqAmpSort);
    sort(colFreqAmp.begin(), colFreqAmp.end(), freqAmpSort);

    nibble = dtmfSpec.freqsToNibble(make_pair(rowFreqAmp[0].first, colFreqAmp[0].first));
    certainty = min(calcCertainty(rowFreqAmp), calcCertainty(colFreqAmp));
}

unsigned char DtmfAnalysis::getNibble() const {
    return nibble;
}

float DtmfAnalysis::getCertainty() const {
    return certainty;
}
