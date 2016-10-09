/*
 * This class refactors the Portaudio initialization and cleans up in the destructor.
 * */

#pragma once
#include "portaudio.h"
#include <string>

class PaLifeHandler {
public:
    PaLifeHandler() : paError(Pa_Initialize()){}
    ~PaLifeHandler(){
        if (initSuccess()){
            Pa_Terminate();
        }
    }
    const bool initSuccess(){
        return paError == paNoError;
    }
    const PaError getPaError(){
        return paError;
    }
    const std::string getPaErrorText(){
        return Pa_GetErrorText(paError);
    }
private:
    PaError paError;
};