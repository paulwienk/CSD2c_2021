#ifndef PITCHER_H
#define PITCHER_H

#include <cmath>
#include <random>
#include <iostream>
#include <string>
#include "saw.h"
#include "circBuffer.h"

constexpr auto twoPi = 3.14159265359 * 2;

// the pitcher class returns 2 samples, 1 with a sawtooth and the windowing waveform
// and 1 which is the same but 180 degrees shifted
class Pitcher {
public:

    Pitcher() = default;

    // generates a waveform that will be merged with the sawtooth
    float windowing(float input) {
        input -= 0.5;
        input *= 0.5;

        return (float) std::cos(input * twoPi);
    }

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

    Saw saw{20, 44100};
    CircBuffer circBuffer1{44100};
    CircBuffer circBuffer2{44100};
};

#endif

