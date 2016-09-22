//
//  Sine.cpp
//  PortAudioTesting
//
//  Created by Haukur Kristinsson on 21/09/2016.
//  Copyright © 2016 Haukur Kristinsson. All rights reserved.
//

#include "Sine.hpp"

Sine::Sine() : stream(0), left_phase(0), right_phase(0)
{
    /*const double dtmfRow[] = {697., 770., 852., 941.};
     const double dtmfCol[] = {1209., 1336., 1477., 1633.};
     
     //initialise sinusoidal wavetable
     for( int i=0; i<TABLE_SIZE; i++ )
     {
     //sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
     sine[i] = (float) sin( (double)i * dtmfCol[col] * 2. * M_PI / SAMPLE_RATE)
     + sin( (double)i * dtmfRow[row] * 2. * M_PI / SAMPLE_RATE);
     */
    initSineWaveTable();
    
    sprintf( message, "No Message" );
}

bool Sine::open(PaDeviceIndex index)
{
    PaStreamParameters outputParameters;
    
    outputParameters.device = index;
    if (outputParameters.device == paNoDevice) {
        return false;
    }
    
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    PaError err = Pa_OpenStream(
                                &stream,
                                NULL, /* no input */
                                &outputParameters,
                                SAMPLE_RATE,
                                FRAMES_PER_BUFFER,
                                paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                                &Sine::paCallback,
                                this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
                                );
    
    if (err != paNoError)
    {
        /* Failed to open stream to device !!! */
        return false;
    }
    
    err = Pa_SetStreamFinishedCallback( stream, &Sine::paStreamFinished );
    
    if (err != paNoError)
    {
        Pa_CloseStream( stream );
        stream = 0;
        
        return false;
    }
    
    return true;
}

void Sine::initSineWaveTable() {
    const double dtmfRow[] = {697., 770., 852., 941.};
    const double dtmfCol[] = {1209., 1336., 1477., 1633.};
    /* initialise sinusoidal wavetable */
    for( int i=0; i<TABLE_SIZE; i++ )
    {
        //sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
        sine[i] = (float) sin( (double)i * dtmfCol[col] * 2. * M_PI / SAMPLE_RATE)
        + sin( (double)i * dtmfRow[row] * 2. * M_PI / SAMPLE_RATE);
    }
}

bool Sine::close()
{
    if (stream == 0)
        return false;
    
    PaError err = Pa_CloseStream( stream );
    stream = 0;
    
    return (err == paNoError);
}


bool Sine::start()
{
    if (stream == 0)
        return false;
    
    PaError err = Pa_StartStream( stream );
    
    return (err == paNoError);
}

bool Sine::stop()
{
    if (stream == 0)
        return false;
    
    PaError err = Pa_StopStream( stream );
    
    return (err == paNoError);
}

void Sine::setDtmfNum(string newNum) {
    dtmfNum = newNum;
}

void Sine::setDtmfColRow(int i){
    switch (dtmfNum[i]) {
        case '1':
            col = 0;
            row = 0;
            break;
        case '2':
            col = 1;
            row = 0;
            break;
        case '3':
            col = 2;
            row = 0;
            break;
        case '4':
            col = 0;
            row = 1;
            break;
        case '5':
            col = 1;
            row = 1;
            break;
        case '6':
            col = 2;
            row = 1;
            break;
        case '7':
            col = 0;
            row = 2;
            break;
        case '8':
            col = 1;
            row = 2;
            break;
        case '9':
            col = 2;
            row = 2;
            break;
        case '0':
            col = 1;
            row = 3;
            break;
        case '*':
            col = 0;
            row = 3;
            break;
        case '#':
            col = 2;
            row = 3;
            break;
        case 'A':
        case 'a':
            col = 3;
            row = 0;
            break;
        case 'B':
        case 'b':
            col = 3;
            row = 1;
            break;
        case 'C':
        case 'c':
            col = 3;
            row = 2;
            break;
        case 'D':
        case 'd':
            col = 3;
            row = 3;
            break;
            
        default:
            break;
    }
    
}

bool Sine::playDtmf() {
    
    PaError err;
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
    
    for (int i = 0; i < dtmfNum.length(); i++) {
        
        setDtmfColRow(i);
        initSineWaveTable();
        if (open(Pa_GetDefaultOutputDevice()))
        {
            if (start())
            {
                printf("Play for %d seconds.\n", NUM_SECONDS );
                Pa_Sleep( NUM_SECONDS * 700 );
                
                stop();
            }
            close();
        }
    }
    
    Pa_Terminate();
    printf("Test finished.\n");
    
    return true;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return false;
}

/* The instance callback, where we have access to every method/variable in object of class Sine */
int Sine::paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo* timeInfo,
                     PaStreamCallbackFlags statusFlags)
{
    float *out = (float*)outputBuffer;
    unsigned long i;
    
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = sine[left_phase];  /* left */
        *out++ = sine[right_phase];  /* right */
        left_phase += 1;
        if( left_phase >= TABLE_SIZE ) left_phase -= TABLE_SIZE;
        right_phase += 1; /* higher pitch so we can distinguish left and right. */
        if( right_phase >= TABLE_SIZE ) right_phase -= TABLE_SIZE;
    }
    
    return paContinue;
    
}

/* This routine will be called by the PortAudio engine when audio is needed.
 ** It may called at interrupt level on some machines so don't do anything
 ** that could mess up the system like calling malloc() or free().
 */
 int Sine::paCallback( const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData )
{
    /* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since
     we called Pa_OpenStream with 'this' for userData */
    return ((Sine*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
                                               framesPerBuffer,
                                               timeInfo,
                                               statusFlags);
}


void Sine::paStreamFinishedMethod()
{
    printf( "Stream Completed: %s\n", message );
}

/*
 * This routine is called by portaudio when playback is done.
 */
void Sine::paStreamFinished(void* userData)
{
    return ((Sine*)userData)->paStreamFinishedMethod();
}




