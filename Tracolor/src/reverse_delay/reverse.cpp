//
// Created by steve on 5/5/2021.
//

#include "reverse.h"

Reverse::Reverse(unsigned int bufferSize, float sampleRate) :
delayLine(bufferSize), sampleRate(sampleRate)
{

}

Reverse::~Reverse()
{

}

void Reverse::process(float *inBuffer, float *outBuffer, unsigned int frames)
{
    delayLine.write(inBuffer[frames]);

    outBuffer[frames] = delayLine.read() * gain();

    delayLine.tick();
}

// formula found in this paper https://ccrma.stanford.edu/~jingjiez/portfolio/echoing-harmonics/pdfs/A%20Pitch%20Shifting%20Reverse%20Echo%20Audio%20Effect.pdf
float Reverse::gain()
{
    float gain;
    float delayMax = getBufferSize();
    float distance = delayLine.getDistanceReadWriteHead();

    gain = 4 * distance/delayMax * (1 - distance/delayMax);

    return gain;
}

unsigned int Reverse::getBufferSize()
{
    return bufferSize;
}


