#include "delayLine.h"
#include "jack_module.h"
#include <iostream>
#include <cstring>

// constructors and destructor
DelayLine::DelayLine() {}

DelayLine::DelayLine(uint size)
: m_size(size),  m_readH(0), m_writeH(0)
{
	allocateBuffer();
}

DelayLine::~DelayLine()
{
	releaseBuffer();
}

// methods
void DelayLine::setSize(uint size)
{
	m_size = size;
	releaseBuffer();
	allocateBuffer();
}

void DelayLine::allocateBuffer()
{
	// allocate buffer and set all samplesinput to 0
	m_buffer = (float*)malloc(m_size * sizeof(float));
	memset(m_buffer, 0, m_size * sizeof(float));
}

void DelayLine::releaseBuffer()
{
  // free data allocated with memset
  free(m_buffer);
}

// setters and getters
void DelayLine::setDistanceRW(uint distanceRW)
{
  // store new distance between R & W heads and update rhead position
	m_distanceRW = distanceRW;
  m_readH = m_writeH - m_distanceRW + m_size;
  wrapH(m_readH);
}

uint DelayLine::getDistanceRW() {
  return m_distanceRW;
}

// logging methods
void DelayLine::logRWPos()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "Read head position: " << m_readH << "\n";
	std::cout << "Write head position: " << m_writeH << "\n";
	std::cout << "_________________________________________________________________\n";
}

void DelayLine::logDistanceRW()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "Distance between read and write head: " << m_distanceRW << "\n";
	std::cout << "_________________________________________________________________\n";
}

void DelayLine::logSize()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "Buffer size: " << m_size << "\n";
	std::cout << "_________________________________________________________________\n";
}

void DelayLine::logAllSettings()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "Read head position: " << m_readH << "\n";
	std::cout << "Write head position: " << m_writeH << "\n";
  std::cout << "Distance between read and write head: " << m_distanceRW << "\n";
  std::cout << "Buffer size: " << m_size << "\n";
	std::cout << "_________________________________________________________________\n";
}

void DelayLine::logAllValues()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "DelayLine - buffer contains: \n";
	for (unsigned int i = 0; i < m_size - 1; i++)
	{
		std::cout << m_buffer[i] << ", ";
	}
	std::cout << m_buffer[m_size - 1] << "\n";
	std::cout << "_________________________________________________________________\n";
}
