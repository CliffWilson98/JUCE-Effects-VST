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

	addAndMakeVisible(cleanButton);
	cleanButton.onClick = [this] {toggleButtonManager(&cleanButton); };
	cleanButton.setRadioGroupId(GenderButtons);
	cleanButton.setToggleState(true, false);

	addAndMakeVisible(sloppyButton);
	sloppyButton.onClick = [this] {toggleButtonManager(&sloppyButton); };
	sloppyButton.setRadioGroupId(GenderButtons);

	addAndMakeVisible(alternativeButton);
	alternativeButton.onClick = [this] {toggleButtonManager(&alternativeButton); };
	alternativeButton.setRadioGroupId(GenderButtons);


	/*rangeKnob.setSliderStyle(Slider::Rotary);
	rangeKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
	rangeKnob.addListener(this);
	rangeKnob.setRange(0.0, 100.0, 1.0);
	rangeKnob.setPopupDisplayEnabled(true, false, this);
	rangeKnob.setTextValueSuffix(" Range");
	rangeKnob.setValue(processor.rangeValue);
	addAndMakeVisible(&rangeKnob);

	blendKnob.setSliderStyle(Slider::Rotary);
	blendKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
	blendKnob.addListener(this);
	addAndMakeVisible(&blendKnob);

	volumeKnob.setSliderStyle(Slider::Rotary);
	volumeKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
	volumeKnob.addListener(this);
	addAndMakeVisible(&volumeKnob);*/

	/*midiVolume.setSliderStyle(Slider::LinearBarVertical);
	midiVolume.setRange(1.0, 127.0, 1.0);
	midiVolume.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	midiVolume.setPopupDisplayEnabled(true, false, this);
	midiVolume.setTextValueSuffix(" Volume");
	midiVolume.setValue(1.0);
	midiVolume.addListener(this);
	addAndMakeVisible(&midiVolume);*/
}

AttemptAtVstAudioProcessorEditor::~AttemptAtVstAudioProcessorEditor()
{
}

/*void AttemptAtVstAudioProcessorEditor::updateToggleState(Button* button, String text)
{
	auto state = button->getToggleState();
	String stateString = state ? "ON" : "OFF";
	DBG("Changed to: " << stateString);
}*/

void AttemptAtVstAudioProcessorEditor::toggleButtonManager(ToggleButton* button)
{
	if (button == &cleanButton)
	{
		//DBG("clean button clicked");
		processor.useCleanDistort = true;
	}
	else if (button == &sloppyButton)
	{
		//DBG("sloppy button clicked");
		processor.useCleanDistort = false;
	}
	/*else if (button == &alternativeButton)
	{

	}*/
}

//==============================================================================
void AttemptAtVstAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (30.0f);
	g.drawText("Drive", 50, 100, 100, 100, Justification::centred, true);
}

void AttemptAtVstAudioProcessorEditor::resized()
{
	//midiVolume.setBounds(40, 30, 20, getHeight() - 60);
	
	//driveKnobTest.setBounds((getWidth() / 5 * 2) - (100 / 2), getHeight() / 2 - (100 / 2), 100, 100);
	
	driveKnob.setBounds(50, 50, 100, 100);
	cleanButton.setBounds(150, 50, 100, 25);
	sloppyButton.setBounds(150, 75, 100, 25);

	/*rangeKnob.setBounds((getWidth() / 5 * 2) - (100 / 2), getHeight() / 2 - (100 / 2), 100, 100);
	blendKnob.setBounds((getWidth() / 5 * 3) - (100 / 2), getHeight() / 2 - (100 / 2), 100, 100);
	volumeKnob.setBounds((getWidth() / 5 * 4) - (100 / 2), getHeight() / 2 - (100 / 2), 100, 100);*/
}


void AttemptAtVstAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &driveKnob)
	{
		processor.driveValue = driveKnob.getValue();
	}
}
