//
//  main.cpp
//  PortAudioTesting
//
//  Created by Haukur Kristinsson on 21/09/2016.
//  Copyright © 2016 Haukur Kristinsson. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "Sine.hpp"

using namespace std;


int main(void)
{
    Sine sine;
    
    string dtmfNum = "";
    cout << "Please enter DTMF nubmer: " << endl;
    cin >> dtmfNum;
    
    sine.setDtmfNum(dtmfNum);
    sine.playDtmf();
    
    return 0;

}
