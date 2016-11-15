#include "DtmfAnalysis.h"
#include "../helperFunctions.h"
#include <algorithm>
#include <iostream>

using namespace std;

bool freqAmpSort(pair<int, float> a, pair<int, float> b){
    return a.second > b.second;
}

float calcCertainty(vector<pair<int, float>> v){
    if (v[0].second == 0) return 0;
    return (v[0].second - v[1].second) / v[0].second;
}

DtmfAnalysis::DtmfAnalysis(const float *its, int size, DtmfSpec dtmfSpec, int sampleRate) {
    rowFreqAmp = goertzelFilter(its, size, dtmfSpec.getFreqRow(), sampleRate);
    colFreqAmp = goertzelFilter(its, size, dtmfSpec.getFreqCol(), sampleRate);

    sort(rowFreqAmp.begin(), rowFreqAmp.end(), freqAmpSort);
    sort(colFreqAmp.begin(), colFreqAmp.end(), freqAmpSort);

    nipple = dtmfSpec.freqsToNibble(make_pair(rowFreqAmp[0].first, colFreqAmp[0].first));
    certainty = min(calcCertainty(rowFreqAmp), calcCertainty(colFreqAmp));
}

unsigned char DtmfAnalysis::getNipple() {
    return nipple;
}

float DtmfAnalysis::getCertainty(){
    return certainty;
}