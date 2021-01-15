#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "UI_SensorConfig.h"
#include "UI_MovementAnalysis.h"
#include "UI_MusicControl.h"

class StsinteractionAudioProcessorEditor  : public AudioProcessorEditor, public ComboBox::Listener,
											public Timer
{
public:
    StsinteractionAudioProcessorEditor (StsinteractionAudioProcessor&);
    ~StsinteractionAudioProcessorEditor();
    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;

private:
    
	// MEMBER OBJECTS
	UI_SensorConfig ui_sensorCon;
	UI_MovementAnalysis ui_movementAnalysis;
	UI_MusicControl ui_musicControl;
    StsinteractionAudioProcessor& processor;
	std::unique_ptr<TabbedComponent> tabs;

	// INTERFACE PROPERTIES
	int interface_Width = 900;						// Pixels
	int interface_Height = 500;						// Pixels
	int UIRefreshFreq = 30;							// UI Real Time Refresh Frequency (Hz)
	short presentTab = 0;							// Global tab index
	int previousTab = 0;							// To detect changes
	void switchTab(int presentTab);					// Switch Tab if Necessary
	void comboBoxChanged(ComboBox *box) override;

	void addControls_ALL()
	{
		addControls_SensorConfig();
		addControls_MovementAnalysis();
		addControls_MusicControl();
	}
	
	// Add Sensor Config Tools
	void addControls_SensorConfig()
	{
		addAndMakeVisible(ui_sensorCon.IP_Header);
		addAndMakeVisible(ui_sensorCon.Status_Header);
		addAndMakeVisible(ui_sensorCon.Port_Header);
		addAndMakeVisible(ui_sensorCon.Location_Header);
		addAndMakeVisible(ui_sensorCon.BiasComp_Header);
		addAndMakeVisible(ui_sensorCon.BatteryLevel_Header);
		addAndMakeVisible(ui_sensorCon.PacketPercent_Header);
		for (int i = 0; i < 3; i++)
		{
			addAndMakeVisible(ui_sensorCon.WLAN_IP[i]);
			addAndMakeVisible(ui_sensorCon.Status[i]);
			addAndMakeVisible(ui_sensorCon.Port[i]);
			addAndMakeVisible(ui_sensorCon.Location[i]);
			addAndMakeVisible(ui_sensorCon.BiasComp[i]);
			addAndMakeVisible(ui_sensorCon.BatteryLevel[i]);
			addAndMakeVisible(ui_sensorCon.PacketPercent[i]);
		}
	}

	// Add Movement Analysis Controls
	void addControls_MovementAnalysis()
	{
		for (int i = 0; i < 3; i++)
		{
			addAndMakeVisible(ui_movementAnalysis.IMUOnline[i]);
			addAndMakeVisible(ui_movementAnalysis.IMUOrientations[i]);
		}

		for (int j = 0; j < 2; j++)		
			addAndMakeVisible(ui_movementAnalysis.JointAngles[j]);
		addAndMakeVisible(ui_movementAnalysis.STSPhasePresent);
	}

	// Add Music Control Controls
	void addControls_MusicControl()
	{

	}

	// Initialize Sensor Config Tab Elements
	void sensorConfig_initializeControls()
	{
		String statusText = "";
		for (int i = 0; i < 3; i++)
		{
			statusText = processor.movementAnalysis.sensorInfo.isOnline[i] ? "ON" : "OFF";
			ui_sensorCon.Status[i].setText(statusText, dontSendNotification);

			ui_sensorCon.Port[i].setText(String(processor.movementAnalysis.sensorInfo.UDP_Ports[i])
				, dontSendNotification);

			ui_sensorCon.Location[i].addListener(this);

			ui_sensorCon.BiasComp[i].onClick = [this, i]
			{
				if (!processor.movementAnalysis.sensors_OSCReceivers[i].isBiasComp_ON)
				{
					processor.movementAnalysis.sensors_OSCReceivers[i].isBiasComp_DONE = false;
					processor.movementAnalysis.sensors_OSCReceivers[i].isBiasComp_ON = true;
				}
			};

			ui_sensorCon.WLAN_IP[i].onTextChange = [this, i]
			{
				processor.movementAnalysis.sensorInfo.remoteIP[i] = ui_sensorCon.WLAN_IP[i].getText();
			};
		}
	}

	// Initialize Movement Analysis Elements
	void movementAnalysis_initializeControls()
	{
		movementAnalysis_updateLabels();				// FIRST TIME UPDATE
	}
	
	// Initialize Music Control Elements
	void musicControl_initializeControls()
	{
		
	}

	// Update Sensor Config Tab Elements
	void sensorConfig_updateLabels()
	{
		String text = "";
		int batteryLevel = 0;
		bool isSensorOnline = false;
		String batteryText = "";
		for (int i = 0; i < 3; i++)
		{
			text = processor.movementAnalysis.sensorInfo.isOnline[i] ? "ON" : "OFF";

			if (processor.movementAnalysis.sensorInfo.isOnline[i])
			{
				ui_sensorCon.WLAN_IP[i].setColour(ui_sensorCon.WLAN_IP[i].backgroundColourId, Colours::green);
				ui_sensorCon.WLAN_IP[i].setReadOnly(true);
			}
			else
			{
				ui_sensorCon.WLAN_IP[i].setColour(ui_sensorCon.WLAN_IP[i].backgroundColourId, Colours::red);
				ui_sensorCon.WLAN_IP[i].setReadOnly(false);
			}

			ui_sensorCon.Status[i].setText(text, dontSendNotification);
			isSensorOnline = processor.movementAnalysis.sensorInfo.isOnline[i];
			batteryLevel = processor.movementAnalysis.sensorInfo.batteryPercent[i];
			batteryText = isSensorOnline ? String(batteryLevel) : "N/A";
			ui_sensorCon.BatteryLevel[i].setText(batteryText, dontSendNotification);
			if (processor.movementAnalysis.sensorInfo.isOnline[i])
				ui_sensorCon.BiasComp[i].setVisible(true);
			else
				ui_sensorCon.BiasComp[i].setVisible(false);
			if (processor.movementAnalysis.sensors_OSCReceivers[i].isBiasComp_ON)
			{
				ui_sensorCon.BiasComp[i].setButtonText("Calibrating");
				ui_sensorCon.BiasComp[i].setColour(ui_sensorCon.BiasComp[i].buttonColourId, Colours::blue);
			}
			if (processor.movementAnalysis.sensors_OSCReceivers[i].isBiasComp_DONE)
			{
				ui_sensorCon.BiasComp[i].setButtonText("Calibrated");
				ui_sensorCon.BiasComp[i].setColour(ui_sensorCon.BiasComp[i].buttonColourId, Colours::green);
			}

			ui_sensorCon.PacketPercent[i].setText
			(
				String(processor.movementAnalysis.sensors_OSCReceivers[i].oscSampleReceived_Percent, 2),
				dontSendNotification
			);
		}
	};

	// Update Movement Analysis Tab Elements
	void movementAnalysis_updateLabels()
	{
		Colour colour_onlineIndicator;
		for (int i = 0; i < 3; i++)
		{
			// SET COLOUR OF ONLINE INDICATOR
			colour_onlineIndicator = processor.movementAnalysis.sensorInfo.isOnline[i] ? Colours::green : Colours::red;
			ui_movementAnalysis.IMUOnline[i].setColour(ui_movementAnalysis.IMUOnline[i].backgroundColourId, colour_onlineIndicator);

			// ORIENTATION ANGLE DISPLAYS
			ui_movementAnalysis.IMUOrientations[i].setText(
				ui_movementAnalysis.IMULocations[i] + " Orientation: "
				+ String(processor.movementAnalysis.orientation_Deg[i], 2),
				dontSendNotification
			);
		}

		// JOINT ANGLE DISPLAYS
		for (int j = 0; j < 2; j++)
			ui_movementAnalysis.JointAngles[j].setText(
				ui_movementAnalysis.JointNames[j] + " Angle: "
				+ String(processor.movementAnalysis.jointAngles_Deg[j], 2)
				, dontSendNotification
			);

		// STS PHASE DISPLAY
		ui_movementAnalysis.STSPhasePresent.setText(
			"Present Phase: " + processor.movementAnalysis.STS_Phases[processor.movementAnalysis.STS_Phase]
			, dontSendNotification
		);
	}

	// Update Music Control Tab Elements
	void musicControls_updateLabels()
	{

	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StsinteractionAudioProcessorEditor)
};
