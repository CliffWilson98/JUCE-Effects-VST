/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//test 

//TODO make it where the number of input channels is not hardcoded
//Delay* testDelay;

//==============================================================================
AttemptAtVstAudioProcessor::AttemptAtVstAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	driveValue = 1.f;
	useCleanDistort = true;

	gain = .3;
	delayTime = 500;
}

AttemptAtVstAudioProcessor::~AttemptAtVstAudioProcessor()
{
}

//==============================================================================
const String AttemptAtVstAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AttemptAtVstAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AttemptAtVstAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AttemptAtVstAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AttemptAtVstAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AttemptAtVstAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AttemptAtVstAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AttemptAtVstAudioProcessor::setCurrentProgram (int index)
{
}

const String AttemptAtVstAudioProcessor::getProgramName (int index)
{
    return {};
}

void AttemptAtVstAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AttemptAtVstAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	//use this to update the delay buffer
	const int inputChannels = getTotalNumInputChannels();
	const int bufferSize =  2 * (sampleRate + samplesPerBlock);
	mSampleRate = sampleRate;

	delayBuffer.setSize(inputChannels, bufferSize);
	/*if (testDelay == nullptr)
	{
		testDelay = new Delay(inputChannels, sampleRate);
	}*/
	delayBuffer.clear();
}

void AttemptAtVstAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AttemptAtVstAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AttemptAtVstAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();

	//const int samples = buffer.getNumSamples();

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = delayBuffer.getNumSamples();

	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		float* channelData = buffer.getWritePointer(channel);

		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		float* dryBuffer = buffer.getWritePointer(channel);

		if(delayOn)
		{
			getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
			fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
		}
		
		//The distortion must occur after the delay or else an initially distorted signal will remained distorted 
		//during the delay even after the distortion is turned all the way down.
		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			if (useCleanDistort)
			{
				performCleanDistortion(channelData);
			}
			else
			{
				performSloppyDistortion(channelData);
			}

			//bitCrush(channelData);

			channelData++;
		}
	}

	mWritePosition += bufferLength;
	mWritePosition %= delayBufferLength;
}

void AttemptAtVstAudioProcessor::fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
	if (delayBufferLength > bufferLength + mWritePosition)
	{
		delayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, gain, gain);
	}
	else
	{
		int bufferRemaining = delayBufferLength - mWritePosition;
		delayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, gain, gain);
		delayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, gain, gain);
	}
}

void AttemptAtVstAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength,
	const float* bufferData, const float* delayBufferData)
{
	const int readPosition = static_cast<int>(delayBufferLength + mWritePosition - (mSampleRate * delayTime / 1000)) % delayBufferLength;

	//originally addFrom
	if (delayBufferLength > bufferLength + readPosition)
	{
		buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferLength);
	}
	else
	{
		const int bufferRemaining = delayBufferLength - readPosition;
		buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
		buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
	}
}

void AttemptAtVstAudioProcessor::performCleanDistortion(float* channelData)
{
	*channelData *= driveValue;

	*channelData = (2.f / float_Pi) * atan(*channelData);
	//formula for phase shift 
	//*channelData = sin(*channelData + 22);
}


void AttemptAtVstAudioProcessor::performSloppyDistortion(float* channelData)
{
	*channelData *= driveValue;

	if (*channelData > 1)
	{
		*channelData = 1;
	}
	else if (*channelData < -1)
	{
		*channelData = -1;
	}
}

void AttemptAtVstAudioProcessor::bitCrush(float* channelData)
{
	//DBG("initial value is: " << *channelData);

	*channelData = (ceil(*channelData * bitCrushValue)) / bitCrushValue;

	//DBG("final value is: " << *channelData);
}

//==============================================================================
bool AttemptAtVstAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AttemptAtVstAudioProcessor::createEditor()
{
    return new AttemptAtVstAudioProcessorEditor (*this);
}

//==============================================================================
void AttemptAtVstAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AttemptAtVstAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AttemptAtVstAudioProcessor();
}
