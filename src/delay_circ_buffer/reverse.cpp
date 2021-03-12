#include "reverse.h"
#include "jack_module.h"
#include <iostream>
#include <cstring>

// constructors and destructor
Reverse::Reverse(uint size, float samplerate)
: m_size(size),  m_readH(size), m_writeH(0), samplerate(samplerate)
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
void Reverse::setDistanceRW(int distanceRW)
{
  // store new distance between R & W heads and update rhead position
	m_distanceRW = distanceRW;
  m_readH = m_writeH - m_distanceRW + m_size;
  wrapReadH(m_readH);
}

int Reverse::getDistanceRW() {
	m_distanceRW = m_writeH - m_readH;

	if (m_distanceRW < 0){
		m_distanceRW *= -1;
		return m_distanceRW;
	} else {
		return m_distanceRW;
	}
}

uint Reverse::getSize(){
	return m_size;
}

void Reverse::process(float* inBuf, float* outBuf, uint frames){
	// write input in buffer
	write(inBuf[frames]);

	// read delayed output
	outBuf[frames] = read() * gain();

  // update reverse --> next sample
  tick();


}

float Reverse::gain(){
	float gain;
	float delayMax = getSize();
	float distance = getDistanceRW();

	gain = 4 * distance/delayMax * (1 - distance/delayMax);

	return gain;
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
