#include <iostream>
#include <thread>
#include "port_audio.h"
#include "jackModuleAdapter.h"
#include "math.h"
#include "delayLine.h"
#include "delay.h"
#include "reverse.h"

// 10 seconds if sampleRate = 44100inBuf[i]
#define MAX_DELAY_SIZE 441000
#define DELAY_TIME_SEC 0.5f
#define REVERSE_TIME_SEC 1.0f

#define PI_2 6.28318530717959

// Run program and set delaytime as argument (in seconds)

bool running = true;

int main(int argc,char **argv)
{
    // init portaudio audio callback
    const auto sampleRate = 44100;

    // retrieve either default or console line argument delaytime
    float delayTimeSec = DELAY_TIME_SEC;
    float reverseTimeSec = REVERSE_TIME_SEC;
    if(argc >= 2) delayTimeSec = (float) atof(argv[1]);
    std::cout <<  "\nDelay time in seconds: " << delayTimeSec << "\n";

    // transform delay inBuf[frames]time in seconds to delay time in number of samples
    unsigned int numSamplesDelay = sampleRate * delayTimeSec;
    unsigned int bufferSizeDelay = MAX_DELAY_SIZE * 2;

    unsigned int numSamplesReverse = sampleRate * reverseTimeSec;
    unsigned int bufferSizeReverse = numSamplesReverse * 2;

    // instantiate delay, 2x larger then delay time and set feedback/delay
    Delay delay(bufferSizeDelay, numSamplesDelay, sampleRate, 0.8);
    delay.log();
    Reverse reverse(bufferSizeReverse, sampleRate);

    JackModuleAdapter adapter;

    adapter.callback = [&reverse, &delay](float *inBuf, float *outBuf, unsigned int nframes) {

        for(unsigned int i = 0; i < nframes; i++) {
            // process reversing input
            reverse.process(inBuf, outBuf, i);
            delay.process(outBuf, outBuf, i);
        }
        return 0;
    };

    PortAudio portAudio { adapter };
    portAudio.setup(sampleRate, 64);

    //keep the program running and listen for user input, q = quit
    std::cout << "\n\nFunctionalities:"
            << "Press 'q' when you want to quit the program.\n"
            << "Press 'f' when you want to set the feedback (float value between 0 and 0.95).\n"
            << "Press 'd' when you want to set the delayTime (float value between 0 and " << MAX_DELAY_SIZE / sampleRate << ").\n";

    while (running)
    {
        switch (std::cin.get())
        {
            case 'q':
            running = false;
            break;
            case 'f':
            float newFeedback;
            std::cin >> newFeedback;
            delay.setFeedback(newFeedback);
            break;
            case 'd':
            float newDelayTime;
            std::cin >> newDelayTime;
            delay.setDelayTime(newDelayTime);
            break;
        }
    }

    //end the program
    return 0;
} // main()
