
// by Paul Wienk and Steven van Esch, with help from Wouter Ensink

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "pitch_shifter/port_audio.h"
#include "pitch_shifter/saw.h"
#include "pitch_shifter/circBuffer.h"
#include "pitch_shifter/pitcher.h"
#include "reverse_delay/jackModuleAdapter.h"
#include "reverse_delay/delayLine.h"
#include "reverse_delay/delay.h"
//#include "reverse_delay/reverse.h"
#include "colorDetection.h"

using namespace cv;
using namespace std;

// 10 seconds if sampleRate = 44100inBuf[i]
constexpr auto MAX_DELAY_SIZE = 441000;
constexpr auto DELAY_TIME_SEC = 0.5f;
constexpr auto REVERSE_TIME_SEC = 1.0f;

// one main callback which calls the right process function
struct Callback : AudioIODeviceCallback {

    void prepareToPlay(int sampleRate, int numSamplesPerBlock) override {
        pitcher.prepareToPlay(sampleRate, numSamplesPerBlock);
        delay.prepareToPlay(sampleRate, numSamplesPerBlock);
    }

    void process(float* in, float* out, int numSamples, int numChannels) override {
        if (!currentState)
            pitcher.process(in, out, numSamples, numChannels);
        else
            delay.process(in, out, numSamples, numChannels);
    }

    JackModuleAdapter delay;
    NoiseTestCallback pitcher;
};


int main()
{
    // init portaudio audio callback
    const auto sampleRate = 44100;
    auto blockSize = 64;

    // retrieve either default or console line argument delaytime
    float delayTimeSec = DELAY_TIME_SEC;
    float reverseTimeSec = REVERSE_TIME_SEC;

    // transform delay inBuf[frames]time in seconds to delay time in number of samples
    unsigned int numSamplesDelay = sampleRate * delayTimeSec;
    unsigned int bufferSizeDelay = MAX_DELAY_SIZE * 2;

    unsigned int numSamplesReverse = sampleRate * reverseTimeSec;
    unsigned int bufferSizeReverse = numSamplesReverse * 2;

    // instantiate delay, 2x larger then delay time and set feedback/delay
    Delay delay(bufferSizeDelay, numSamplesDelay, sampleRate, 0.8);
//    Reverse reverse(bufferSizeReverse, sampleRate);

    Callback callback;

    callback.delay.callback = [&delay](float *inBuf, float *outBuf, unsigned int nFrames) {

        for(unsigned int i = 0; i < nFrames; i++) {
            // process reversing input
//            reverse.process(inBuf, outBuf, i);
            delay.process(inBuf, outBuf, i);

        }
        return 0;
    };

    PortAudio portAudio { callback };
    portAudio.setup(sampleRate, blockSize);

    VideoCapture cap(0);

    while (true)
    {
        cap.read(frame);
        findColor(frame);
//
        flip(frame, frame, 1);
        imshow("Image", frame);

//        // triggers the pitch shifter and sets frequency
//        if (!currentState)
//        {
//            callback.pitcher.pitcher.saw.setFrequency(greenY);
//        }
//
//        // triggers the reverse delay and sets feedback and delay time
//        if (currentState)
//        {
//            std::cout << "Current state: " << currentState << "\n";
////            delay.setFeedback(blueY);
////            delay.setDelayTime(blueX);
//        }

        // quit with escape button
        if (waitKey(30) == 27)
        {
            return 0;
        }
    }
}