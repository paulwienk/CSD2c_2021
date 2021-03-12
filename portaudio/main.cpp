
// PortAudio API wrapper by Wouter Ensink

#include <cmath>
#include <random>
#include <iostream>
#include <string>
#include "port_audio.h"
#include "saw.h"
#include "circBuffer.h"

constexpr double twoPi = 3.14159265359 * 2;

// generates a waveform that will be merged with the sawtooth
float windowing(float input) {
    input -= 0.5;
    input *= 0.5;

    return (float) std::cos(input * twoPi);

}

// the pitcher class returns 2 samples, 1 with a sawtooth and the windowing waveform
// and 1 which is the same but 180 degrees shifted
class Pitcher {
public:

    Pitcher() = default;

    float process(float input) {
        saw.tick();
        auto sawSample = saw.getSample();

        circBuffer1.setDistanceRW(sawSample * 4410);
        circBuffer2.setDistanceRW(std::fmod(sawSample + 0.5, 1) * 4410);

        circBuffer1.write(input);
        circBuffer2.write(input);

        auto sample1 = circBuffer1.read() * windowing(sawSample);
        auto sample2 = circBuffer2.read() * windowing(std::fmod(sawSample + 0.5, 1));

        circBuffer1.tick();
        circBuffer2.tick();


        return sample1 + sample2;


    }

    Saw saw{4, 44100};
    CircBuffer circBuffer1{44100};
    CircBuffer circBuffer2{44100};

};

class NoiseTestCallback : public AudioIODeviceCallback
{
public:

    void prepareToPlay (int sampleRate, int blockSize) override
    {
        std::cout << "starting callback\n";

    }

    // both channels are added together in the input buffer and mixed with the pitcher class
    void process (float* input, float* output, int numSamples, int numChannels) override
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
    
    void releaseResources() override
    {
        std::cout << "stopping callback\n";
    }


    Pitcher pitcher;

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
            callback.pitcher.saw.setFrequency(getNumericInput());


        portAudio.teardown();
    }
    
    catch (std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
