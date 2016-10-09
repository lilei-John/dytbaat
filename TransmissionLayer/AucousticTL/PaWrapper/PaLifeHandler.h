/*
 * This class refactors the Portaudio initialization and cleans up in the destructor.
 * */

#pragma once
#include "Portaudio.h"

class PaLifeHandler {
public:
    PaLifeHandler() : paError(Pa_Initialize()){}
    ~PaLifeHandler(){
        if (initSuccess()){
            Pa_Terminate();
        }
    }
    bool initSuccess() const {
        return paError == paNoError;
    }
    PaError getPaError() const {
        return paError;
    }
private:
    PaError paError;
};