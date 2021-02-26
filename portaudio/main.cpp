
// PortAudio API wrapper by Wouter Ensink

#include <random>
#include <iostream>
#include <string>
#include "port_audio.h"
#include "saw.h"
#include "circBuffer.h"


class NoiseTestCallback : public AudioIODeviceCallback
{
public:

    void prepareToPlay (int sampleRate, int blockSize) override
    {
        std::cout << "starting callback\n";
        saw.setSampleRate(sampleRate);

    }

    // change in distance = change in pitch
    void process (float* input, float* output, int numSamples, int numChannels) override
    {
        for (auto sample = 0; sample < numSamples; ++sample)
        {
            circBufferL.setDistanceRW((saw.getSample() * 4410));
            circBufferR.setDistanceRW((saw.getSample() * 4410));
            circBufferL.write(input[sample * 2]);
            circBufferR.write(input[sample * 2 + 1]);
            circBufferL.tick();
            circBufferR.tick();
            output[sample * 2] = circBufferL.read();
            output[sample * 2 + 1] = circBufferR.read();
            saw.tick();
        }
    }
    
    void releaseResources() override
    {
        std::cout << "stopping callback\n";
    }


    Saw saw{30, 44100};
    CircBuffer circBufferL{44100};
    CircBuffer circBufferR{44100};

};

// function to change the pitch in the commandline, real change happens at line 77
auto getNumericInput() {
    auto ans = std::string {};
    std::getline(std::cin, ans);
    return std::stod(ans);
}


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
    
        while (true)
            callback.saw.setFrequency(getNumericInput());


        portAudio.teardown();
    }
    
    catch (std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
