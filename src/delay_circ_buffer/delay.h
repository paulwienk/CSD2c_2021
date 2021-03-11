#pragma once

#include "delayLine.h"

typedef unsigned int uint;

class Delay{
public:
  // constructor/destructor
  Delay(uint size, int numSamplesDelay, float samplerate, float feedback);
  ~Delay();

  // methods
  void proces(float* inBuf, float* outBuf, unsigned int frames);
  void log();


  // getters and setters
  void setNumSamplesDelay(int numSamplesDelay);
  int getNumSamplesDelay();
  void setFeedback(float feedback);
  void setDelayTime(float delayLineTime);

private:
  // has-a class
  DelayLine delayLine;

  // variables
  uint size;
  uint frames;
  int numSamplesDelay;
  float samplerate;
  float feedback;
  float delayLineTime;


};
