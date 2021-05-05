/* based on circBuffer by Ciska Vriezinga */

#include "delayLine.h"
#include <iostream>
#include <cstring>

// constructor and destructor
DelayLine::DelayLine(unsigned int size) :
bufferSize(size)
{
    allocateBuffer();
}

DelayLine::~DelayLine()
{
    releaseBuffer();
}

// methods
// based on circbuffer by Ciska Vriezinga
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

void DelayLine::decreaseReadReverseHead()
{
    readReverseHead--;
    wrapReverseReadHead(readReverseHead);
}

// based on circbuffer by Ciska Vriezinga
void DelayLine::wrapHead(unsigned int& head)
{
    if (head >= bufferSize)
    {
        head -= bufferSize;
    }
}

void DelayLine::wrapReverseReadHead(unsigned int& head)
{
    if (head == 0)
    {
        head += bufferSize - 1;
    }
}

void DelayLine::tick()
{
    increaseWriteHead();
    increaseReadHead();
}

void DelayLine::reverseTick()
{
    increaseWriteHead();
    decreaseReadReverseHead();
}

void DelayLine::write(float value)
{
    buffer[writeHead] = value;
}

float DelayLine::read()
{
    return buffer[readHead];
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


