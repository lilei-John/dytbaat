//
// Created by Haukur Kristinsson on 07/10/2016.
//

#include "AudioLibrary.h"

AudioLibrary::AudioLibrary() : stream(0), left_phase(0), right_phase(0)
{
    initSineWaveTable();
    sprintf( message, "No Message" );
}

bool AudioLibrary::open(PaDeviceIndex index)
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
            &AudioLibrary::paPlayCallback,
            this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
    );

    if (err != paNoError)
    {
        /* Failed to open stream to device !!! */
        return false;
    }

    err = Pa_SetStreamFinishedCallback( stream, &AudioLibrary::paStreamFinished );

    if (err != paNoError)
    {
        Pa_CloseStream( stream );
        stream = 0;

        return false;
    }

    return true;
}

bool AudioLibrary::record(PaDeviceIndex index)
{
    int totalFrames = 0;
    maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    frameIndex = 0;
    //int numSamples = totalFrames;
    //int numBytes = numSamples * sizeof(float);
    //recordedSamples = (float *) malloc( numBytes );
    //if( recordedSamples == NULL )
    //{
    //    cout << "Could not allocate record array." << endl;
    //}
    //for(int i=0; i<numSamples; i++ ) recordedSamples[i] = 0;
    PaStreamParameters inputParameters;

    inputParameters.device = index;
    if (inputParameters.device == paNoDevice) {
        return false;
    }

    inputParameters.channelCount = 1;       /* stereo output */
    inputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    PaError err = Pa_OpenStream(
            &stream,
            &inputParameters, /* no input */
            NULL,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            &AudioLibrary::paRecordCallback,
            this          /* Using 'this' for userData so we can cast to Sine* in paCallback method */
    );

    if (err != paNoError)
    {
        /* Failed to open stream to device !!! */
        return false;
    }

    err = Pa_SetStreamFinishedCallback( stream, &AudioLibrary::paStreamFinished);

    if (err != paNoError)
    {
        Pa_CloseStream( stream );
        stream = 0;

        return false;
    }

    return true;
}


void AudioLibrary::initSineWaveTable() {
    int sampleLength = 4000;
    const double dtmfRow[] = {697., 770., 852., 941.};
    const double dtmfCol[] = {1209., 1336., 1477., 1633.};
    /* initialise sinusoidal wavetable */
    for (int i = 0; i < dtmfNum.length(); i++) {
        setDtmfColRow(i);

        for( int j = i * sampleLength; j <= (i+1) * sampleLength; j++ ) {
            sine[j] = (float) (sin( (double)j * dtmfCol[col] * 2. * M_PI / SAMPLE_RATE)
                               + sin( (double)j * dtmfRow[row] * 2. * M_PI / SAMPLE_RATE))/2;
        }

    }
}


bool AudioLibrary::close()
{
    if (stream == 0)
        return false;

    PaError err = Pa_CloseStream( stream );
    stream = 0;

    return (err == paNoError);
}


bool AudioLibrary::start()
{
    if (stream == 0)
        return false;

    PaError err = Pa_StartStream( stream );

    return (err == paNoError);
}

bool AudioLibrary::stop()
{
    if (stream == 0)
        return false;

    PaError err = Pa_StopStream( stream );

    return (err == paNoError);
}

void AudioLibrary::setDtmfNum(std::string newNum) {
    dtmfNum = newNum;
}

void AudioLibrary::setDtmfColRow(int i){
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

bool AudioLibrary::playDtmf(std::string newNum) {


    PaError err;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    dtmfNum = newNum;
    initSineWaveTable();

    if (open(Pa_GetDefaultOutputDevice()))
    {
        if (start())
        {
            printf("Play for %d seconds.\n", NUM_SECONDS );
            Pa_Sleep( NUM_SECONDS * 1000 );

            stop();
        }
        close();
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

bool AudioLibrary::recordDtmf() {


    PaError err;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    if (record(Pa_GetDefaultInputDevice()))
    {
        if (start())
        {
            printf("Play for %d seconds.\n", NUM_SECONDS );
            Pa_Sleep( NUM_SECONDS * 1000 );

            stop();
        }
        close();
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
int AudioLibrary::paPlayCallbackMethod(const void *inputBuffer, void *outputBuffer,
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
int AudioLibrary::paPlayCallback( const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData )
{
    /* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since
     we called Pa_OpenStream with 'this' for userData */
    return ((AudioLibrary*)userData)->paPlayCallbackMethod(inputBuffer, outputBuffer,
                                                   framesPerBuffer,
                                                   timeInfo,
                                                   statusFlags);
}


/* The instance callback, where we have access to every method/variable in object of class Sine */
int AudioLibrary::paRecordCallbackMethod(const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags)
{
    //unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) outputBuffer;

    //for( i=0; i<framesPerBuffer; i++ )
    //{

    //}

    //return paContinue;

    const float *rptr = (const float*)inputBuffer;
    //float *wptr = &recordedSamples[frameIndex];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = maxFrameIndex - frameIndex;

    //cout << maxFrameIndex << " : " << frameIndex << endl;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) outputBuffer;

    if( framesLeft < framesPerBuffer )
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesToCalc; i++ )
        {
            recSample.push(0.0f);
        }
    }
    else
    {
        while (safeGuard) {

        }
        safeGuard = true;
        for( i=0; i<framesToCalc; i++ )
        {
            recSample.push(*rptr++);

            //*wptr++ = *rptr++;
            //recordedSamples[frameIndex+i] = *rptr++;
            //cout << recordedSamples[frameIndex+i] << endl;
        }
        safeGuard = false;
    }
    frameIndex += framesToCalc;
    return finished;

}

/* This routine will be called by the PortAudio engine when audio is needed.
 ** It may called at interrupt level on some machines so don't do anything
 ** that could mess up the system like calling malloc() or free().
 */
int AudioLibrary::paRecordCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    /* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since
     we called Pa_OpenStream with 'this' for userData */
    return ((AudioLibrary*)userData)->paRecordCallbackMethod(inputBuffer, outputBuffer,
                                                     framesPerBuffer,
                                                     timeInfo,
                                                     statusFlags);

}

void AudioLibrary::paStreamFinishedMethod()
{
    printf( "Stream Completed: %s\n", message );
    for (int i = 0; i <= recSample.size(); i++) {
        //cout << recSample.front() << endl;
        //recSample.pop();
    }
}

/*
 * This routine is called by portaudio when playback is done.
 */
void AudioLibrary::paStreamFinished(void* userData)
{
    return ((AudioLibrary*)userData)->paStreamFinishedMethod();
}


void AudioLibrary::getFrame(float frame[FRAMES_PER_BUFFER]) {
    while (safeGuard) {

    }
    safeGuard = true;
    if (recSample.size() > FRAMES_PER_BUFFER) {
        for (int i = 0; i < FRAMES_PER_BUFFER; i++) {
            frame[i] = recSample.front();
            recSample.pop();
        }
    }
    safeGuard = false;
}


bool AudioLibrary::frameReady() {
    if (recSample.size() > FRAMES_PER_BUFFER) {
        return true;
    }
    return false;
}