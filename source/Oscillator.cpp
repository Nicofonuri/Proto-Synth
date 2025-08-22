
#include "Oscillator.h"

void Oscillator::setFrequency(float freq)
{
    frequency = freq;
    sampleSizeRadians = (frequency * twoPi) / sampleRate;
}

void Oscillator::prepare(float sr)
{
    sampleRate = sr;
    currentPhase = 0.f;
}

void Oscillator::resetPhase()
{
    currentPhase = 0.f;
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