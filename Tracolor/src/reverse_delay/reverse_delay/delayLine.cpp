/* based on circBuffer by Ciska Vriezinga */

#include "delayLine.h"
#include <iostream>
#include <cstring>

// constructor and destructor
DelayLine::DelayLine(unsigned int size, unsigned int distance, float sampleRate) :
bufferSize(size), distanceReadWriteHead(distance)
{
    allocateBuffer();
}

DelayLine::~DelayLine()
{
    releaseBuffer();
}

// methods
void DelayLine::allocateBuffer()
{
    buffer = (float*)malloc(bufferSize * sizeof(float));
    memset(buffer, 0, bufferSize * sizeof(float));
}

void DelayLine::releaseBuffer()
{
    free(buffer);
}

void DelayLine::setBufferSize(unsigned int size)
{
    bufferSize = size;
    releaseBuffer();
    allocateBuffer();
}

void DelayLine::increaseReadHead()
{
    readHead++;
    wrapHead(readHead);
}

void DelayLine::increaseWriteHead()
{
    writeHead++;
    wrapHead(writeHead);
}

// based on circbuffer by Ciska Vriezinga
void DelayLine::wrapHead(unsigned int& head)
{
    if (head >= bufferSize)
    {
        head -= bufferSize;
    }
}

void DelayLine::tick()
{
    increaseWriteHead();
    increaseReadHead();
}

// based on circbuffer by Ciska Vriezinga
void DelayLine::setDistanceReadWriteHead(unsigned int distance)
{
    distanceReadWriteHead = distance;
    readHead = writeHead - distanceReadWriteHead + bufferSize;
    wrapHead(readHead);
}

unsigned int DelayLine::getDistanceReadWriteHead()
{
    return distanceReadWriteHead;
}
