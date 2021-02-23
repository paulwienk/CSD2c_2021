#pragma once

#include <iostream>
#include "oscillator.h"

// saw class, derived from base class Oscillator
class Saw : public Oscillator {
public:
    Saw(double frequency, double sampleRate) : Oscillator(frequency, sampleRate) {}

    void tick() override;
};