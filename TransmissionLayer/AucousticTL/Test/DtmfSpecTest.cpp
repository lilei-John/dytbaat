//
// Created by Haukur Kristinsson on 12/10/2016.
//

#include <iostream>
#include "../AucousticTL.h"

using namespace std;

vector<float> samples;


int main() {

    DtmfSpec spec;

    cout << "Frequencies 697Hz and 1477Hz to nibble:" << endl;
    if(spec.freqsToNibble(pair<int,int>(697, 1477)) == 0b0010) {
        cout << "0b0010" << endl;
    }

    cout << endl << "Nibble 0b0010 to frequencies:" << endl;
    cout << "Row Frequency: " << spec.nibbleToFreqs(0b0010).first << endl;
    cout << "Column Frequency: " << spec.nibbleToFreqs(0b0010).second << endl;
    return 0;
}
