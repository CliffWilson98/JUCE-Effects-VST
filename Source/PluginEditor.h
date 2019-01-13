/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AttemptAtVstAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    AttemptAtVstAudioProcessorEditor (AttemptAtVstAudioProcessor&);
    ~AttemptAtVstAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void toggleButtonManager(ToggleButton* button);

private:
	void sliderValueChanged(Slider* slider) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AttemptAtVstAudioProcessor& processor;

	Slider midiVolume;

	Slider driveKnob;

	Slider delayTimeKnob;
	Slider delayVolumeKnob;

	ToggleButton cleanButton{ "Clean" };
	ToggleButton sloppyButton{ "Sloppy" };

	ToggleButton delayButton{ "On/Off" };

	enum RadioButtonIDs
	{
		GenderButtons = 1001
	};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttemptAtVstAudioProcessorEditor)
};
