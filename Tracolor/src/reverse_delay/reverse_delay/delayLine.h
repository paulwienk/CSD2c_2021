#ifndef REVDELAYREMAKE_DELAYLINE_H
#define REVDELAYREMAKE_DELAYLINE_H

#include <iostream>

using namespace std;

class DelayLine{
public:
    // constructor and destructor
    DelayLine(unsigned int size, unsigned int distance, float sampleRate);

    ~DelayLine();

    // methods
    void allocateBuffer();

    void releaseBuffer();

    void increaseReadHead();

    void increaseWriteHead();

    void wrapHead(unsigned int& head);

    void tick();

    // setters and getters
    void setBufferSize(unsigned int size);

    void setDistanceReadWriteHead(unsigned int distance);

    unsigned int getDistanceReadWriteHead();

private:
    // variables
    float* buffer = nullptr;
    unsigned int bufferSize;
    unsigned int readHead;
    unsigned int writeHead;
    unsigned int distanceReadWriteHead;
};


#endif //REVDELAYREMAKE_DELAYLINE_H
