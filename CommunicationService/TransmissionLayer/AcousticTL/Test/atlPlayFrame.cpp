#include "../AcousticTL.h"
#include "../../../../PaWrapper/PaWrapper.h"
#include <vector>

using namespace std;

int main(){
    double sampleRate = 44100;
    AcousticTL atl(sampleRate, 20000, 2000);
    PaWrapper paWrapper(sampleRate);
    paWrapper.setOnOutRequest([&](vector<float> &out){
        atl.setOutput(out);
    });
    atl.sendFrame({0x12, 0x34, 0x45, 0x78, 0x9A});
    cin.get();
    return 0;
}