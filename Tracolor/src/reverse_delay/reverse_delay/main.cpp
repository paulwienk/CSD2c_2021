#include <iostream>
#include "delayLine.h"
#include "jackModuleAdapter.h"

#define MAX_DELAY_SIZE 441000
#define DELAY_TIME_SEC 0.5f

int main() {
    const auto sampleRate = 44100;

    float delayTime = DELAY_TIME_SEC * sampleRate;
    float bufferSize = MAX_DELAY_SIZE;

    DelayLine delayLine(bufferSize, delayTime, sampleRate);

    JackModuleAdapter adapter;

    adapter.callback = [&delayLine](float *inbuf, float *outbuf, unsigned int nframes) {

        for(unsigned int i = 0; i < nframes; i++) {
            delayLine
        }
    };

    return 0;
}
