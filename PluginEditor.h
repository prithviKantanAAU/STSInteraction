#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "UI_SensorConfig.h"
#include "UI_MovementAnalysis.h"
#include "UI_MusicControl.h"
#include "UI_MappingMatrix.h"

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
	UI_MappingMatrix ui_mappingMatrix;
    StsinteractionAudioProcessor& processor;
	std::unique_ptr<TabbedComponent> tabs;

	// SIMULATION KEYPRESSES
	KeyPress TrunkThigh_PlusMinus_RLUD[4];

	void simulation_HandleKeyPresses()
	{
		bool isKeyDown[4] = { false, false, false, false};
		isKeyDown[0] = TrunkThigh_PlusMinus_RLUD[0].isKeyCurrentlyDown(KeyPress::rightKey);
		isKeyDown[1] = TrunkThigh_PlusMinus_RLUD[1].isKeyCurrentlyDown(KeyPress::leftKey);
		isKeyDown[2] = TrunkThigh_PlusMinus_RLUD[2].isKeyCurrentlyDown(KeyPress::upKey);
		isKeyDown[3] = TrunkThigh_PlusMinus_RLUD[3].isKeyCurrentlyDown(KeyPress::downKey);

		for (int i = 0; i < 4; i++)
		{
			if (isKeyDown[i])
			{
				switch (i)
				{
				case 0:
					ui_movementAnalysis.simulation_OrientAngles[0].setValue(
						ui_movementAnalysis.simulation_OrientAngles[0].getValue() + 1.7
					);
					break;
				case 1:
					ui_movementAnalysis.simulation_OrientAngles[0].setValue(
						ui_movementAnalysis.simulation_OrientAngles[0].getValue() - 1.7
					);
					break;
				case 2:
					ui_movementAnalysis.simulation_OrientAngles[1].setValue(
						ui_movementAnalysis.simulation_OrientAngles[1].getValue() + 2.3
					);
					break;
				case 3:
					ui_movementAnalysis.simulation_OrientAngles[1].setValue(
						ui_movementAnalysis.simulation_OrientAngles[1].getValue() - 2.3
					);
					break;
				}
			}
		}
	}

	// INTERFACE PROPERTIES
	int interface_Width = 1300;						// Pixels
	int interface_Height = 650;						// Pixels
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
		addControls_MappingMatrix();
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
		{
			addAndMakeVisible(ui_movementAnalysis.JointAngles[j]);
			addAndMakeVisible(ui_movementAnalysis.JointVelocities[j]);
		}
		addAndMakeVisible(ui_movementAnalysis.STSPhasePresent);
		addAndMakeVisible(ui_movementAnalysis.record_MovementLog);
		addAndMakeVisible(ui_movementAnalysis.operationMode);
		addAndMakeVisible(ui_movementAnalysis.orientationAlgo);

		for (int i = 0; i < 3; i++)
		{
			addAndMakeVisible(ui_movementAnalysis.simulation_OrientAngles[i]);
			addAndMakeVisible(ui_movementAnalysis.simulation_OrientAngles_LAB[i]);
		}

		addAndMakeVisible(ui_movementAnalysis.range_Horiz);
		addAndMakeVisible(ui_movementAnalysis.range_Horiz_LAB);
		addAndMakeVisible(ui_movementAnalysis.range_Vert);
		addAndMakeVisible(ui_movementAnalysis.range_Vert_LAB);
		addAndMakeVisible(ui_movementAnalysis.range_TrunkAP);
		addAndMakeVisible(ui_movementAnalysis.range_TrunkAP_LAB);
		addAndMakeVisible(ui_movementAnalysis.thresh_AP_preStand);
		addAndMakeVisible(ui_movementAnalysis.thresh_AP_preStand_LAB);

		for (int i = 0; i < 3; i++)
		{
			addAndMakeVisible(ui_movementAnalysis.sensor_Axis[i]);
			addAndMakeVisible(ui_movementAnalysis.sensor_Invert[i]);
		}

		// STS VISUALIZER
		for (int i = 0; i < 4; i++)
			addAndMakeVisible(ui_movementAnalysis.stsAnim_joint[i]);
		for (int j = 0; j < 20; j++)
		{
			addAndMakeVisible(ui_movementAnalysis.stsAnim_trunk[j]);
			addAndMakeVisible(ui_movementAnalysis.stsAnim_thigh[j]);
			addAndMakeVisible(ui_movementAnalysis.stsAnim_shank[j]);
		}
	}

	// Add Music Control Controls
	void addControls_MusicControl()
	{
		addAndMakeVisible(ui_musicControl.gain_Master);
		addAndMakeVisible(ui_musicControl.gain_Master_LAB);
		for (int i = 0; i < processor.movementAnalysis.musicControl.mixerSettings.num_Tracks; i++)
		{
			addAndMakeVisible(ui_musicControl.gain_Track[i]);
			addAndMakeVisible(ui_musicControl.gain_Track_LAB[i]);
		}
		addAndMakeVisible(ui_musicControl.tonic);
		addAndMakeVisible(ui_musicControl.scale);
		for (int i = 0; i < 8; i++)
			addAndMakeVisible(ui_musicControl.chord_Degree[i]);
		addAndMakeVisible(ui_musicControl.chord_Type);
	}

	// Add Mapping Matrix Controls
	void addControls_MappingMatrix()
	{
		for (int i = 0; i < 20; i++)		// Columns
		{
			addAndMakeVisible(ui_mappingMatrix.labels_movementParams[i]);
			addAndMakeVisible(ui_mappingMatrix.labels_audioParams[i]);
			addAndMakeVisible(ui_mappingMatrix.mapping_Function[i]);
			addAndMakeVisible(ui_mappingMatrix.mapping_Polarity[i]);
			addAndMakeVisible(ui_mappingMatrix.mapping_QuantLevels[i]);
			addAndMakeVisible(ui_mappingMatrix.movementParams_Value[i][0]);
			addAndMakeVisible(ui_mappingMatrix.audioParams_Value[i][0]);
			addAndMakeVisible(ui_mappingMatrix.movementParams_Value[i][1]);
			addAndMakeVisible(ui_mappingMatrix.audioParams_Value[i][1]);

			for (int j = 0; j < 20; j++)	// Rows
			{
				addAndMakeVisible(ui_mappingMatrix.mapping_Matrix[i][j]);
				addAndMakeVisible(ui_mappingMatrix.mapping_Strength[i][j]);
			}
		}

		addAndMakeVisible(ui_mappingMatrix.preset_Save);
		addAndMakeVisible(ui_mappingMatrix.preset_ListLoad);
		addAndMakeVisible(ui_mappingMatrix.preset_ListLoad_LAB);
		addAndMakeVisible(ui_mappingMatrix.preset_Name);
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

		// Operation Mode - Sensor / Simulation
		ui_movementAnalysis.operationMode.addListener(this);
		for (int i = 0; i < processor.movementAnalysis.numOperationModes; i++)
			ui_movementAnalysis.operationMode.addItem(processor.movementAnalysis.OperationModes[i], i + 1);
		ui_movementAnalysis.operationMode.setSelectedId(processor.movementAnalysis.operationMode_Present);

		// Orientation Detection Algorithm - Madgwick / 6DOF Complementary Filter
		ui_movementAnalysis.orientationAlgo.addListener(this);
		for (int i = 0; i < processor.movementAnalysis.numOrientationAlgos; i++)
			ui_movementAnalysis.orientationAlgo.addItem(processor.movementAnalysis.OrientationAlgos[i], i + 1);
		ui_movementAnalysis.orientationAlgo.setSelectedId(processor.movementAnalysis.orientAlgo_Present);
		
		// Simulation Sliders, Sensor Invert, Axis
		for (int i = 0; i < 3; i++)
		{
			ui_movementAnalysis.simulation_OrientAngles[i].onValueChange = [this, i]
			{
				processor.movementAnalysis.setSimulationAngle
				(i, ui_movementAnalysis.simulation_OrientAngles[i].getValue());
			};

			ui_movementAnalysis.sensor_Axis[i].addListener(this);
			ui_movementAnalysis.sensor_Invert[i].addListener(this);
		}

		ui_movementAnalysis.range_Horiz.setMinValue(processor.movementAnalysis.range_horiz[0]);
		ui_movementAnalysis.range_Horiz.setMaxValue(processor.movementAnalysis.range_horiz[1]);
		ui_movementAnalysis.range_Horiz.onValueChange = [this]
		{
			processor.movementAnalysis.range_horiz[0] = ui_movementAnalysis.range_Horiz.getMinValue();
			processor.movementAnalysis.range_horiz[1] = ui_movementAnalysis.range_Horiz.getMaxValue();
		};

		ui_movementAnalysis.range_Vert.setMinValue(processor.movementAnalysis.range_vert[0]);
		ui_movementAnalysis.range_Vert.setMaxValue(processor.movementAnalysis.range_vert[1]);
		ui_movementAnalysis.range_Vert.onValueChange = [this]
		{
			processor.movementAnalysis.range_vert[0] = ui_movementAnalysis.range_Vert.getMinValue();
			processor.movementAnalysis.range_vert[1] = ui_movementAnalysis.range_Vert.getMaxValue();
		};

		ui_movementAnalysis.thresh_AP_preStand.setValue(processor.movementAnalysis.thresh_Stand_trunk_AP);
		ui_movementAnalysis.thresh_AP_preStand.onValueChange = [this]
		{
			processor.movementAnalysis.thresh_Stand_trunk_AP = ui_movementAnalysis.thresh_AP_preStand.getValue();
		};

		ui_movementAnalysis.range_TrunkAP.setMinValue(processor.movementAnalysis.movementParams[0].minVal);
		ui_movementAnalysis.range_TrunkAP.setMaxValue(processor.movementAnalysis.movementParams[0].maxVal);
		ui_movementAnalysis.range_TrunkAP.onValueChange = [this]
		{
			processor.movementAnalysis.setMovementLimits(
				ui_movementAnalysis.range_TrunkAP.getMinValue(),
				ui_movementAnalysis.range_TrunkAP.getMaxValue()
			);

			ui_movementAnalysis.simulation_OrientAngles[0].setRange(
				ui_movementAnalysis.range_TrunkAP.getMinValue(),
				ui_movementAnalysis.range_TrunkAP.getMaxValue()
			);
		};

		ui_movementAnalysis.record_MovementLog.onClick = [this]
		{
			if (!processor.is_Recording_MPLog)
			{
				processor.start_Recording_MPLog();
				ui_movementAnalysis.record_MovementLog.setButtonText("Stop Recording");
				ui_movementAnalysis.record_MovementLog.setColour(
					ui_movementAnalysis.record_MovementLog.buttonColourId, Colours::blue);
			}
			else
			{
				processor.stop_Recording_MPLog();
				ui_movementAnalysis.record_MovementLog.setButtonText("Record");
				ui_movementAnalysis.record_MovementLog.setColour(
					ui_movementAnalysis.record_MovementLog.buttonColourId, Colours::red);
			}
		};
	}
	
	// Initialize Music Control Elements
	void musicControl_initializeControls()
	{
		ui_musicControl.gain_Master.setValue(processor.movementAnalysis.musicControl.mixerSettings.masterGain);
		ui_musicControl.gain_Master.onValueChange = [this]
		{
			if (processor.isReady)
			processor.set_masterGain(ui_musicControl.gain_Master.getValue());
		};

		for (int i = 0; i < processor.movementAnalysis.musicControl.mixerSettings.num_Tracks; i++)
		{
			ui_musicControl.gain_Track[i].setValue(
				processor.movementAnalysis.musicControl.mixerSettings.gain_Track[i]);
			ui_musicControl.gain_Track_LAB[i].setText(
				processor.movementAnalysis.musicControl.mixerSettings.names_Tracks[i],
				dontSendNotification
			);

			ui_musicControl.gain_Track[i].onValueChange = [this,i]
			{
				if (processor.isReady)
				processor.set_trackFader
				(i, ui_musicControl.gain_Track[i].getValue());
			};
		}

		ui_musicControl.tonic.addListener(this);
		for (int i = 0; i < 12; i++)
		{
			ui_musicControl.tonic.addItem(
				processor.movementAnalysis.musicControl.musicInfoCompute.tonics_Names[i], i + 1);
		}
		ui_musicControl.tonic.setSelectedId(1);

		ui_musicControl.scale.addListener(this);
		for (int i = 0; i < processor.movementAnalysis.musicControl.musicInfoCompute.numScales; i++)
		{
			ui_musicControl.scale.addItem(
				processor.movementAnalysis.musicControl.musicInfoCompute.scales_Names[i], i + 1);
		}
		ui_musicControl.scale.setSelectedId(1);

		ui_musicControl.chord_Type.addListener(this);
		for (int i = 0; i < processor.movementAnalysis.musicControl.musicInfoCompute.numChordTypes; i++)
		{
			ui_musicControl.chord_Type.addItem(
				processor.movementAnalysis.musicControl.musicInfoCompute.chordTypes_Names[i], i + 1
			);
		}
		ui_musicControl.chord_Type.setSelectedId(1);

		for (int i = 0; i < 8; i++)
		{
			ui_musicControl.chord_Degree[i].addListener(this);
			for (int j = 1; j <= 8; j++)
				ui_musicControl.chord_Degree[i].addItem(String(j), j);
			ui_musicControl.chord_Degree[i].setSelectedId
			(processor.movementAnalysis.musicControl.musicInfoCompute.chord_degSequence[i]);
		}
	}

	// Initialize Mapping Matrix Elements
	void mappingMatrix_initializeControls()
	{
		for (int i = 0; i < processor.movementAnalysis.numMovementParams; i++)
		{
			ui_mappingMatrix.labels_movementParams[i].setText(
				processor.movementAnalysis.movementParams[i].name
				, dontSendNotification
			);
			if (processor.movementAnalysis.movementParams[i].name == "Placeholder")
				ui_mappingMatrix.labels_movementParams[i].setColour(
					ui_mappingMatrix.labels_movementParams[i].textColourId, Colours::red
				);

			for (int j = 0; j < processor.movementAnalysis.musicControl.numFbVariables; j++)
			{
				ui_mappingMatrix.mapping_Matrix[i][j].setToggleState
				(processor.movementAnalysis.musicControl.mappingMatrix[i][j],
					dontSendNotification);

				ui_mappingMatrix.mapping_Matrix[i][j].onStateChange = [this, i, j]
				{
					processor.movementAnalysis.musicControl.mappingMatrix[i][j] =
						ui_mappingMatrix.mapping_Matrix[i][j].getToggleState();

					ui_mappingMatrix.mapping_Strength[i][j].setVisible(
						ui_mappingMatrix.mapping_Matrix[i][j].getToggleState()
					);
				};

				ui_mappingMatrix.mapping_Strength[i][j].setRange(-2, 2, 0.5);
				ui_mappingMatrix.mapping_Strength[i][j].setValue(1);
				ui_mappingMatrix.mapping_Strength[i][j].setSliderStyle(Slider::SliderStyle::Rotary);
				ui_mappingMatrix.mapping_Strength[i][j].setColour(
					ui_mappingMatrix.mapping_Strength[i][j].rotarySliderFillColourId, Colours::yellow);
				ui_mappingMatrix.mapping_Strength[i][j].setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 20, 20);

				ui_mappingMatrix.mapping_Strength[i][j].onValueChange = [this, i, j]
				{
					processor.movementAnalysis.musicControl.mappingStrength[i][j] = ui_mappingMatrix.mapping_Strength[i][j].getValue();
					if (ui_mappingMatrix.mapping_Strength[i][j].getValue() == 1.0)
						ui_mappingMatrix.mapping_Strength[i][j].setColour(
							ui_mappingMatrix.mapping_Strength[i][j].thumbColourId,Colours::green);
					else ui_mappingMatrix.mapping_Strength[i][j].setColour(
						ui_mappingMatrix.mapping_Strength[i][j].thumbColourId, Colours::blue);
					if (ui_mappingMatrix.mapping_Strength[i][j].getValue() == -1.0)
						ui_mappingMatrix.mapping_Strength[i][j].setColour(
							ui_mappingMatrix.mapping_Strength[i][j].thumbColourId, Colours::red);
					if (ui_mappingMatrix.mapping_Strength[i][j].getValue() == 0.0)
						ui_mappingMatrix.mapping_Strength[i][j].setColour(
							ui_mappingMatrix.mapping_Strength[i][j].thumbColourId, Colours::white);
				};
			}

			ui_mappingMatrix.preset_ListLoad.addListener(this);
		}

		for (int k = 0; k < processor.movementAnalysis.musicControl.numFbVariables; k++)
		{
			ui_mappingMatrix.labels_audioParams[k].setText(
				processor.movementAnalysis.musicControl.feedbackVariables[k].name
				, dontSendNotification
			);

			if (processor.movementAnalysis.musicControl.feedbackVariables[k].name == "Placeholder")
				ui_mappingMatrix.labels_audioParams[k].setColour(
					ui_mappingMatrix.labels_audioParams[k].textColourId, Colours::red
				);

			// ADD MAPPING FUNCTION SHAPES TO ui_mappingMatrix.mapping_Function[k]
			ui_mappingMatrix.mapping_Function[k].addListener(this);
			for (int m = 0; m < processor.movementAnalysis.musicControl.numMapFunc; m++)
			{
				ui_mappingMatrix.mapping_Function[k].addItem(
					processor.movementAnalysis.musicControl.mapFunc_Names[m]
					,m + 1
				);
			}
			ui_mappingMatrix.mapping_Function[k].setSelectedId(
				processor.movementAnalysis.musicControl.feedbackVariables[k].mapFunc
			);

			ui_mappingMatrix.mapping_Polarity[k].addListener(this);
			ui_mappingMatrix.mapping_Polarity[k].addItem("+", 1);
			ui_mappingMatrix.mapping_Polarity[k].addItem("-", 2);
			ui_mappingMatrix.mapping_Polarity[k].setSelectedId(1);

			// ADD QUANTIZATION LEVELS 
			ui_mappingMatrix.mapping_QuantLevels[k].addListener(this);
			ui_mappingMatrix.mapping_QuantLevels[k].addItem("0", 1);
			ui_mappingMatrix.mapping_QuantLevels[k].addItem("1", 2);
			ui_mappingMatrix.mapping_QuantLevels[k].addItem("2", 3);
			ui_mappingMatrix.mapping_QuantLevels[k].addItem("3", 4);
			ui_mappingMatrix.mapping_QuantLevels[k].addItem("4", 5);
			ui_mappingMatrix.mapping_QuantLevels[k].addItem("5", 6);
			ui_mappingMatrix.mapping_QuantLevels[k].setSelectedId(
				processor.movementAnalysis.musicControl.feedbackVariables[k].quantLevels_2raisedTo + 1
			);

		}

		ui_mappingMatrix.preset_Save.onClick = [this]
		{
			ui_mappingMatrix.saveAsPreset();
			ui_mappingMatrix.populatePresets(processor.movementAnalysis.musicControl.mappingPresets);
			for (int i = 0; i < ui_mappingMatrix.num_Presets; i++)
			{
				if (processor.movementAnalysis.musicControl.mappingPresets[i].name ==
					ui_mappingMatrix.preset_Name.getText())
					ui_mappingMatrix.preset_ListLoad.setSelectedId(i + 1);
				ui_mappingMatrix.preset_Name.setText("");
			}
		};
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
		{
			ui_movementAnalysis.JointAngles[j].setText(
				ui_movementAnalysis.JointNames[j] + " Angle: "
				+ String(processor.movementAnalysis.jointAngles_Deg[j], 2)
				, dontSendNotification
			);

			ui_movementAnalysis.JointVelocities[j].setText(
				ui_movementAnalysis.JointNames[j] + " Ang Vel: "
				+ String(processor.movementAnalysis.jointAngularVel_DegPerSec[j], 2)
				, dontSendNotification
			);
		}

		// STS PHASE DISPLAY
		ui_movementAnalysis.STSPhasePresent.setText(
			processor.movementAnalysis.STS_Phases[processor.movementAnalysis.STS_Phase]
			, dontSendNotification
		);

		// REAL TIME VISUALIZE
		ui_movementAnalysis.updateSTSAnim(
			processor.movementAnalysis.movementParams[0].value,
			processor.movementAnalysis.movementParams[1].value,
			processor.movementAnalysis.movementParams[2].value
		);

		// MOVEMENT RANGE SLIDER LABELS
		ui_movementAnalysis.range_TrunkAP_LAB.setText(
			String(processor.movementAnalysis.movementParams[0].minVal, 2) + " to " 
			+ String(processor.movementAnalysis.movementParams[0].maxVal, 2)
			, dontSendNotification
		);
	}

	// Update Music Control Tab Elements
	void musicControls_updateLabels()
	{

	}

	// Update Mapping Matrix Tab Elements
	void mappingMatrix_updateLabels()
	{
		ui_mappingMatrix.updateValueVisualizers(
			interface_Width,
			interface_Height,
			processor.movementAnalysis.numMovementParams,
			processor.movementAnalysis.musicControl.numFbVariables,
			processor.movementAnalysis.movementParams,
			processor.movementAnalysis.musicControl.feedbackVariables
		);
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StsinteractionAudioProcessorEditor)
};
