#pragma once
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
        isNoteOn = true;

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

        clearCurrentNote();
        isNoteOn = false;
        level = 0.0;
        sineLeft.resetPhase();
        sineCenter.resetPhase();
        sineRight.resetPhase();

    }
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (isNoteOn)
        {
            for (int samp = startSample; samp < numSamples; ++samp)
            {
                float leftOsc = sineLeft.generate() * 0.5f;
                float centreOsc = sineCenter.generate() * 0.5f;
                float rightOsc = sineRight.generate() * 0.5f;

                float leftSample = leftOsc + 0.5f * centreOsc;
                float rightSample = rightOsc + 0.5f * centreOsc;

                outputBuffer.addSample(0, startSample + samp, leftSample * level);
                outputBuffer.addSample(1, startSample + samp, rightSample * level);
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

    bool isNoteOn = false;

    float midiToFreq(float midi)
    {
        float f = 440.0 * std::pow(2.0, (midi - 69.0) / 12.0);
        return f;
    }

};


