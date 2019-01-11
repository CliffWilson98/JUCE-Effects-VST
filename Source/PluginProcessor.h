#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class AttemptAtVstAudioProcessor  : public AudioProcessor
{
public:

	float noteOnVel;

	float driveValue;

	float gain;
	int delayTime;

	bool useCleanDistort;

    //==============================================================================
    AttemptAtVstAudioProcessor();
    ~AttemptAtVstAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void performCleanDistortion(float* channelData);
	void performSloppyDistortion(float* channelData);
	void delayAttempt(float* channelData);

	void fillDelayBuffer(int channel, int bufferLength, int delayBufferLength, const float* bufferData, const float* delayBufferData);
	void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength,
		const float* bufferData, const float* delayBufferData);
	void feedBackDelay(int channel, const int bufferLength, const int delayBufferLength, float* dryBuffer);

private:

	//the delayBuffer is necessary to store a copy of the original audio buffer for playback at a later time
	//to obtain a delayed effect
	AudioBuffer<float> delayBuffer;

	//the writePosition will be used to keep track of where to write in the delayBuffer
	int mWritePosition{ 0 };

	//the sampleRate of the project is needed for delay calculations
	int mSampleRate{ 44100 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttemptAtVstAudioProcessor)
};
