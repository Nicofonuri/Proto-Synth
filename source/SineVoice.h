#pragma once
#include <JuceHeader.h>
#include "SineSound.h"
class SineVoice : public juce::SynthesiserVoice

{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SineSound*> (sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.25; // controla volumen máximo
        tailOff = 0.0;

        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        angleDelta = frequency * 2.0 * juce::MathConstants<double>::pi / getSampleRate();
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 0.0)
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0.0)
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float)(std::sin(currentAngle) * level * tailOff);

                    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                        outputBuffer.addSample(channel, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;

                    tailOff *= 0.99;

                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float)(std::sin(currentAngle) * level);

                    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                        outputBuffer.addSample(channel, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

private:
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;
    double frequency = 440.0;
};


