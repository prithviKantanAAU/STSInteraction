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
	

	// MUSIC
	ComboBox tonic;
	ComboBox scale;
	ComboBox chord_Degree[8];
	ComboBox chord_Type;

	// VOICE CUES
	ToggleButton voiceCue_Enable;
	Label voiceCue_Enable_Lab;
	Slider voiceCue_Level;
	Label voiceCue_Level_Lab;
	Slider voiceCue_CountLength;
	Label voiceCue_CountLength_Lab;
	Label voiceCue_ColHeaders[3];
	ToggleButton voiceCue_intervalEnable[8];
	ToggleButton voiceCue_isPosCrossing[8];
	Slider voiceCue_OscTrig_Location[8];
	Slider voiceCue_FineTimeAdjust;
	Label voiceCue_FineTimeAdjust_Lab;


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

		

		// Voice Cue
		voiceCue_Enable_Lab.setText("Voice Cue", dontSendNotification);
		voiceCue_Enable_Lab.attachToComponent(&voiceCue_Enable, true);

		voiceCue_Level.setRange(-100, 0);
		voiceCue_Level.setValue(-6);
		voiceCue_Level.setColour(voiceCue_Level.trackColourId, Colours::yellow);
		voiceCue_Level.setColour(voiceCue_Level.backgroundColourId, Colours::blue);
		voiceCue_Level.setTextBoxStyle(Slider::NoTextBox,true,10,10);
		voiceCue_Level_Lab.setText("Volume", dontSendNotification);
		voiceCue_Level_Lab.attachToComponent(&voiceCue_Level, true);

		voiceCue_CountLength.setRange(1, 8, 1);
		voiceCue_CountLength.setValue(4);
		voiceCue_CountLength.setColour(voiceCue_CountLength.trackColourId, Colours::yellow);
		voiceCue_CountLength.setColour(voiceCue_CountLength.backgroundColourId, Colours::blue);
		voiceCue_CountLength.setTextBoxStyle(Slider::NoTextBox, true, 10, 10);
		voiceCue_CountLength_Lab.setText("Length", dontSendNotification);
		voiceCue_CountLength_Lab.attachToComponent(&voiceCue_CountLength, true);

		voiceCue_ColHeaders[0].setText("Enable", dontSendNotification);
		voiceCue_ColHeaders[1].setText("+/-", dontSendNotification);
		voiceCue_ColHeaders[2].setText("Location", dontSendNotification);

		for (int i = 0; i < 8; i++)
		{
			voiceCue_OscTrig_Location[i].setRange(0, 1, 0.125);
			voiceCue_OscTrig_Location[i].setValue(0.5);
			voiceCue_OscTrig_Location[i].setColour(voiceCue_OscTrig_Location[i].trackColourId, Colours::yellow);
			voiceCue_OscTrig_Location[i].setColour(voiceCue_OscTrig_Location[i].backgroundColourId, Colours::blue);
			voiceCue_OscTrig_Location[i].setTextBoxStyle(Slider::NoTextBox, true, 10, 10);
		}

		voiceCue_FineTimeAdjust.setRange(-0.125, 0.125);
		voiceCue_FineTimeAdjust.setValue(0);
		voiceCue_FineTimeAdjust.setColour(voiceCue_FineTimeAdjust.trackColourId, Colours::yellow);
		voiceCue_FineTimeAdjust.setColour(voiceCue_FineTimeAdjust.backgroundColourId, Colours::blue);
		voiceCue_FineTimeAdjust.setTextBoxStyle(Slider::NoTextBox, true, 10, 10);
		voiceCue_FineTimeAdjust_Lab.setText("Timing Fine", dontSendNotification);
		voiceCue_FineTimeAdjust_Lab.attachToComponent(&voiceCue_FineTimeAdjust, true);
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

		// Voice Cue
		voiceCue_Enable.setVisible(on);
		voiceCue_Enable_Lab.setVisible(on);
		voiceCue_Level.setVisible(on);
		voiceCue_Level_Lab.setVisible(on);
		voiceCue_CountLength.setVisible(on);
		voiceCue_CountLength_Lab.setVisible(on);
		for (int i = 0; i < 3; i++) voiceCue_ColHeaders[i].setVisible(on);
		for (int i = 0; i < 8; i++)
		{
			voiceCue_intervalEnable[i].setVisible(on);
			voiceCue_isPosCrossing[i].setVisible(on);
			voiceCue_OscTrig_Location[i].setVisible(on);
		}
		voiceCue_FineTimeAdjust.setVisible(on);
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

		//triOsc_BPM.setBounds(450, 160, 390, 25);

		// Voice Cue
		voiceCue_Enable.setBounds(450, 190, 250, 25);
		voiceCue_Level.setBounds(450, 220, 150, 25);
		voiceCue_CountLength.setBounds(450, 250, 250, 25);
		for (int i = 0; i < 3; i++) voiceCue_ColHeaders[i].setBounds(430 + i * 100, 285, 120, 15);
		for (int i = 0; i < 3; i++)
		{
			voiceCue_intervalEnable[i].setBounds(440, 315 + 27*i, 50, 25);
			voiceCue_isPosCrossing[i].setBounds(536, 315 + 27 * i, 50, 25);
			voiceCue_OscTrig_Location[i].setBounds(590, 315 + 27 * i, 250, 25);
		}
		voiceCue_FineTimeAdjust.setBounds(590, 190, 250, 25);

	}
};
