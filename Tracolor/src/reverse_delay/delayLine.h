#ifndef REVDELAYREMAKE_DELAYLINE_H
#define REVDELAYREMAKE_DELAYLINE_H

#include <iostream>

using namespace std;

class DelayLine{
public:
    // constructor and destructor
    DelayLine(unsigned int size);

    ~DelayLine();

    // methods
    void allocateBuffer();

    void releaseBuffer();

    void increaseReadHead();

    void increaseWriteHead();

    void decreaseReadReverseHead();

    void wrapHead(unsigned int& head);

    void wrapReverseReadHead(unsigned int& head);

    void tick();

    void reverseTick();

    void write(float value);

    float read();

    // setters and getters
    void setBufferSize(unsigned int size);

    void setDistanceReadWriteHead(unsigned int distance);

    unsigned int getDistanceReadWriteHead();

private:
    // variables
    float* buffer = nullptr;
    unsigned int bufferSize;
    unsigned int readHead;
    unsigned int readReverseHead;
    unsigned int writeHead;
    unsigned int distanceReadWriteHead;
};


#endif //REVDELAYREMAKE_DELAYLINE_H
