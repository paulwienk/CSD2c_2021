#include <iostream>
#include "delay.h"
#include "delayLine.h"


Delay::Delay(uint size, int numSamplesDelay, float samplerate, float feedback = 0.5) :
delayLine(size), size(size), samplerate(samplerate), feedback(feedback)
{
  delayLine.setDistanceRW(numSamplesDelay);
}

Delay::~Delay(){

}

// methods

// proces input and output and tick to next sample
void Delay::proces(float* inBuf, float* outBuf, uint frames){
  // write input to delay
  delayLine.write(inBuf[frames] + delayLine.read() * feedback);

  // read delayed output
  outBuf[frames] = delayLine.read() * 0.5;

  // update delay --> next sample
  delayLine.tick();
}

// getters and setters
void Delay::setNumSamplesDelay(int numSamplesDelay){
  // delayLine.setDistanceRW(numSamplesDelay);
}

int Delay::getNumSamplesDelay(){
  return numSamplesDelay;
}

void Delay::setFeedback(float feedback){
	if((feedback >= 0) && (feedback <= 0.95)){
		this->feedback = feedback;
	}
}

void Delay::setDelayTime(float delayLineTime){
	if((delayLineTime >= 0) && (delayLineTime < size)){
		this->delayLineTime = delayLineTime;
		int numSamplesDelayLine = samplerate * delayLineTime;
		delayLine.setDistanceRW(numSamplesDelayLine);
	}
	else {
		std::cout << "Sorry, this is not the correct value. I need an integer between 0 and " << size << "\n";
	}
}

void Delay::log(){
  delayLine.logAllSettings();
}
