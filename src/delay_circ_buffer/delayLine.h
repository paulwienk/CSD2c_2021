#pragma once

typedef unsigned int uint;

class DelayLine {
public:
	/*
	 * size: the number of samples in the buffer
	 */
	DelayLine();
	DelayLine(uint size);
	~DelayLine();

	// allocate and release methods --> are needed when altering the buffer size
	void allocateBuffer();
	void releaseBuffer();
	void setSize(uint size);

	// setter and getter for the distance between read and write head
	void setDistanceRW(uint distanceRW);
  uint getDistanceRW();

	// write and read values at write / read head
	inline void write(float val) {
		m_buffer[m_writeH] = val + read();
	}
	inline float read() {
		return m_buffer[m_readH];
	}

  // method to set a step in time --> move to next sample
  inline void tick() {
    incrWriteH();
    incrReadH();
  }

	// logging methods
	void logRWPos();
	void logDistanceRW();
	void logSize();
	void logAllSettings();
	void logAllValues();


private:
  // increase write and read heads with 1 or more samples and wrap if necessary
	inline void incrWriteH() {
		m_writeH++;
		wrapH(m_writeH);
	}

	inline void incrReadH() {
		m_readH++;
		wrapH(m_readH);
	}

  // wrap a head if necessary
  inline void wrapH(uint& head) {
    if (head >= m_size) head -= m_size;
  }

	float* m_buffer = nullptr;
	uint m_size;
	// read and write heads, delayLine size
	uint m_readH;
	uint m_writeH;
	uint m_distanceRW;
};
