#define _USE_MATH_DEFINES

#include "saw.h"
#include <math.h>


void Saw::tick() {
    updatePhase();

    // calculations for the saw wave.
    // ((phase * 2) - 1) creates a gradual, diagonal line from -1 to 1.
    // when phase reaches 1, it is set to 0 again by the updatePhase function, which results to sample = -1.
    sample = ((phase * 2) - 1) * amplitude;
}