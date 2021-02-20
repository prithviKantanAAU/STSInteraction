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
	void mpLogStream_Configure_Button_Behaviors();

	// INTERFACE PROPERTIES
	int interface_Width = 1100;						// Pixels
	int interface_Height = 550;						// Pixels
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
			addAndMakeVisible(ui_movementAnalysis.IMU_Mount_Side[i]);
			addAndMakeVisible(ui_movementAnalysis.IMU_Polarity[i]);
			addAndMakeVisible(ui_movementAnalysis.IMU_Online_Indicator[i]);
			addAndMakeVisible(ui_movementAnalysis.IMU_Location[i]);
			addAndMakeVisible(ui_movementAnalysis.IMU_segmentRanges_AP_Bounds[i][0]);
			addAndMakeVisible(ui_movementAnalysis.IMU_range_segmentAngles_AP[i]);
			addAndMakeVisible(ui_movementAnalysis.IMU_segmentAngles_AP_Indicator[i]);
			addAndMakeVisible(ui_movementAnalysis.IMU_segmentRanges_AP_Bounds[i][1]);
			addAndMakeVisible(ui_movementAnalysis.IMU_segmentRanges_ML_Bounds[i][0]);
			addAndMakeVisible(ui_movementAnalysis.IMU_range_segmentAngles_ML[i]);
			addAndMakeVisible(ui_movementAnalysis.IMU_segmentAngles_ML_Indicator[i]);
			addAndMakeVisible(ui_movementAnalysis.IMU_segmentRanges_ML_Bounds[i][1]);

			// MP LOG STREAMING
			addAndMakeVisible(ui_movementAnalysis.dataInput_Mode_Status);
			addAndMakeVisible(ui_movementAnalysis.mpLog_File_Load_Unload);
			addAndMakeVisible(ui_movementAnalysis.mpLog_File_Play_Pause);
			addAndMakeVisible(ui_movementAnalysis.mpLog_File_Stop);
			addAndMakeVisible(ui_movementAnalysis.mpLog_File_Progress);

			addAndMakeVisible(ui_movementAnalysis.simulation_OrientAngles[i]);
			addAndMakeVisible(ui_movementAnalysis.simulation_OrientAngles_LAB[i]);
		}

		for (int j = 0; j < 2; j++)
		{
			addAndMakeVisible(ui_movementAnalysis.JointAngles[j]);
			addAndMakeVisible(ui_movementAnalysis.JointVelocities[j]);
		}
		addAndMakeVisible(ui_movementAnalysis.ML);
		addAndMakeVisible(ui_movementAnalysis.AP);
		addAndMakeVisible(ui_movementAnalysis.STS_Phase_Disp);
		addAndMakeVisible(ui_movementAnalysis.record_MovementLog);
		addAndMakeVisible(ui_movementAnalysis.operationMode);
		addAndMakeVisible(ui_movementAnalysis.orientationAlgo);

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
		addAndMakeVisible(ui_musicControl.toggle_DSP_OnOff);
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
		addAndMakeVisible(ui_musicControl.levelMeter[0]);
		addAndMakeVisible(ui_musicControl.levelMeter[1]);
		addAndMakeVisible(ui_musicControl.triOsc_BPM);
		addAndMakeVisible(ui_musicControl.triOsc_BPM_LAB);

		addAndMakeVisible(ui_musicControl.voiceCue_Enable);
		addAndMakeVisible(ui_musicControl.voiceCue_Enable_Lab);
		addAndMakeVisible(ui_musicControl.voiceCue_Level);
		addAndMakeVisible(ui_musicControl.voiceCue_Level_Lab);
		addAndMakeVisible(ui_musicControl.voiceCue_CountLength);
		addAndMakeVisible(ui_musicControl.voiceCue_CountLength_Lab);
		for (int i = 0; i < 3; i++) addAndMakeVisible(ui_musicControl.voiceCue_ColHeaders[i]);
		for (int i = 0; i < 8; i++)
		{
			addAndMakeVisible(ui_musicControl.voiceCue_intervalEnable[i]);
			addAndMakeVisible(ui_musicControl.voiceCue_isPosCrossing[i]);
			addAndMakeVisible(ui_musicControl.voiceCue_OscTrig_Location[i]);
		}
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
				addAndMakeVisible(ui_mappingMatrix.mp_minThresh[i]);
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
		
		// MP Log Playback
		mpLogStream_Configure_Button_Behaviors();

		// Simulation Sliders, Sensor Invert, Axis
		for (int i = 0; i < 3; i++)
		{
			ui_movementAnalysis.simulation_OrientAngles[i].onValueChange = [this, i]
			{
				processor.movementAnalysis.setSimulationAngle
				(i, ui_movementAnalysis.simulation_OrientAngles[i].getValue());
			};

			ui_movementAnalysis.IMU_Mount_Side[i].addListener(this);
			ui_movementAnalysis.IMU_Polarity[i].addListener(this);

			ui_movementAnalysis.IMU_range_segmentAngles_AP[i].setMinValue(processor.movementAnalysis.movementParams[i].minVal);
			ui_movementAnalysis.IMU_range_segmentAngles_AP[i].setMaxValue(processor.movementAnalysis.movementParams[i].maxVal);

			ui_movementAnalysis.IMU_range_segmentAngles_AP[i].onValueChange = [this,i]
			{
				processor.movementAnalysis.movementParams[i].minVal = ui_movementAnalysis.IMU_range_segmentAngles_AP[i].getMinValue();
				processor.movementAnalysis.movementParams[i].maxVal = ui_movementAnalysis.IMU_range_segmentAngles_AP[i].getMaxValue();

				ui_movementAnalysis.simulation_OrientAngles[i].setRange(
					processor.movementAnalysis.movementParams[i].minVal,
					processor.movementAnalysis.movementParams[i].maxVal
				);
			};

			ui_movementAnalysis.IMU_range_segmentAngles_ML[i].setMinValue(processor.movementAnalysis.movementParams[i+3].minVal);
			ui_movementAnalysis.IMU_range_segmentAngles_ML[i].setMaxValue(processor.movementAnalysis.movementParams[i+3].maxVal);

			ui_movementAnalysis.IMU_range_segmentAngles_ML[i].onValueChange = [this, i]
			{
				processor.movementAnalysis.movementParams[i + 3].minVal = ui_movementAnalysis.IMU_range_segmentAngles_ML[i].getMinValue();
				processor.movementAnalysis.movementParams[i + 3].maxVal = ui_movementAnalysis.IMU_range_segmentAngles_ML[i].getMaxValue();
			};
		}

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
		ui_musicControl.toggle_DSP_OnOff.onClick = [this]
		{
			if (!processor.isDSP_ON)
			{
				processor.startMusicDSP();
				ui_musicControl.toggle_DSP_OnOff.setColour(
					ui_musicControl.toggle_DSP_OnOff.buttonColourId, Colours::red);
				ui_musicControl.toggle_DSP_OnOff.setButtonText("Stop Music DSP");
			}
			else
			{
				processor.stopMusicDSP();
				ui_musicControl.toggle_DSP_OnOff.setColour(
					ui_musicControl.toggle_DSP_OnOff.buttonColourId, Colours::blue);
				ui_musicControl.toggle_DSP_OnOff.setButtonText("Start Music DSP");
			}
		};

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

		ui_musicControl.triOsc_BPM.onValueChange = [this]
		{
			processor.movementAnalysis.triOsc_Freq = ui_musicControl.triOsc_BPM.getValue() / 60.0;
		};

		ui_musicControl.voiceCue_Enable.onStateChange = [this]
		{
			processor.movementAnalysis.voiceCue.isEnabled = ui_musicControl.voiceCue_Enable.getToggleState();
		};

		ui_musicControl.voiceCue_Level.onValueChange = [this]
		{
			processor.movementAnalysis.voiceCue.voiceGain_dB = ui_musicControl.voiceCue_Level.getValue();
		};

		ui_musicControl.voiceCue_CountLength.onValueChange = [this]
		{
			processor.movementAnalysis.voiceCue.count_Length = ui_musicControl.voiceCue_CountLength.getValue();
		};

		for (int i = 0; i < 8; i++)
		{
			ui_musicControl.voiceCue_intervalEnable[i].onStateChange = [this]
			{

			}
		}
	}

	// Initialize Mapping Matrix Elements
	void mappingMatrix_initializeControls()
	{
		Colour labelColour = Colours::black;

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

			ui_mappingMatrix.mp_minThresh[i].onValueChange = [this, i]
			{
				processor.movementAnalysis.movementParams[i].thresh_min_NORM = ui_mappingMatrix.mp_minThresh[i].getValue();
			};

			for (int j = 0; j < processor.movementAnalysis.musicControl.numFbVariables; j++)
			{
				ui_mappingMatrix.mapping_Matrix[i][j].setToggleState
				(processor.movementAnalysis.musicControl.mappingMatrix[i][j],
					dontSendNotification);
				ui_mappingMatrix.mapping_Matrix[i][j].setColour(
					ui_mappingMatrix.mapping_Matrix[i][j].tickColourId, Colours::yellow);

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
			switch (processor.movementAnalysis.musicControl.feedbackVariables[k].parameterType)
			{
			case 1:
				labelColour = Colours::magenta;
				break;
			case 2:
				labelColour = Colours::lightblue;
				break;
			case 3:
				labelColour = Colours::lightgreen;
				break;
			case 4:
				labelColour = Colours::lightgrey;
				break;
			}

			ui_mappingMatrix.labels_audioParams[k].setText(
				processor.movementAnalysis.musicControl.feedbackVariables[k].name
				, dontSendNotification
			);

			ui_mappingMatrix.labels_audioParams[k].setColour(ui_mappingMatrix.labels_audioParams[k].textColourId, labelColour);

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
			ui_mappingMatrix.preset_Name.setText("");
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
		int rowIndex = 0;
		for (int i = 0; i < 3; i++)
		{
			colour_onlineIndicator = processor.movementAnalysis.locationsOnline[i] != -1 ? Colours::green : Colours::red;
			// SET COLOUR OF ONLINE INDICATOR
			ui_movementAnalysis.IMU_Online_Indicator[i].setColour(ui_movementAnalysis.IMU_Online_Indicator[i].backgroundColourId, colour_onlineIndicator);
		}

		ui_movementAnalysis.update_Indicators_SensorOrientation(processor.movementAnalysis.movementParams);

		// JOINT ANGLE DISPLAYS
		for (int j = 0; j < 2; j++)
		{
			ui_movementAnalysis.JointAngles[j].setText(
				String(processor.movementAnalysis.movementParams[6 + j].value, 2)
				, dontSendNotification
			);

			ui_movementAnalysis.JointVelocities[j].setText(
				String(processor.movementAnalysis.movementParams[9 - j].value, 2)
				, dontSendNotification
			);
		}

		// DATA INPUT MODE
		ui_movementAnalysis.dataInput_Mode_Status.setText(
			processor.movementAnalysis.dataInput_Mode_Names[processor.movementAnalysis.dataInput_Mode]
			, dontSendNotification
		);
		
		// LOG PROGRESS
		ui_movementAnalysis.mpLog_File_Progress_VAL = processor.movementAnalysis.mpFile_Streaming_Progress;

		// STS PHASE DISPLAY
		ui_movementAnalysis.STS_Phase_Disp.setText(
			processor.movementAnalysis.STS_Phases[(int)processor.movementAnalysis.movementParams[10].value]
			, dontSendNotification
		);

		// REAL TIME VISUALIZE
		ui_movementAnalysis.updateSTSAnim(
			processor.movementAnalysis.movementParams[0].value,
			processor.movementAnalysis.movementParams[1].value,
			processor.movementAnalysis.movementParams[2].value
		);
	}

	// Update Music Control Tab Elements
	void musicControls_updateLabels()
	{
		float level_label1_width = (processor.musicLevel_dB - (-60)) / 60.0 * ui_musicControl.levelMeter_Width;
		float level_label2_width = ui_musicControl.levelMeter_Width - level_label1_width;

		ui_musicControl.levelMeter[0].setColour(
			ui_musicControl.levelMeter[0].backgroundColourId,
			(processor.musicLevel_dB > -2) ? Colours::red : Colours::yellow);

		ui_musicControl.levelMeter[0].setBounds(
			850,
			50,
			level_label1_width,
			25
		);

		ui_musicControl.levelMeter[1].setBounds(
			850 + level_label1_width,
			50,
			level_label2_width,
			25
		);
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
