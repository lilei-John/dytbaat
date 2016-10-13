//
// Created by Rasmus Haugaard on 13/10/2016.
//

#include "../AcousticTL.h"
#include <vector>

using namespace std;

int main(){
    AcousticTL acousticTL;
    vector<unsigned char> byteFrame = {0b0, 0b1, 0b10, 0b100, 0b1000, 0b10000, 0b100000, 0b1000000, 0b10000000};
    acousticTL.sendFrame(byteFrame);
    cin.get();
    return 0;
}