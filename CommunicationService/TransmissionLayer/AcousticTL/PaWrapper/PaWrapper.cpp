#include <string>
#include <iostream>
#include "PaWrapper.h"

using namespace std;

PaWrapper::PaWrapper(double sr, function<void(PaCallbackData)> cb) : sampleRate(sr), userCallback(cb){
    if(!paLifeHandler.initSuccess()) {
        throw paLifeHandler.getPaErrorText();
    }
    open();
}

void PaWrapper::open(){
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = 1;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaStreamParameters inputParameters = outputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;

    PaError err = Pa_OpenStream(
            &paStream,
            &inputParameters,
            &outputParameters,
            sampleRate,
            paFramesPerBufferUnspecified,
            paNoFlag,
            &PaWrapper::paCallback,
            this
    );
    if (err != paNoError) throw (Pa_GetErrorText(err));
    err = Pa_StartStream(paStream);
    if (err != paNoError) throw (Pa_GetErrorText(err));

}

int PaWrapper::paCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *objectLink) {
    return ((PaWrapper*)objectLink)->callback(
            PaCallbackData{inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags}
    );
}

PaStreamCallbackResult PaWrapper::callback(PaCallbackData paCBData) {
    if (userCallback == nullptr) return paComplete;
    userCallback(paCBData);
    return paContinue;
}