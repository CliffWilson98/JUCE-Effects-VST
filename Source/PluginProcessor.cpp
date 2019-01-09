/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//test 
#include "Delay.h"

//TODO make it where the number of input channels is not hardcoded
Delay* testDelay;

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

	//DBG("Sample Rate: " << sampleRate);
	if (testDelay == nullptr)
	{
		testDelay = new Delay(inputChannels, sampleRate);
	}

	//this allows for approximately 1.5 seconds of audio storage with a sample rate of 44.1khz
	//const int bufferSize = 661500;
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

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		float* channelData = buffer.getWritePointer(channel);

		const int samples = buffer.getNumSamples();

		testDelay->writeToBuffer(channel, channelData, samples);

		/*for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			testDelay.writeToBuffer(*channelData);
			if (testDelay.readFromBuffer() != 0)
			{
				*channelData = testDelay.readFromBuffer();
			}
			//*channelData = testDelay.readFromBuffer();
			if (useCleanDistort)
			{
				performCleanDistortion(channelData);
			}
			else
			{
				performSloppyDistortion(channelData);
			}
			//delayAttempt(channelData);

			//DBG("Test of Circular Buffer: " << delayTest.readFromBuffer());

			channelData++;
		}*/
	}
}

void AttemptAtVstAudioProcessor::performCleanDistortion(float* channelData)
{
	*channelData *= driveValue;

	*channelData = (2.f / float_Pi) * atan(*channelData);
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

void AttemptAtVstAudioProcessor::delayAttempt(float* channelData)
{
	//float currentChannelData = *channelData;
	/*if ((channelData + 3) != nullptr)
	{

	}*/
	//*(channelData + 60000) = currentChannelData;
	//DBG("Current Channel Data " << *channelData);
	//DBG("Next Should Be " << *(channelData + 1));
	//*(channelData + 1) = .30;
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
