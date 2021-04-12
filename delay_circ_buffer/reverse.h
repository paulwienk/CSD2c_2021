#pragma once

typedef unsigned int uint;

class Reverse {
public:
	/*
	 * size: the number of samples in the buffer
	 */

	Reverse(uint size, float sampleRate);
	~Reverse();

	void resetSize(uint size);

	// setter and getter for the distance between read and write head
	void setDistanceRW(int distanceRW);
    int getDistanceRW();
	uint getSize();

	// write and read values at write / read head
	inline void write(float val) { m_buffer[m_writeH] = val; }
	inline float read() { return m_buffer[m_readH]; }

    // method to set a step in time --> move to next sample
    inline void tick() {
    incrWriteH();
    incrReadH();
  }

	// process method
	void process(float* inBuf, float* outBuf, uint frames);

	float gain();

private:
    // increase write and read heads with 1 or more samples and wrap if necessary
    inline void incrWriteH() {
        m_writeH++;
        wrapWriteH(m_writeH);
    }

    inline void incrReadH() {
        m_readH--;
        wrapReadH(m_readH);
    }

    // wrap a head if necessary
    inline void wrapWriteH(uint& head) {
        if (head >= m_size) head -= m_size;
    }

    inline void wrapReadH(uint& head) {
        if (head == 0) head += m_size - 1;
    }

    // allocate and release methods --> are needed when altering the buffer size
    void allocateBuffer();
    void releaseBuffer();

    float* m_buffer;
    uint m_size;

    float* current_buffer;
    float* previous_buffer;
    // read and write heads, reverse size
    uint m_readH;
    uint m_writeH;
    int m_distanceRW;

    // variables
    float sampleRate;
    bool bufferFilled = false;
    uint count = 0;
};
