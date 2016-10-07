//
// Created by Haukur Kristinsson on 07/10/2016.
//

#ifndef DYTBAAT_PORTAUDIO_H
#define DYTBAAT_PORTAUDIO_H

#include <math.h>
#include "portaudio.h"
#include <iostream>
#include <stdio.h>
#include <queue>

#define NUM_SECONDS   (10)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (500)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (1000000)


class AudioLibrary
{
public:
    //Default Constructor
    AudioLibrary();

    //Functions
    bool open(PaDeviceIndex index);
    bool record(PaDeviceIndex index);
    void initSineWaveTable();
    bool close();
    bool start();
    bool stop();
    void setDtmfNum(std::string newNum);
    void setDtmfColRow(int i);
    bool playDtmf(std::string newNum);
    bool recordDtmf();
    void getFrame(float frame[FRAMES_PER_BUFFER]);
    bool frameReady();

private:
    std::string dtmfNum = "";
    int row = 0, col = 0;
    float *recordedSamples;
    int frameIndex;        /* Index into sample array. */
    int maxFrameIndex;
    std::queue <float> recSample;
    bool safeGuard = false;

    int paPlayCallbackMethod(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags);

    int paRecordCallbackMethod(const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags);

    static int paPlayCallback( const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData );

    static int paRecordCallback( const void *inputBuffer, void *outputBuffer,
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


#endif //DYTBAAT_PORTAUDIO_H
