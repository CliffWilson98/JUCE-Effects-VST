/*
  ==============================================================================

    Delay.h
    Created: 8 Jan 2019 7:09:15pm
    Author:  Cliff

  ==============================================================================
*/
#include "PluginProcessor.h"

#pragma once
class Delay
{
public:
	
	//void incrementBuffer();
	void incrementBuffer(int numberToIncrement);

	//float readFromBuffer();
	void readFromBuffer(AudioBuffer<float>& buffer, int destinationChannel, const float* source, int numSamples);

	//void writeToBuffer(float sampleValue);
	void writeToBuffer(int destinationChannel, const float* source, int numSamples);

	Delay(int numberOfInputChannels, int sampleRate);
	~Delay();

private:

	//AudioBuff circularBuffer[66150];
	AudioBuffer<float> circularBuffer;

	int bufferPosition;
	int writePosition;

	int sampleRate;
};
