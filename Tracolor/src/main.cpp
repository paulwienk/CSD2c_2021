
// by Paul Wienk and Steven van Esch, with help from Wouter Ensink

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "port_audio/port_audio.h"
#include "pitch_shifter/saw.h"
#include "pitch_shifter/circBuffer.h"
#include "pitch_shifter/pitcher.h"
#include "reverse_delay/jackModuleAdapter.h"
#include "reverse_delay/delayLine.h"
#include "reverse_delay/delay.h"
#include "port_audio/audioModuleAdapter.h"
#include "colorDetection.h"
//#include "reverse_delay/reverse.h"

using namespace cv;
using namespace std;

// 10 seconds if sampleRate = 44100inBuf[i]
constexpr auto MAX_DELAY_SIZE = 441000;
constexpr auto DELAY_TIME_SEC = 0.5f;
constexpr auto REVERSE_TIME_SEC = 1.0f;

// TODO make help function with instructions
// TODO make function for setting up colours with sliders
// TODO add motion detection class

//// TODO make callback a class
//// TODO put in separate file
//// one main callback which calls the right process function
//struct Callback : AudioIODeviceCallback {
//
//    void prepareToPlay(int sampleRate, int numSamplesPerBlock) override {
//        pitcher.prepareToPlay(sampleRate, numSamplesPerBlock);
//        delay.prepareToPlay(sampleRate, numSamplesPerBlock);
//    }
//
//    void process(float* in, float* out, int numSamples, int numChannels) override {
//        if (!currentState)
//            pitcher.process(in, out, numSamples, numChannels);
//        else
//            delay.process(in, out, numSamples, numChannels);
//    }
//
//    JackModuleAdapter delay;
//    AudioModuleAdapter pitcher;
//};

int main()
{
    // init portaudio audio callback
    const auto sampleRate = 44100;
    auto blockSize = 64;

    // retrieve either default or console line argument delayTime
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

    AudioModuleAdapter audioProcess;

    audioProcess.prepareToPlay(sampleRate, blockSize);

    audioProcess.process(float* in, float* out, int numSamples, int numChannels)
    {
        if (!currentState)
            pitcher.process(in, out, numSamples, numChannels);
        else
            delay.process(in, out, numSamples, numChannels);
    }

    callback.delay.callback = [&delay](float *inBuf, float *outBuf, unsigned int nFrames) {

        for(unsigned int i = 0; i < nFrames; i++) {
            // process reversing input
//            reverse.process(inBuf, outBuf, i);
//            delay.process(inBuf, outBuf, i);

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

        // triggers the pitch shifter and sets frequency
        if (!currentState)
        {
            callback.pitcher.pitcher.saw.setFrequency(greenY);
        }

        // triggers the reverse delay and sets feedback and delay time
        if (currentState)
        {
//            TODO uncomment when delay en pitcher process with callback is functioning as intended
//            delay.setFeedback(blueY);
//            delay.setDelayTime(blueX);
        }

        // quit with escape button
        if (waitKey(30) == 27)
        {
            return 0;
        }
    }
}