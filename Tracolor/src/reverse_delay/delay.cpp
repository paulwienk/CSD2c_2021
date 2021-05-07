//
// Created by steven on 4/27/21.
//

#include "delay.h"

Delay::Delay(uint bufferSize, uint numSamples, float sampleRate, float feedback) :
delayLine(bufferSize), bufferSize(bufferSize), numSamples(numSamples), sampleRate(sampleRate), feedback(feedback)
{
    delayLine.setDistanceReadWriteHead(numSamples);
}

Delay::~Delay()
{
    delayLine.releaseBuffer();
}

// TODO make stereo
void Delay::process(float* inBuffer, float* outBuffer, unsigned int frames)
{
    delayLine.write(inBuffer[frames] + delayLine.read() * feedback);

    outBuffer[frames] = delayLine.read();

    delayLine.tick();
}

// TODO add windowing
void Delay::setFeedback(float feedback)
{
    if((feedback >= 0.0) && (feedback < 1.0))
    {
        this->feedback = feedback;
    }
}

// TODO fill in function and
// TODO add windowing
void Delay::setDelayTime(float delayLineTime)
{
//    int numSamplesDelayLine = sampleRate * delayLineTime;
//
//    if((delayLineTime > 0) && (delayLineTime < (bufferSize * 0.5)))
//    {
//        this->delayLineTime = delayLineTime;
//
//        delayLine.setDistanceReadWriteHead(numSamplesDelayLine);
//    }
}