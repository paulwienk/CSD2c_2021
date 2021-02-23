/*
 * PortAudio API wrapper by Wouter Ensink
 */

#include <random>
#include <iostream>
#include "port_audio.h"
#include "saw.h"

class NoiseTestCallback : public AudioIODeviceCallback
{
public:

    void prepareToPlay (int sampleRate, int blockSize) override
    {
        std::cout << "starting callback\n";
        saw.setSampleRate(sampleRate);
    }
    
    void process (float* input, float* output, int numSamples, int numChannels) override
    {
        for (auto sample = 0; sample < numSamples; ++sample)
        {
            output[sample * 2] = input[sample * 2] * saw.getSample();
            output[sample * 2 + 1] = input[sample * 2 + 1] * saw.getSample();
            saw.tick();
        }
    }
    
    void releaseResources() override
    {
        std::cout << "stopping callback\n";
    }


    Saw saw{800, 44100};

};

// ========================================================================================

int main()
{
    auto callback = NoiseTestCallback {};
    auto portAudio = PortAudio { callback };

    auto sampleRate = 44100;
    auto blockSize = 64;
    
    try
    {
        portAudio.setup (sampleRate, blockSize);
    
        std::cout << "Press enter to exit...\n";
        std::cin.get();

        portAudio.teardown();
    }
    
    catch (std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
