#pragma once
#include <JuceHeader.h>
#include "SineSound.h"
#include "Oscillator.h"
class SineVoice : public juce::SynthesiserVoice

{
public:
    SineVoice() 
    {
        sineLeft.prepare(getSampleRate());
        sineCenter.prepare(getSampleRate());
        sineRight.prepare(getSampleRate());
    }
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

        float upperDetunedMidi = midiNoteNumber + detuneValue;
        float lowerDetuneMidi = midiNoteNumber - detuneValue;

        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

        sineLeft.setFrequency(midiToFreq(lowerDetuneMidi));
        sineCenter.setFrequency(midiToFreq((float)midiNoteNumber));
        sineRight.setFrequency(midiToFreq(upperDetunedMidi));

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
        /*if (angleDelta != 0.0)
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
        }*/

        int numChans = outputBuffer.getNumChannels();
        int numSamps = outputBuffer.getNumSamples();

        for (int chan = 0; chan < numChans; chan++)
        {
            //Canal izquierdo, genera sine izquierdo y 0.5 del centro
            if (chan == 0)
            {
                for (int samp = 0; samp < numSamps; samp++)
                {
                    float sample = (sineLeft.generate() + sineCenter.generate() * 0.66) / 2.0;
                    outputBuffer.setSample(chan, samp, sample);
                }
            }

            //Canal derecho, genera sine derecho y 0.5 del centro
            if (chan == 1)
            {
                for (int samp = 0; samp < numSamps; samp++)
                {
                    float sample = (sineRight.generate() + sineCenter.generate() * 0.66) / 2.0;
                    outputBuffer.setSample(chan, samp, sample);
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
    Oscillator sineLeft;
    Oscillator sineCenter;
    Oscillator sineRight;
    float detuneValue = 0.1;

    float midiToFreq(float midi)
    {
        float f = 440.0 * std::pow(2.0, (midi - 69.0) / 12.0);
        return f;
    }

};


