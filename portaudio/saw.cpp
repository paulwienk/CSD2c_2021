
// Sawtooth formula by Paul Wienk

#include "saw.h"
#include <math.h>

void Saw::tick() {
    updatePhase();

    // calculations for the saw wave.
    // the same as the phase, from 0 to 1.
    // when phase reaches 1, it is set to 0 again by the updatePhase function, which results to sample = 0.
    sample = phase;

}