//
// Created by steven on 4/27/21.
//

#ifndef REVDELAYREMAKE_DELAY_H
#define REVDELAYREMAKE_DELAY_H
#include "delayLine.h"

typedef unsigned int uint;

class Delay {
public:
    Delay(uint bufferSize, uint numSamples, float sampleRate, float feedback);

    ~Delay();

    void process(float* inBuffer, float* outBuffer, unsigned int frames);

    void setFeedback(float feedback);

    void setDelayTime(float delayLineTime);

private:
    DelayLine delayLine;
    uint bufferSize;
    uint numSamples;
    uint frames;
    float sampleRate;
    float feedback;
    float delayLineTime;
};


#endif //REVDELAYREMAKE_DELAY_H
