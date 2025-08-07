    #include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    amplitudeSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    amplitudeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    amplitudeSlider.setRange(0.0f, 0.9f, 0.01f);

    addAndMakeVisible (amplitudeSlider);
        
    setSize (400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("MOCHI", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()

{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..


    amplitudeSlider.setBounds(getWidth() / 2 - 50, getHeight() / 2, 100, 100);

}
