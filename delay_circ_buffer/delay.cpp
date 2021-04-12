#include <iostream>
#include "delay.h"

Delay::Delay(uint size, int numSamplesDelay, float sampleRate, float feedback = 0.5) :
        delayLine(size), size(size), sampleRate(sampleRate), feedback(feedback)
{
    delayLine.setDistanceRW(numSamplesDelay);
}

Delay::~Delay(){

}

// methods

// proces input and output and tick to next sample
void Delay::process(float* inBuf, float* outBuf, uint frames){
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
    int numSamplesDelayLine = sampleRate * delayLineTime;
    std::cout << "numSamplesDelayLine: " << numSamplesDelayLine << "\n";

    if((delayLineTime > 0) && (delayLineTime < (size * 0.5))){
        this->delayLineTime = delayLineTime;
        std::cout << "delayLineTime: " << delayLineTime << "\n";

        delayLine.setDistanceRW(numSamplesDelayLine);
	}
	else {
		std::cout << "Sorry, this is not the correct value. I need an integer between 0 and " << size / sampleRate / 2 << "\n";
	}
}

void Delay::log(){
  delayLine.logAllSettings();
}
