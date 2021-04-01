#include <iostream>
#include <thread>
#include "jack_module.h"
#include "math.h"
#include "delayLine.h"
#include "delay.h"
#include "reverse.h"

/*
TODO:
Write gain function - page 2
file:///home/steven/Downloads/A%20Pitch%20Shifting%20Reverse%20Echo%20Audio%20Effect.pdf
*/

// 10 seconds if samplerate = 44100inBuf[i]
#define MAX_DELAY_SIZE 441000
#define DELAY_TIME_SEC 4.0f
#define REVERSE_TIME_SEC 2.0f

#define PI_2 6.28318530717959

// Run program and set delaytime as argument (in seconds)

bool running = true;

int main(int argc,char **argv)
{
  // create a JackModule instance
  JackModule jack;

  // init the jack, use program name as JACK client name
  jack.init(argv[0]);
  float samplerate = jack.getSamplerate();

  // retrieve either default or console line argument delaytime
  float delayTimeSec = DELAY_TIME_SEC;
  float reverseTimeSec = REVERSE_TIME_SEC;
  if(argc >= 2) delayTimeSec = (float) atof(argv[1]);
  std::cout <<  "\nDelay time in seconds: " << delayTimeSec << "\n";

  // transform delay inBuf[frames]time in seconds to delay time in number of samples
  unsigned int numSamplesDelay = samplerate * delayTimeSec;
  unsigned int bufferSizeDelay = numSamplesDelay * 2;

  unsigned int numSamplesReverse = samplerate * reverseTimeSec;
  unsigned int bufferSizeReverse = numSamplesReverse * 2;

  // instantiate delay, 2x larger then delay time and set feedback/delay
  Delay delay(bufferSizeDelay, numSamplesDelay, samplerate, 0.7);
  delay.log();
  Reverse reverse(bufferSizeReverse, samplerate);

  //assign a function to the JackModule::onProces
  jack.onProcess = [&reverse, &delay](float *inBuf, float *outBuf, unsigned int nframes) {

    for(unsigned int i = 0; i < nframes; i++) {
      // process reversing input
      reverse.process(inBuf, outBuf, i);
      delay.proces(outBuf, outBuf, i);
    }
    return 0;
  };

  jack.autoConnect();

  //keep the program running and listen for user input, q = quit
  std::cout << "\n\nFunctionalities:"
            << "Press 'q' when you want to quit the program.\n"
            << "Press 'f' when you want to set the feedback (float value between 0 and 0.95).\n"
            << "Press 'd' when you want to set the delayTime (float value between 0 and " << (bufferSizeDelay/samplerate)/2 << ").\n";


  while (running)
  {
    switch (std::cin.get())
    {
      case 'q':
        running = false;
        jack.end();
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
