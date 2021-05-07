//
// PortAudio API wrapper by Wouter Ensink
//

#ifndef TRACOLOR_AUDIOMODULEADAPTER_H
#define TRACOLOR_AUDIOMODULEADAPTER_H

#include <iostream>
#include "port_audio.h"
#include "../pitch_shifter/pitcher.h"
#include "../reverse_delay/delay.h"

class AudioModuleAdapter : public AudioIODeviceCallback {
public:

    AudioModuleAdapter();

    ~AudioModuleAdapter();

    void prepareToPlay (int sampleRate, int blockSize) override;

    void process (float* input, float* output, int numSamples, int numChannels) override;

    void releaseResources() override;

    Pitcher pitcher;
};

#endif //TRACOLOR_AUDIOMODULEADAPTER_H
