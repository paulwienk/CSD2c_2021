//
// Created by steven on 4/27/21.
//

#ifndef REVDELAYREMAKE_DELAY_H
#define REVDELAYREMAKE_DELAY_H
#include "delayLine.h"

class delay {
public:
    void process();

private:
    DelayLine delayLine;

};


#endif //REVDELAYREMAKE_DELAY_H
