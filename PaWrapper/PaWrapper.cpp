#include <string>
#include <iostream>
#include "PaWrapper.h"

using namespace std;

PaWrapper::PaWrapper(double sr) : sampleRate(sr) {
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
            512,
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
            PaCallbackData{(float *)inputBuffer, (float *)outputBuffer, framesPerBuffer, timeInfo, statusFlags}
    );
}

PaStreamCallbackResult PaWrapper::callback(PaCallbackData paCBData) {
    vector<float> in;
    in.assign(paCBData.inputBuffer, paCBData.inputBuffer + paCBData.framesPerBuffer);
    if (onInReceived) onInReceived(in);

    vector<float> out (paCBData.framesPerBuffer);
    if (onOutRequest) onOutRequest(out);
    for (int i = 0; i < paCBData.framesPerBuffer; i++){
        *(paCBData.outputBuffer + i) = out[i];
    }
    return paContinue;
}

void PaWrapper::setOnInReceived(const function<void(vector<float> &)> &onInReceived) {
    PaWrapper::onInReceived = onInReceived;
}

void PaWrapper::setOnOutRequest(const function<void(vector<float> &)> &onOutRequest) {
    PaWrapper::onOutRequest = onOutRequest;
}