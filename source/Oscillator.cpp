
#include "Oscillator.h"

void Oscillator::setFrequency(float freq)
{
    frequency = freq;
    sampleSize = frequency / sampleRate;
    sampleSizeRadians = sampleSize * twoPi;
}

void Oscillator::prepare(float sr)
{
    sampleRate = sr;
}

float Oscillator::getFrequency()
{
    return frequency;
}

float Oscillator::generate()
{
    float returnSample = 0.f;
    if (currentPhase > twoPi)
        currentPhase = currentPhase - twoPi;
    returnSample = std::sin(currentPhase);
    currentPhase += sampleSizeRadians;

    return returnSample;
}