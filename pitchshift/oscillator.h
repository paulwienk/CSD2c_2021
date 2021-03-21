#pragma once

// Oscillator base class
class Oscillator {
public:
    Oscillator(double frequency, double sampleRate) : frequency(frequency), sampleRate(sampleRate) {}

    virtual ~Oscillator() {}

    //return the current sample
    float getSample();

    //getters and setters
    void setAmplitude(float);

    void setFrequency(float frequency);

    void setSampleRate(float sampleRate);

    virtual void tick() {}

protected:
    double sampleRate;
    double frequency;
    double amplitude = 1.0;
    double phase = 0.0;
    double sample = 0.0;

    double phaseIncrement = frequency / sampleRate;

    void updatePhase();
};
