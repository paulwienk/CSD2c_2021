
// Sawtooth formula by Paul Wienk

#include <cmath>
#include "saw.h"
#include "oscillator.h"
#define PI 3.1416


void Saw::tick() {
    updatePhase();

    // calculations for the saw wave.
    // the same as the phase, from 0 to 1.
    // when phase reaches 1, it is set to 0 again by the updatePhase function, which results to sample = 0.
    sample = phase;
    //std::cout << sample1 << "\n";
}





