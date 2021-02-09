#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class UI_MusicControl
{
public:
	UI_MusicControl() 
	{
		configure();
	};

	~UI_MusicControl()
	{}
	
	// AUDIO PROCESSING
	TextButton toggle_DSP_OnOff;
	Slider gain_Master;
	Label gain_Master_LAB;
	Slider gain_Track[10];
	Label gain_Track_LAB[10];
	Label levelMeter[2];
	float levelMeter_Width = 150;
	Slider triOsc_BPM;
	Label triOsc_BPM_LAB;

	// MUSIC
	ComboBox tonic;
	ComboBox scale;
	ComboBox chord_Degree[8];
	ComboBox chord_Type;

	//MEMBER ELEMENTS
	// STEPS TO ADD NEW CONTROLS
	// Declare here
	// Add and make visible in PluginEditor.h (add to existing function)
	// Initialize here
	// Initialize in PluginEditor.h
	// Add SetVisible to ToggleVisible() here
	// Set position in SetLayout() here

	void configure()
	{
		// Start Music DSP - Off by Default
		toggle_DSP_OnOff.setColour(toggle_DSP_OnOff.buttonColourId, Colours::red);
		toggle_DSP_OnOff.setButtonText("Stop Music DSP");

		// Master Gain
		gain_Master.setRange(-96, 12);
		gain_Master.setValue(-6);
		gain_Master.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
		gain_Master.setColour(gain_Master.trackColourId, Colours::yellow);
		gain_Master.setColour(gain_Master.backgroundColourId, Colours::blue);
		gain_Master_LAB.setText("Master Volume", dontSendNotification);
		gain_Master_LAB.attachToComponent(&gain_Master,true);

		// Track Gain
		for (int i = 0; i < 10; i++)
		{
			gain_Track[i].setRange(-50, 30);
			gain_Track[i].setSkewFactor(2.5);
			gain_Track[i].setValue(0);
			gain_Track[i].setColour(gain_Track[i].trackColourId, Colours::yellow);
			gain_Track[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
			gain_Track_LAB[i].attachToComponent(&gain_Track[i], true);
		}

		// Level Meter
		levelMeter[0].setColour(levelMeter[0].backgroundColourId, Colours::yellow);
		levelMeter[1].setColour(levelMeter[1].backgroundColourId, Colours::blue);

		// Tri Osc BPM
		triOsc_BPM.setRange(40, 100);
		triOsc_BPM.setTextBoxStyle(Slider::NoTextBox,true,20,20);
		triOsc_BPM.setValue(60);
		triOsc_BPM.setColour(triOsc_BPM.trackColourId, Colours::yellow);
		triOsc_BPM.setColour(triOsc_BPM.backgroundColourId, Colours::blue);
		triOsc_BPM_LAB.setText("Osc BPM", dontSendNotification);
		triOsc_BPM_LAB.attachToComponent(&triOsc_BPM, true);
	}

	void toggleVisible(bool on)
	{
		toggle_DSP_OnOff.setVisible(on);
		gain_Master.setVisible(on);
		gain_Master_LAB.setVisible(on);
		for (int i = 0; i < 8; i++)
		{
			gain_Track[i].setVisible(on);
			gain_Track_LAB[i].setVisible(on);
		}
		tonic.setVisible(on);
		scale.setVisible(on);
		for (int i = 0; i < 8; i++)		chord_Degree[i].setVisible(on);
		chord_Type.setVisible(on);
		levelMeter[0].setVisible(on);
		levelMeter[1].setVisible(on);
		triOsc_BPM.setVisible(on);
	}

	void setLayout(int numTracks)
	{
		toggle_DSP_OnOff.setBounds(20, 50, 300, 25);
		gain_Master.setBounds(450, 50, 350, 25);
		for (int i = 0; i < numTracks; i++)
		{
			gain_Track[i].setBounds(70, 100 + 30 * i, 250, 25);
		}

		tonic.setBounds(350, 100, 110, 25);
		scale.setBounds(470, 100, 110, 25);
		chord_Type.setBounds(590, 100, 230, 25);

		for (int i = 0; i < 8; i++)
			chord_Degree[i].setBounds(350 + 60 * i, 130, 50, 25);

		triOsc_BPM.setBounds(450, 160, 350, 25);
	}
};
