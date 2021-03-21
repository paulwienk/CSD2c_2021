#define _USE_MATH_DEFINES

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

// returnt de huidige sample
float Oscillator::getSample() {
    return sample;
}


void Oscillator::setFrequency(float frequency) {
    this->frequency = frequency;
    // phaseIncrement changes based on the new frequency
    phaseIncrement = frequency / sampleRate;
}

void Oscillator::setSampleRate(float sampleRate) {
    this->sampleRate = sampleRate;
    phaseIncrement = frequency / sampleRate;
}


void Oscillator::setAmplitude(float amp) {
    amplitude = amp;
}