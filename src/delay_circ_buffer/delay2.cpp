#include "delay.h"
#include "circBuffer.h"

Delay::Delay(int numSamplesDelay) :
numSamplesDelay(numSamplesDelay)
{
  // initialize values for circBuffer
  cBuffer->setDistanceRW(numSamplesDelay);
}

Delay::~Delay(){

}

// methods

// proces input and output and tick to next sample
void Delay::proces(unsigned int frames, float* inBuf, float* outBuf){
  // write input to delay
  cBuffer->write(inBuf[frames]);

  // read delayed output
  outBuf[frames] = cBuffer->read() * 0.5;

  // update delay --> next sample
  cBuffer->tick();
}

// CircBuffer Delay::initBuffer(int numSamplesDelay){
//
// }

// getters and setters
void Delay::setNumSamplesDelay(int numSamplesDelay){
  this->numSamplesDelay = numSamplesDelay;
}

int Delay::getNumSamplesDelay(){
  return numSamplesDelay;
}
