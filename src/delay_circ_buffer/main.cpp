#include <iostream>
#include <thread>
#include "jack_module.h"
#include "math.h"
#include "delay.h"


/*
 * NOTE: jack2 needs to be installed
 * jackd invokes the JACK audio server daemon
 * https://github.com/jackaudio/jackaudio.github.com/wiki/jackd(1)
 * on mac, you can start the jack audio server daemon in the terminal:
 * jackd -d coreaudio
 */

// 10 seconds if samplerate = 44100inBuf[i]
#define MAX_DELAY_SIZE 441000
#define DELAY_TIME_SEC 9.0f

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
  if(argc >= 2) delayTimeSec = (float) atof(argv[1]);
  std::cout <<  "\nDelay time in seconds: " << delayTimeSec << "\n";

  // transform delay time in seconds to delay time in number of samples
  unsigned int numSamplesDelay = samplerate * delayTimeSec;
  unsigned int bufferSize = numSamplesDelay * 2;
  std::cout << "\ninput is delay by " << numSamplesDelay << " number of samples\n";

  // instantiate delay, 2x larger then delay time and set feedback/delay
  Delay delay(bufferSize, samplerate, 0.7);
  delay.setDistanceRW(numSamplesDelay);
  delay.logAllSettings();

  //assign a function to the JackModule::onProces
  jack.onProcess = [&delay](jack_default_audio_sample_t *inBuf,
     jack_default_audio_sample_t *outBuf, jack_nframes_t nframes) {

    for(unsigned int i = 0; i < nframes; i++) {
      // write input to delay
      delay.write(inBuf[i]);
      // read delayed output
      outBuf[i] = delay.read() * 0.8;
      // update delay --> next sample
      delay.tick();
      // proces input and output for the delay

    }
    return 0;
  };

  jack.autoConnect();

  //keep the program running and listen for user input, q = quit
  std::cout << "\n\nFunctionalities:"
            << "Press 'q' when you want to quit the program.\n"
            << "Press 'f' when you want to set the feedback (float value between 0 and 0.95).\n"
            << "Press 'd' when you want to set the delayTime (float value between 0 and " << (bufferSize/samplerate)/2 << ").\n";


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
      case 'd':
        float newDelayTime;
        std::cin >> newDelayTime;
        delay.setDelayTime(newDelayTime);
    }
  }

  //end the program
  return 0;
} // main()
