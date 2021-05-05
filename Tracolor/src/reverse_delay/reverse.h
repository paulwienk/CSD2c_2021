//
// Created by steve on 5/5/2021.
//

#ifndef TRACOLOR_REVERSE_H
#define TRACOLOR_REVERSE_H

#include "delayLine.h"

class Reverse {
public:
    Reverse(unsigned int bufferSize, float sampleRate);

    ~Reverse();

    // methods
    void process(float* inBuffer, float* outBuffer, unsigned int frames);

    float gain();

    // getters/setters
    unsigned int getBufferSize();


private:
    DelayLine delayLine;

    // variables
    unsigned int bufferSize;
    float sampleRate;
};


#endif //TRACOLOR_REVERSE_H
