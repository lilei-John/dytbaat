//
//  Sine.hpp
//  PortAudioTesting
//
//  Created by Haukur Kristinsson on 21/09/2016.
//  Copyright © 2016 Haukur Kristinsson. All rights reserved.
//

#ifndef Sine_hpp
#define Sine_hpp

#include <math.h>
#include "portaudio.h"
#include <iostream>
#include <stdio.h>

#define NUM_SECONDS   (1)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (1000000)

using namespace std;

class Sine
{
public:
    Sine();
    bool open(PaDeviceIndex index);
    void initSineWaveTable();
    bool close();
    bool start();
    bool stop();
    void setDtmfNum(string newNum);
    void setDtmfColRow(int i);
    bool playDtmf();
    
private:
    string dtmfNum = "";
    int row = 0, col = 0;
    int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags);
    
    static int paCallback( const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData );
    
    void paStreamFinishedMethod();
    
    static void paStreamFinished(void* userData);
    PaStream *stream;
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
    
};


#endif /* Sine_hpp */
