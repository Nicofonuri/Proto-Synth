
#include <juce_audio_basics/juce_audio_basics.h>

class Sine
{
public:
	void prepare(double sampleRate, int numChannels);
	void process(juce::AudioBuffer<float>& buffer);

private:

	float amplitude = 0.3f;
	float frequency = 440.0f;
	float currentSampleRate = 0.0f;
	float timeIncrement = 0.0f;
	std::vector<float> currentTime;

};