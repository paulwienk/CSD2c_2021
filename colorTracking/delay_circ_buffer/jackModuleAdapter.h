// JackModuleAdapter by Wouter Ensink

#ifndef TRACOLOR_JACKMODULEADAPTER_H
#define TRACOLOR_JACKMODULEADAPTER_H

#include <functional>
#include <vector>

struct JackModuleAdapter : AudioIODeviceCallback
{
    void prepareToPlay(int sampleRate, int numSamplesPerBlock) override
    {
        this->sampleRate = sampleRate;
        scratchBuffer.resize(numSamplesPerBlock);
    }

    void process(float* in, float* out, int numSamples, int numChannels) override
    {
        for (int i = 0; i < numSamples; ++i)
        {
            scratchBuffer[i] = in[i*2];
        }

        callback(scratchBuffer.data(), scratchBuffer.data(), numSamples);

        for (int i = 0; i < numSamples; ++i)
        {
            out[2*i] = out[2*i+1] = scratchBuffer[i];
        }
    }

    std::function<void(float*, float*, int)> callback;
    int sampleRate = 0;

private:
    std::vector<float> scratchBuffer;
};

#endif //TRACOLOR_JACKMODULEADAPTER_H
