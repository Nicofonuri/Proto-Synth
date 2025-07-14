#include "JuceHeader.h"

class MidiProcessor

{
public:

	void process(MidiBuffer& midiMessages)
	{
		MidiBuffer::Iterator it(midiMessages);
		MidiMessage currentMessage;
		int samplePos;

		while (it.getNextEvent(currentMessage, samplePos))

		{ 
		currentMessage.getDescription();
		currentMessage.setNoteNumber(50);
		}

	}
};

