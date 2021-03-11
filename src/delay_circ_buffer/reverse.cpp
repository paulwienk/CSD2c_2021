#include "reverse.h"
#include "jack_module.h"
#include <iostream>
#include <cstring>

// constructors and destructor
Reverse::Reverse(uint size, float samplerate, float feedback = 0.5)
: m_size(size),  m_readH(0), m_writeH(0), samplerate(samplerate), feedback(feedback)
{
	allocateBuffer();
}

Reverse::~Reverse()
{
	releaseBuffer();
}

// methods
void Reverse::resetSize(uint size)
{
	m_size = size;
	releaseBuffer();
	allocateBuffer();
}

void Reverse::allocateBuffer()
{
	// allocate buffer and set all samplesinput to 0
	m_buffer = (float*)malloc(m_size * sizeof(float));
	memset(m_buffer, 0, m_size * sizeof(float));
}

void Reverse::releaseBuffer()
{
  // free data allocated with memset
  free(m_buffer);
}

// setters and getters
void Reverse::setDistanceRW(uint distanceRW)
{
  // store new distance between R & W heads and update rhead position
	m_distanceRW = distanceRW;
  m_readH = m_writeH - m_distanceRW + m_size;
  wrapH(m_readH);
}

uint Reverse::getDistanceRW() {
  return m_distanceRW;
}

void Reverse::setFeedback(float f){
	if((f >= 0) && (f <= 0.95)){
		this->feedback = f;
	}
}

void Reverse::setReverseTime(float reverseTime){
	if((reverseTime >= 0) && (reverseTime < m_size)){
		this->reverseTime = reverseTime;
		int numSamplesReverse = samplerate * reverseTime;
		setDistanceRW(numSamplesReverse);
	}
	else {
		std::cout << "Sorry, this is not the correct value. I need an integer between 0 and " << m_size << "\n";
	}
}

// logging methods
void Reverse::logRWPos()
{
	std::cout << "\n______________________ Reverse ______________________________\n";
	std::cout << "Read head position: " << m_readH << "\n";
	std::cout << "Write head position: " << m_writeH << "\n";
	std::cout << "_________________________________________________________________\n";
}

void Reverse::logDistanceRW()
{
	std::cout << "\n______________________ Reverse ______________________________\n";
	std::cout << "Distance between read and write head: " << m_distanceRW << "\n";
	std::cout << "_________________________________________________________________\n";
}

void Reverse::logSize()
{
	std::cout << "\n______________________ Reverse ______________________________\n";
	std::cout << "Buffer size: " << m_size << "\n";
	std::cout << "_________________________________________________________________\n";
}

void Reverse::logAllSettings()
{
	std::cout << "\n______________________ Reverse ______________________________\n";
	std::cout << "Read head position: " << m_readH << "\n";
	std::cout << "Write head position: " << m_writeH << "\n";
  std::cout << "Distance between read and write head: " << m_distanceRW << "\n";
  std::cout << "Buffer size: " << m_size << "\n";
	std::cout << "_________________________________________________________________\n";
}

void Reverse::logAllValues()
{
	std::cout << "\n______________________ Reverse ______________________________\n";
	std::cout << "Reverse - buffer contains: \n";
	for (unsigned int i = 0; i < m_size - 1; i++)
	{
		std::cout << m_buffer[i] << ", ";
	}
	std::cout << m_buffer[m_size - 1] << "\n";
	std::cout << "_________________________________________________________________\n";
}
