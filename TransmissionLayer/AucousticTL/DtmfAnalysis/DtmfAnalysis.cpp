#include "DtmfAnalysis.h"
#include "../helperFunctions.h"
#include <algorithm>

using namespace std;

bool freqAmpSort(pair<int, float> p1, pair<int, float> p2){
    return p1.second > p2.second;
}

float calcCertainty(vector<pair<int, float>> v){
    return (v[0].second - v[1].second) / v[0].second;
}

DtmfAnalysis::DtmfAnalysis(const vector<float> &samples, DtmfSpec dtmfSpec, int sampleRate) {
    rowFreqAmp = goertzelFilter(samples, dtmfSpec.getFreqRow(), sampleRate);
    colFreqAmp = goertzelFilter(samples, dtmfSpec.getFreqCol(), sampleRate);
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