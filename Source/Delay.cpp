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
	circularBuffer.setSize(numberOfInputChannels, 661500);
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

void Delay::incrementBuffer(int numberToIncrement)
{
	bufferPosition = (bufferPosition + numberToIncrement) % 66150;
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

void Delay::writeToBuffer(int destinationChannel, const float* source, int numSamples)
{
	DBG("Sample Rate is: " << sampleRate);
	circularBuffer.copyFrom(destinationChannel, bufferPosition, source, numSamples);
}

void Delay::readFromBuffer(AudioBuffer<float>& buffer, int destinationChannel, const float* source, int numSamples)
{
	int delayTime = 1500;
	int delayBufferLength = circularBuffer.getNumSamples();

	int readPosition;


	//buffer.addFrom(destinationChannel, bufferPosition,  )
	incrementBuffer(numSamples);
}

