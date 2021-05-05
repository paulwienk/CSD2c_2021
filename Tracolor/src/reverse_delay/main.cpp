#include <iostream>
#include "delayLine.h"
#include "port_audio.h"
#include "jackModuleAdapter.h"

#define MAX_DELAY_SIZE 441000
#define DELAY_TIME_SEC 0.5f

int main() {
    const auto sampleRate = 44100;

    float delayTime = DELAY_TIME_SEC * sampleRate;
    float bufferSize = MAX_DELAY_SIZE;

    DelayLine delayLine(bufferSize, delayTime, sampleRate);

    JackModuleAdapter adapter;

    adapter.callback = [&delayLine](float *inBuffer, float *outBuffer, unsigned int nFrames) {

        for(unsigned int i = 0; i < nFrames; i++) {

        }
    };

    return 0;
}
