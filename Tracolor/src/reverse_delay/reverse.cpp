#include "reverse.h"
#include <iostream>
#include <cstring>

// constructors and destructor
Reverse::Reverse(uint size, float sampleRate)
: m_size(size), m_readH(size), m_writeH(0), sampleRate(sampleRate)
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

// formula found in this paper https://ccrma.stanford.edu/~jingjiez/portfolio/echoing-harmonics/pdfs/A%20Pitch%20Shifting%20Reverse%20Echo%20Audio%20Effect.pdf
float Reverse::gain(){
	float gain;
	float delayMax = getSize();
	float distance = getDistanceRW();


	gain = 4 * distance/delayMax * (1 - distance/delayMax);

	return gain;
}


