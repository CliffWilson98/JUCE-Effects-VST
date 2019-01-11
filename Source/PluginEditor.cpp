#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AttemptAtVstAudioProcessorEditor::AttemptAtVstAudioProcessorEditor (AttemptAtVstAudioProcessor& p) : AudioProcessorEditor (&p), processor (p)
{
    setSize (500, 200);

	driveKnob.setSliderStyle(Slider::Rotary);
	driveKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
	driveKnob.setRange(1.0, 100.0, 1.0);
	driveKnob.addListener(this);
	driveKnob.setPopupDisplayEnabled(true, false, this);
	driveKnob.setValue(processor.driveValue);
	addAndMakeVisible(&driveKnob);

	delayTimeKnob.setSliderStyle(Slider::Rotary);
	delayTimeKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
	delayTimeKnob.setRange(1, 1500, 1.0);
	delayTimeKnob.addListener(this);
	delayTimeKnob.setPopupDisplayEnabled(true, false, this);
	delayTimeKnob.setValue(processor.delayTime);
	delayTimeKnob.setTextValueSuffix(" ms");
	addAndMakeVisible(&delayTimeKnob);

	delayVolumeKnob.setSliderStyle(Slider::Rotary);
	delayVolumeKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
	delayVolumeKnob.setRange(0, 1, .01);
	delayVolumeKnob.addListener(this);
	delayVolumeKnob.setPopupDisplayEnabled(true, false, this);
	delayVolumeKnob.setValue(processor.gain);
	addAndMakeVisible(&delayVolumeKnob);

	addAndMakeVisible(cleanButton);
	cleanButton.onClick = [this] {toggleButtonManager(&cleanButton); };
	cleanButton.setRadioGroupId(GenderButtons);
	cleanButton.setToggleState(true, false);

	addAndMakeVisible(sloppyButton);
	sloppyButton.onClick = [this] {toggleButtonManager(&sloppyButton); };
	sloppyButton.setRadioGroupId(GenderButtons);

}

AttemptAtVstAudioProcessorEditor::~AttemptAtVstAudioProcessorEditor()
{
}

void AttemptAtVstAudioProcessorEditor::toggleButtonManager(ToggleButton* button)
{
	if (button == &cleanButton)
	{
		processor.useCleanDistort = true;
	}
	else if (button == &sloppyButton)
	{
		processor.useCleanDistort = false;
	}
}

//==============================================================================
void AttemptAtVstAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (30.0f);
	g.drawText("Drive", 50, 100, 100, 100, Justification::centred, true);
	g.drawText("Delay", 250, 100, 100, 100, Justification::centred, true);
	g.drawText("Volume", 350, 100, 100, 100, Justification::centred, true);
}

void AttemptAtVstAudioProcessorEditor::resized()
{
	driveKnob.setBounds(50, 50, 100, 100);

	cleanButton.setBounds(150, 50, 100, 25);
	sloppyButton.setBounds(150, 75, 100, 25);

	delayTimeKnob.setBounds(250, 50, 100, 100);

	delayVolumeKnob.setBounds(350, 50, 100, 100);
}


void AttemptAtVstAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &driveKnob)
	{
		processor.driveValue = driveKnob.getValue();
	}
	if (slider == &delayTimeKnob)
	{
		processor.delayTime = delayTimeKnob.getValue();
	}
	if (slider == &delayVolumeKnob)
	{
		processor.gain = delayVolumeKnob.getValue();
	}

}
