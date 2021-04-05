
// Oscillator by Paul Wienk

#include <math.h>
#include "oscillator.h"

// modulo function by Wouter Ensink
template<typename T>
inline constexpr T modulo(T dividend, const T divisor) noexcept {
    while (dividend >= divisor)
        dividend -= divisor;

    return dividend;
}

// function that updates the phase and keeps the phase between 0 and 1 with the modulo function
void Oscillator::updatePhase() {
    phase = modulo(phase + phaseIncrement, 1.0);
}

// current sample return
float Oscillator::getSample() {
    return sample;
}


void Oscillator::setFrequency(float frequency) {
    this->frequency = frequency;
    // phaseIncrement changes based on the new frequency
    phaseIncrement = frequency / sampleRate;
}

