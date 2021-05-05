
// PortAudio API wrapper by Wouter Ensink

#pragma once

#include <portaudio.h>
#include <exception>

class AudioIODeviceCallback {
public:
    virtual void prepareToPlay(int sampleRate, int numSamplesPerBlock) {}

    virtual void process(float* input, float* output, int numSamples, int numChannels) = 0;

    virtual void releaseResources() {}
};


class PortAudio {
public:
    PortAudio(AudioIODeviceCallback& callback) : callback { callback } {}
    ~PortAudio() {}

    void setup(int sampleRate, int framesPerBuffer, int numOutputChannels = 2)
    {
        if (Pa_Initialize() != paNoError)
            throw std::runtime_error("failed to initialize port audio");

        inputParameters.device = Pa_GetDefaultInputDevice();
        outputParameters.device = Pa_GetDefaultOutputDevice();

        if (outputParameters.device == paNoDevice)
            throw std::runtime_error("no default output device");
        
        if (inputParameters.device == paNoDevice)
            throw std::runtime_error("no default input device");
        
        inputParameters.channelCount = numOutputChannels;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowOutputLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;
        
        outputParameters.channelCount = numOutputChannels;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;

        callback.prepareToPlay(sampleRate, framesPerBuffer);

        auto error = Pa_OpenStream(&stream,
                                   &inputParameters,
                                   &outputParameters,
                                   sampleRate,
                                   framesPerBuffer,
                                   paClipOff,
                                   &PortAudio::internalCallback,
                                   this);

        if (error != paNoError)
            throw std::runtime_error("failed to open stream");

        Pa_SetStreamFinishedCallback(stream, &PortAudio::streamFinished);

        if (Pa_StartStream(stream) != paNoError)
            throw std::runtime_error("failed to start stream");
    }


    void teardown()
    {
        if (Pa_StopStream(stream) != paNoError)
            throw std::runtime_error("failed to stop stream");


        if (Pa_CloseStream(stream) != paNoError)
            throw std::runtime_error("failed to close stream");

        Pa_Terminate();
    }


private:
    
    AudioIODeviceCallback& callback;
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    PaStream* stream;

    
    static int internalCallback(const void* inputBuffer, void* outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData)
    {
        auto* input = (float*) inputBuffer;
        auto* output = (float*) outputBuffer;

        auto* self = reinterpret_cast<PortAudio*> (userData);

        self->callback.process(input, output, framesPerBuffer, self->outputParameters.channelCount);

        return 0;
    }
    

    static void streamFinished(void* userData)
    {
        auto* self = reinterpret_cast<PortAudio*> (userData);
        self->callback.releaseResources();
    }

};


