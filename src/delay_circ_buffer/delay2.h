#pragma once

#include "circBuffer.h"
#include "jack_module.h"

class Delay{
public:
  // constructor/destructor
  Delay(int numSamplesDelay);
  ~Delay();

  // methods
  void proces(unsigned int frames, jack_default_audio_sample_t inBuf, jack_default_audio_sample_t outBuf);

  // getters and setters
  void setNumSamplesDelay(int numSamplesDelay);
  int getNumSamplesDelay();
private:
  // variables
  unsigned int frames;
  int numSamplesDelay;

  // has-a class
  CircBuffer* cBuffer;
};
