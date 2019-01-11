/*
  ==============================================================================

    Delay.cpp
    Created: 8 Jan 2019 7:09:15pm
    Author:  Cliff

  ==============================================================================
*/

#include "Delay.h"
Delay::Delay(int numberOfInputChannels, int sampleRate)
{
	circularBuffer.setSize(numberOfInputChannels, 88680);
	this->sampleRate = sampleRate;
	bufferPosition = 0;
	writePosition = 0;
}

Delay::~Delay() {};

/*void Delay::incrementBuffer()
{
	if (bufferPosition < 66149)
	{
		bufferPosition++;
	}
	else
	{
		bufferPosition = 0;
	}
}*/

void Delay::incrementBuffer(int numberToIncrementBy)
{
	bufferPosition = ((bufferPosition + numberToIncrementBy) % 88680);
	//circularBuffer.getNumSamples()
}

/*float Delay::readFromBuffer()
{
	//return circularBuffer[bufferPosition];
}*/

//does not incorporate feedback percentage yet
/*void Delay::writeToBuffer(float sampleValue)
{
	circularBuffer[bufferPosition] = sampleValue;
	incrementBuffer();
}*/

/*void Delay::writeToBuffer(int destinationChannel, const float* source, int numSamples)
{
	//if (circularBuffer.getNumSamples() > )
	circularBuffer.copyFrom(destinationChannel, bufferPosition, source, numSamples);
}*/

void Delay::writeToBuffer(int destinationChannel, int bufferLength, const float* bufferData)
{
	int delayBufferLength = circularBuffer.getNumSamples();

	if (delayBufferLength > bufferLength + bufferPosition)
	{
		circularBuffer.copyFromWithRamp(destinationChannel, bufferPosition, bufferData, bufferLength, 0.8, 0.8);
	}
	else
	{
		int bufferLeft = delayBufferLength - bufferPosition;
		circularBuffer.copyFromWithRamp(destinationChannel, bufferPosition, bufferData, bufferLeft, 0.8, 0.8);
		circularBuffer.copyFromWithRamp(destinationChannel, 0, bufferData + bufferLeft, bufferLength - bufferLeft, 0.8, 0.8);
	}
}

void Delay::readFromBuffer(AudioBuffer<float>& buffer, int destinationChannel, int numSamples)
{
	/*DBG("Reading from circular Buffer!");
	for (int i = 0; i < circularBuffer.getNumSamples(); i++)
	{
		float sample = circularBuffer.getSample(destinationChannel, i);
		DBG("Sample At: " << i << "Equals: " << sample);
	}*/
	int delayTime = 200;
	int circularBufferLength = circularBuffer.getNumSamples();

	//if this breaks then it is because of the bufferPosition variable I suspect
	int readPosition = (int)(circularBufferLength + bufferPosition - (sampleRate * delayTime / 1000)) % circularBufferLength;

	float* circularBufferData = circularBuffer.getWritePointer(destinationChannel);

	if (circularBufferLength > readPosition + numSamples)
	{
		buffer.addFrom(destinationChannel, 0, circularBufferData + bufferPosition, numSamples);
	}
	else
	{
		int bufferLeft = circularBufferLength - readPosition;
		buffer.addFrom(destinationChannel, 0, circularBufferData + bufferPosition, bufferLeft);
		buffer.addFrom(destinationChannel, bufferLeft, circularBufferData, numSamples - bufferLeft);
	}
}

