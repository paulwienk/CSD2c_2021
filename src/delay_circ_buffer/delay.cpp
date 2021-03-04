#include "delay.h"
#include "jack_module.h"
#include <iostream>
#include <cstring>

// constructors and destructor
Delay::Delay(uint size, float samplerate, float feedback = 0.5)
: m_size(size),  m_readH(0), m_writeH(0), samplerate(samplerate), feedback(feedback)
{
	allocateBuffer();
}

Delay::~Delay()
{
	releaseBuffer();
}

// methods
void Delay::resetSize(uint size)
{
	m_size = size;
	releaseBuffer();
	allocateBuffer();
}

void Delay::allocateBuffer()
{
	// allocate buffer and set all samples to 0
	m_buffer = (float*)malloc(m_size * sizeof(float));
	memset(m_buffer, 0, m_size * sizeof(float));
}

void Delay::releaseBuffer()
{
  // free data allocated with memset
  free(m_buffer);
}

// setters and getters
void Delay::setDistanceRW(uint distanceRW)
{
  // store new distance between R & W heads and update rhead position
	m_distanceRW = distanceRW;
  m_readH = m_writeH - m_distanceRW + m_size;
  wrapH(m_readH);
}

uint Delay::getDistanceRW() {
  return m_distanceRW;
}

void Delay::setFeedback(float f){
	if((f >= 0) && (f <= 0.95)){
		this->feedback = f;
	}
}

void Delay::setDelayTime(float delayTime){
	if((delayTime >= 0) && (delayTime < m_size)){
		this->delayTime = delayTime;
		int numSamplesDelay = samplerate * delayTime;
		setDistanceRW(numSamplesDelay);
	}
	else {
		std::cout << "Sorry, this is not the correct value. I need an integer between 0 and " << m_size << "\n";
	}
}

// logging methods
void Delay::logRWPos()
{
	std::cout << "\n______________________ Delay ______________________________\n";
	std::cout << "Read head position: " << m_readH << "\n";
	std::cout << "Write head position: " << m_writeH << "\n";
	std::cout << "_________________________________________________________________\n";
}

void Delay::logDistanceRW()
{
	std::cout << "\n______________________ Delay ______________________________\n";
	std::cout << "Distance between read and write head: " << m_distanceRW << "\n";
	std::cout << "_________________________________________________________________\n";
}

void Delay::logSize()
{
	std::cout << "\n______________________ Delay ______________________________\n";
	std::cout << "Buffer size: " << m_size << "\n";
	std::cout << "_________________________________________________________________\n";
}

void Delay::logAllSettings()
{
	std::cout << "\n______________________ Delay ______________________________\n";
	std::cout << "Read head position: " << m_readH << "\n";
	std::cout << "Write head position: " << m_writeH << "\n";
  std::cout << "Distance between read and write head: " << m_distanceRW << "\n";
  std::cout << "Buffer size: " << m_size << "\n";
	std::cout << "_________________________________________________________________\n";
}

void Delay::logAllValues()
{
	std::cout << "\n______________________ Delay ______________________________\n";
	std::cout << "Delay - buffer contains: \n";
	for (unsigned int i = 0; i < m_size - 1; i++)
	{
		std::cout << m_buffer[i] << ", ";
	}
	std::cout << m_buffer[m_size - 1] << "\n";
	std::cout << "_________________________________________________________________\n";
}
