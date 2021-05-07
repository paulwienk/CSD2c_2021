//
// Based on PortAudio API wrapper by Wouter Ensink
//

#include "audioModuleAdapter.h"

#include <utility>

AudioModuleAdapter::AudioModuleAdapter() : pitcher() {}

AudioModuleAdapter::~AudioModuleAdapter() = default;

void AudioModuleAdapter::prepareToPlay (int sampleRate, int blockSize)
{
std::cout << "starting callback\n";
}

// both channels are added together in the input buffer and mixed with the pitcher class
void AudioModuleAdapter::process (float* input, float* output, int numSamples, int numChannels)
{
    for (auto sample = 0; sample < numSamples; ++sample)
    {
        auto left = input[sample * numChannels];
        auto right = input[sample * numChannels + 1];
        auto in = (left + right) / 2;
        auto out = pitcher.process(in);

        output[sample * numChannels] = out;
        output[sample * numChannels + 1] = out;
    }
}

void AudioModuleAdapter::releaseResources()
{
    std::cout << "stopping callback\n";
}


