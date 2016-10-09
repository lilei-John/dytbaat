#include <string>
#include <iostream>
#include "PaWrapper.h"

using namespace std;

PaWrapper::PaWrapper() {
    if(!paLifeHandler.initSuccess()) {
        throw paLifeHandler.getPaErrorText();
    }
}

bool PaWrapper::open(){
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
            SAMPLE_RATE,
            paFramesPerBufferUnspecified,
            paNoFlag,
            &PaWrapper::paCallback,
            this
    );
    if (err != paNoError) throw (Pa_GetErrorText(err));
    err = Pa_StartStream(paStream);
    if (err != paNoError) throw (Pa_GetErrorText(err));
    return true;
}

int PaWrapper::paCallback(const void *inputBuffer, void *outputBuffer,
                                             unsigned long framesPerBuffer,
                                             const PaStreamCallbackTimeInfo *timeInfo,
                                             PaStreamCallbackFlags statusFlags, void *userData) {
    PaCallbackData paCBData;
    paCBData.inputBuffer = inputBuffer;
    paCBData.outputBuffer = outputBuffer;
    paCBData.framesPerBuffer = framesPerBuffer;
    paCBData.timeInfo = timeInfo;
    paCBData.statusFlags = statusFlags;

    return ((PaWrapper*)userData)->callback(paCBData);
}

PaStreamCallbackResult PaWrapper::callback(PaCallbackData paCBData) {
    if (onCallback == nullptr) return paComplete;
    return (*onCallback)(paCBData);
}

void PaWrapper::setOnCallback(PaStreamCallbackResult(*onCB)(PaCallbackData)) {
    onCallback = onCB;
}