
#define _USE_MATH_DEFINES

#pragma once

#include<iostream>
#include<cmath>
#include<math.h>

class Oscillator
{
public:
    Oscillator() {};
    ~Oscillator() {};

    void setFrequency(float freq);
    void prepare(float sampleRate);
    float getFrequency();
    float generate();

private:

    float sampleRate = 44100;
    float sampleSize = 0.f;
    float sampleSizeRadians = 0.f;
    float frequency = 0.f;
    float twoPi = 2.0 * M_PI;
    float currentPhase = 0.f;
};