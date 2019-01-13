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

	addAndMakeVisible(sloppyButton);
	sloppyButton.onClick = [this] {toggleButtonManager(&sloppyButton); };
	sloppyButton.setRadioGroupId(GenderButtons);

	if (processor.useCleanDistort)
	{
		cleanButton.setToggleState(true, false);
	}
	else
	{
		sloppyButton.setToggleState(true, false);
	}

	addAndMakeVisible(delayButton);
	delayButton.onClick = [this] {toggleButtonManager(&delayButton); };
	if (processor.delayOn)
	{
		delayButton.setToggleState(true, false);
	}


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
	else if (button == &delayButton)
	{
		processor.delayOn = !(processor.delayOn);
	}
}

//==============================================================================
void AttemptAtVstAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (24.0f);
	g.drawText("Distortion", 50, 0, 100, 50, Justification::centred, true);
	g.drawText("Drive", 50, 100, 100, 100, Justification::centred, true);

	g.drawText("Delay", 300, 0, 100, 50, Justification::centred, true);
	g.drawText("Time", 250, 100, 100, 100, Justification::centred, true);
	g.drawText("Volume", 350, 100, 100, 100, Justification::centred, true);
}

void AttemptAtVstAudioProcessorEditor::resized()
{
	driveKnob.setBounds(50, 50, 100, 100);

	cleanButton.setBounds(25, 150, 100, 50);
	sloppyButton.setBounds(125, 150, 100, 50);


	delayTimeKnob.setBounds(250, 50, 100, 100);

	delayVolumeKnob.setBounds(350, 50, 100, 100);

	delayButton.setBounds(400, 13, 100, 25);
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
