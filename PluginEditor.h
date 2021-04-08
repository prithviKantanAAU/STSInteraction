#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "UI_SensorConfig.h"
#include "UI_MovementAnalysis.h"
#include "GaitParam_Single.h"
#include "AudioParam_Single.h"
#include "SensorInfo.h"
#include "MappingPreset.h"
#include "MusicControl.h"
#include "MovementAnalysis.h"
#include "MusicInfoCompute.h"
#include "OSC_Class.h"
#include "VoiceCue_Settings.h"
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
	
	// SHORTCUT POINTERS
	MovementAnalysis* mAnalysisPtr = &processor.movementAnalysis;
	MovementParameter* mpArrayPtr = mAnalysisPtr->movementParams;
	FeedbackVariable* apArrayPtr = mAnalysisPtr->musicControl.feedbackVariables;
	MappingPreset* mapPresetPtr = mAnalysisPtr->musicControl.mappingPresets;
	MusicInfoCompute* musInfoCompPtr = &mAnalysisPtr->musicControl.musicInfoCompute;
	MusicControl* musControlPtr = &mAnalysisPtr->musicControl;
	SensorInfo* sensInfoPtr = &mAnalysisPtr->sensorInfo;
	OSCReceiverUDP_Sensor* oscSensPtr = mAnalysisPtr->sensors_OSCReceivers;
	VoiceCues* voiceCuePtr = &mAnalysisPtr->voiceCue;

	// SIMULATION KEYPRESSES
	KeyPress TrunkThigh_PlusMinus_RLUD[4];

	void simulation_HandleKeyPresses()
	{
		bool isKeyDown[6] = { false, false, false, false};
		isKeyDown[0] = TrunkThigh_PlusMinus_RLUD[0].isKeyCurrentlyDown(KeyPress::rightKey);
		isKeyDown[1] = TrunkThigh_PlusMinus_RLUD[1].isKeyCurrentlyDown(KeyPress::leftKey);
		isKeyDown[2] = TrunkThigh_PlusMinus_RLUD[2].isKeyCurrentlyDown(KeyPress::upKey);
		isKeyDown[3] = TrunkThigh_PlusMinus_RLUD[3].isKeyCurrentlyDown(KeyPress::downKey);
		isKeyDown[4] = TrunkThigh_PlusMinus_RLUD[4].isKeyCurrentlyDown(KeyPress::pageUpKey);
		isKeyDown[5] = TrunkThigh_PlusMinus_RLUD[5].isKeyCurrentlyDown(KeyPress::pageDownKey);

		for (int i = 0; i < 6; i++)
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
				case 4:
					ui_movementAnalysis.simulation_OrientAngles[2].setValue(
						ui_movementAnalysis.simulation_OrientAngles[2].getValue() + 2.3
					);
					break;
				case 5:
					ui_movementAnalysis.simulation_OrientAngles[2].setValue(
						ui_movementAnalysis.simulation_OrientAngles[2].getValue() - 2.3
					);
					break;
				}
			}
		}
	}
	void mpLogStream_Configure_Button_Behaviors();

	// INTERFACE PROPERTIES
	int interface_Width = 1350;						// Pixels
	int interface_Height = 680;						// Pixels
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
			addAndMakeVisible(ui_movementAnalysis.mpLog_File_FWD);
			addAndMakeVisible(ui_movementAnalysis.mpLog_File_RWD);

			addAndMakeVisible(ui_movementAnalysis.simulation_OrientAngles[i]);
			addAndMakeVisible(ui_movementAnalysis.simulation_OrientAngles_LAB[i]);
		}

		for (int j = 0; j < 3; j++)
		{
			addAndMakeVisible(ui_movementAnalysis.JointAngles[j]);
			addAndMakeVisible(ui_movementAnalysis.JointAngles_Indicators[j]);
			addAndMakeVisible(ui_movementAnalysis.JointVelocities[j]);

			addAndMakeVisible(ui_movementAnalysis.Joint_Range_AP[j]);
			addAndMakeVisible(ui_movementAnalysis.Joint_Range_AP_MovementRanges[j][0]);
			addAndMakeVisible(ui_movementAnalysis.Joint_Range_AP_MovementRanges[j][1]);
			addAndMakeVisible(ui_movementAnalysis.Joint_Range_AP_HyperExtendThresh[j]);
		}
		addAndMakeVisible(ui_movementAnalysis.ML);
		addAndMakeVisible(ui_movementAnalysis.AP);
		addAndMakeVisible(ui_movementAnalysis.STS_Phase_Disp);
		addAndMakeVisible(ui_movementAnalysis.gyrMeasError);
		addAndMakeVisible(ui_movementAnalysis.record_MovementLog);
		addAndMakeVisible(ui_movementAnalysis.operationMode);
		//addAndMakeVisible(ui_movementAnalysis.orientationAlgo);

		// CoM BOUND SLIDERS
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_Bounds_H);
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_Bounds_H_Lab);
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_Bounds_H_MIN);
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_Bounds_H_MAX);
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_INDIC_VAL_H);

		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_Bounds_V);
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_Bounds_V_Lab);
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_Bounds_V_MIN);
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_Bounds_V_MAX);
		addAndMakeVisible(ui_movementAnalysis.CoM_Disp_INDIC_VAL_V);
		addAndMakeVisible(ui_movementAnalysis.cal_CoM_SIT);
		addAndMakeVisible(ui_movementAnalysis.cal_CoM_STAND);

		// STS VISUALIZER
		for (int i = 0; i < 4; i++)
			addAndMakeVisible(ui_movementAnalysis.stsAnim_joint[i]);

		for (int j = 0; j < 20; j++)
		{
			for (int k = 0; k < 3; k++)
			addAndMakeVisible(ui_movementAnalysis.stsAnim_Segments[k][j]);
		}
		addAndMakeVisible(ui_movementAnalysis.stsAnim_CoM_Indicator);
	}

	// Add Music Control Controls
	void addControls_MusicControl()
	{
		addAndMakeVisible(ui_musicControl.toggle_DSP_OnOff);
		addAndMakeVisible(ui_musicControl.gain_Master);
		addAndMakeVisible(ui_musicControl.gain_Master_LAB);
		for (int i = 0; i < musControlPtr->mixerSettings.num_Tracks; i++)
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
		for (int i = 0; i < 3; i++)
		{
			addAndMakeVisible(ui_musicControl.voiceCue_intervalEnable[i]);
			addAndMakeVisible(ui_musicControl.voiceCue_isPosCrossing[i]);
			addAndMakeVisible(ui_musicControl.voiceCue_OscTrig_Location[i]);
		}
		addAndMakeVisible(ui_musicControl.voiceCue_FineTimeAdjust);
		addAndMakeVisible(ui_musicControl.voiceCue_FineTimeAdjust_Lab);
	}

	// Add Mapping Matrix Controls
	void addControls_MappingMatrix()
	{
		for (int m = 0; m < 40; m++)
		{
			addAndMakeVisible(ui_mappingMatrix.labels_movementParams[m]);
			addAndMakeVisible(ui_mappingMatrix.movementParams_Value[m][0]);
			addAndMakeVisible(ui_mappingMatrix.movementParams_Value[m][1]);
			addAndMakeVisible(ui_mappingMatrix.normRange_MP[m]);
		}

		for (int a = 0; a < 40; a++)
		{
			addAndMakeVisible(ui_mappingMatrix.labels_audioParams[a]);
			addAndMakeVisible(ui_mappingMatrix.mapping_Function[a]);
			addAndMakeVisible(ui_mappingMatrix.mapping_Polarity[a]);
			addAndMakeVisible(ui_mappingMatrix.mapping_QuantLevels[a]);
			addAndMakeVisible(ui_mappingMatrix.audioParams_Value[a][0]);
			addAndMakeVisible(ui_mappingMatrix.audioParams_Value[a][1]);
			addAndMakeVisible(ui_mappingMatrix.normRange_AP[a]);
		}

		for (int m = 0; m < 40; m++)
		{
			for (int a = 0; a < 40; a++)
			{
				addAndMakeVisible(ui_mappingMatrix.mapping_Matrix[m][a]);
				addAndMakeVisible(ui_mappingMatrix.mapping_Strength[m][a]);
			}
		}

		addAndMakeVisible(ui_mappingMatrix.preset_Save);
		addAndMakeVisible(ui_mappingMatrix.preset_ListLoad);
		addAndMakeVisible(ui_mappingMatrix.preset_Name);
		addAndMakeVisible(ui_mappingMatrix.mp_isVisible);
		addAndMakeVisible(ui_mappingMatrix.mp_isHidden);
		addAndMakeVisible(ui_mappingMatrix.ap_isVisible);
		addAndMakeVisible(ui_mappingMatrix.ap_isHidden);
	}

	// Initialize Sensor Config Tab Elements
	void sensorConfig_initializeControls()
	{
		String statusText = "";
		for (int i = 0; i < 3; i++)
		{
			statusText = sensInfoPtr->isOnline[i] ? "ON" : "OFF";
			ui_sensorCon.Status[i].setText(statusText, dontSendNotification);

			ui_sensorCon.Port[i].setText(String(sensInfoPtr->UDP_Ports[i]), dontSendNotification);

			ui_sensorCon.Location[i].addListener(this);

			ui_sensorCon.BiasComp[i].onClick = [this, i]
			{
				if (!oscSensPtr[i].isBiasComp_ON)
				{
					oscSensPtr[i].isBiasComp_DONE = false;
					oscSensPtr[i].isBiasComp_ON = true;
				}
			};

			ui_sensorCon.WLAN_IP[i].onTextChange = [this, i]
			{
				sensInfoPtr->remoteIP[i] = ui_sensorCon.WLAN_IP[i].getText();
			};
		}
	}

	// Initialize Movement Analysis Elements
	void movementAnalysis_initializeControls()
	{
		movementAnalysis_updateLabels();				// FIRST TIME UPDATE

		// Operation Mode - Sensor / Simulation
		ui_movementAnalysis.operationMode.addListener(this);
		for (int i = 0; i < mAnalysisPtr->numOperationModes; i++)
			ui_movementAnalysis.operationMode.addItem(mAnalysisPtr->OperationModes[i], i + 1);
		ui_movementAnalysis.operationMode.setSelectedId(mAnalysisPtr->operationMode_Present);
		
		// MP Log Playback
		mpLogStream_Configure_Button_Behaviors();

		// Simulation Sliders, Sensor Invert, Axis
		for (int i = 0; i < 3; i++)
		{
			ui_movementAnalysis.simulation_OrientAngles[i].onValueChange = [this, i]
			{
				mAnalysisPtr->setSimulationAngle
				(i, ui_movementAnalysis.simulation_OrientAngles[i].getValue());
			};

			ui_movementAnalysis.IMU_Mount_Side[i].addListener(this);
			ui_movementAnalysis.IMU_Mount_Side[i].setSelectedId(mAnalysisPtr->IMU_Mount_Side[i]);

			ui_movementAnalysis.IMU_Polarity[i].addListener(this);
			ui_movementAnalysis.IMU_Polarity[i].setSelectedId(mAnalysisPtr->IMU_Polarity[i]);

			ui_movementAnalysis.IMU_range_segmentAngles_AP[i].setMinValue(mpArrayPtr[i].minVal);
			ui_movementAnalysis.IMU_range_segmentAngles_AP[i].setMaxValue(mpArrayPtr[i].maxVal);

			ui_movementAnalysis.IMU_range_segmentAngles_AP[i].onValueChange = [this,i]
			{
				mpArrayPtr[i].minVal = ui_movementAnalysis.IMU_range_segmentAngles_AP[i].getMinValue();
				mpArrayPtr[i].maxVal = ui_movementAnalysis.IMU_range_segmentAngles_AP[i].getMaxValue();

				ui_movementAnalysis.simulation_OrientAngles[i].setRange(
					mpArrayPtr[i].minVal,
					mpArrayPtr[i].maxVal
				);
			};

			ui_movementAnalysis.IMU_range_segmentAngles_ML[i].setMinValue(mpArrayPtr[i+3].minVal);
			ui_movementAnalysis.IMU_range_segmentAngles_ML[i].setMaxValue(mpArrayPtr[i+3].maxVal);

			ui_movementAnalysis.IMU_range_segmentAngles_ML[i].onValueChange = [this, i]
			{
				mpArrayPtr[i + 3].minVal = ui_movementAnalysis.IMU_range_segmentAngles_ML[i].getMinValue();
				mpArrayPtr[i + 3].maxVal = ui_movementAnalysis.IMU_range_segmentAngles_ML[i].getMaxValue();
			};
		}

		// GyrError Adjust
		ui_movementAnalysis.gyrMeasError.onValueChange = [this]
		{
			for (int i = 0; i < 3; i++)
			{
				mAnalysisPtr->quaternionFilters[i].updateBeta(ui_movementAnalysis.gyrMeasError.getValue());
			}
		};

		// Hip Angle
		ui_movementAnalysis.Joint_Range_AP[0].setMaxValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Angle Hip", "Max"));
		ui_movementAnalysis.Joint_Range_AP[0].setMinValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Angle Hip", "Min"));
		ui_movementAnalysis.Joint_Range_AP[0].setValue(mAnalysisPtr->jointAngles_thresh_MaxFlex[0]);
		ui_movementAnalysis.Joint_Range_AP[0].onValueChange = [this]
		{
			mAnalysisPtr->setBounds_MP(mpArrayPtr,"Angle Hip", ui_movementAnalysis.Joint_Range_AP[0].getMinValue(),
				ui_movementAnalysis.Joint_Range_AP[0].getMaxValue());
			mAnalysisPtr->jointAngles_thresh_MaxFlex[0] = ui_movementAnalysis.Joint_Range_AP[0].getValue();
		};

		// Knee Angle
		ui_movementAnalysis.Joint_Range_AP[1].setMaxValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Angle Knee", "Max"));
		ui_movementAnalysis.Joint_Range_AP[1].setMinValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Angle Knee", "Min"));
		ui_movementAnalysis.Joint_Range_AP[1].setValue(mAnalysisPtr->jointAngles_thresh_MaxFlex[1]);
		ui_movementAnalysis.Joint_Range_AP[1].onValueChange = [this]
		{
			mAnalysisPtr->setBounds_MP(mpArrayPtr, "Angle Knee", ui_movementAnalysis.Joint_Range_AP[1].getMinValue(),
				ui_movementAnalysis.Joint_Range_AP[1].getMaxValue());
			mAnalysisPtr->jointAngles_thresh_MaxFlex[1] = ui_movementAnalysis.Joint_Range_AP[1].getValue();
		};

		// Ankle Angle
		ui_movementAnalysis.Joint_Range_AP[2].setMinValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Angle Ankle", "Min"));
		ui_movementAnalysis.Joint_Range_AP[2].setMaxValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Angle Ankle", "Max"));
		ui_movementAnalysis.Joint_Range_AP[2].setValue(mAnalysisPtr->jointAngles_thresh_MaxFlex[2]);
		ui_movementAnalysis.Joint_Range_AP[2].onValueChange = [this]
		{
			mAnalysisPtr->setBounds_MP(mpArrayPtr, "Angle Ankle", ui_movementAnalysis.Joint_Range_AP[2].getMinValue(),
				ui_movementAnalysis.Joint_Range_AP[2].getMaxValue());
			mAnalysisPtr->jointAngles_thresh_MaxFlex[1] = ui_movementAnalysis.Joint_Range_AP[2].getValue();
		};

		// CoM H
		ui_movementAnalysis.CoM_Disp_Bounds_H.setMaxValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Horiz Disp", "Max"));
		ui_movementAnalysis.CoM_Disp_Bounds_H.setMinValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Horiz Disp", "Min"));
		ui_movementAnalysis.CoM_Disp_Bounds_H.onValueChange = [this]
		{
			mAnalysisPtr->setBounds_MP(mpArrayPtr, "Horiz Disp",
				ui_movementAnalysis.CoM_Disp_Bounds_H.getMinValue(),
				ui_movementAnalysis.CoM_Disp_Bounds_H.getMaxValue());
		};

		ui_movementAnalysis.CoM_Disp_Bounds_V.setMaxValue(mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Verti Disp", "Max"));
		ui_movementAnalysis.CoM_Disp_Bounds_V.setMinValue((double)mAnalysisPtr->getMPVal_fromArray(mpArrayPtr, "Verti Disp", "Min"));
		ui_movementAnalysis.CoM_Disp_Bounds_V.onValueChange = [this]
		{
			mAnalysisPtr->setBounds_MP(mpArrayPtr, "Verti Disp",
				ui_movementAnalysis.CoM_Disp_Bounds_V.getMinValue(),
				ui_movementAnalysis.CoM_Disp_Bounds_V.getMaxValue());
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
		
		// CALIBRATE SIT
		ui_movementAnalysis.cal_CoM_SIT.onClick = [this]
		{
			float CoM_H_Pos = MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Horiz Disp", "Val");
			float CoM_V_Pos = MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Verti Disp", "Val");

			ui_movementAnalysis.CoM_Disp_Bounds_H.setMinValue(CoM_H_Pos);
			ui_movementAnalysis.CoM_Disp_Bounds_V.setMinValue(CoM_V_Pos - 0.03);
		};

		// CALIBRATE STAND
		ui_movementAnalysis.cal_CoM_STAND.onClick = [this]
		{
			float CoM_H_Pos = MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Horiz Disp", "Val");
			float CoM_V_Pos = MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Verti Disp", "Val");

			ui_movementAnalysis.CoM_Disp_Bounds_H.setMaxValue(CoM_H_Pos + 0.03);
			ui_movementAnalysis.CoM_Disp_Bounds_V.setMaxValue(CoM_V_Pos);
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

		ui_musicControl.gain_Master.setValue(musControlPtr->mixerSettings.masterGain);
		ui_musicControl.gain_Master.onValueChange = [this]
		{
			if (processor.isReady)
			processor.set_masterGain(ui_musicControl.gain_Master.getValue());
		};

		for (int i = 0; i < musControlPtr->mixerSettings.num_Tracks; i++)
		{
			ui_musicControl.gain_Track[i].setValue(musControlPtr->mixerSettings.gain_Track[i]);
			ui_musicControl.gain_Track_LAB[i].setText(musControlPtr->mixerSettings.names_Tracks[i],dontSendNotification);

			ui_musicControl.gain_Track[i].onValueChange = [this,i]
			{
				if (processor.isReady)
				processor.set_trackFader
				(i, ui_musicControl.gain_Track[i].getValue());
			};
		}

		// TONIC
		ui_musicControl.tonic.addListener(this);
		for (int i = 0; i < 12; i++)
			ui_musicControl.tonic.addItem(musInfoCompPtr->tonics_Names[i], i + 1);
		ui_musicControl.tonic.setSelectedId(7);

		// SCALE
		ui_musicControl.scale.addListener(this);
		for (int i = 0; i < musInfoCompPtr->numScales; i++)
			ui_musicControl.scale.addItem(musInfoCompPtr->scales_Names[i], i + 1);
		ui_musicControl.scale.setSelectedId(1);

		// CHORD TYPE
		ui_musicControl.chord_Type.addListener(this);
		for (int i = 0; i < musInfoCompPtr->numChordTypes; i++)
			ui_musicControl.chord_Type.addItem(musInfoCompPtr->chordTypes_Names[i], i + 1);
		ui_musicControl.chord_Type.setSelectedId(1);

		for (int i = 0; i < 8; i++)
		{
			ui_musicControl.chord_Degree[i].addListener(this);
			for (int j = 1; j <= 8; j++)
				ui_musicControl.chord_Degree[i].addItem(String(j), j);
			ui_musicControl.chord_Degree[i].setSelectedId(musInfoCompPtr->chord_degSequence[i]);
		}

		ui_musicControl.triOsc_BPM.onValueChange = [this]
		{
			mAnalysisPtr->triOsc_Freq = ui_musicControl.triOsc_BPM.getValue() / 60.0;
			ui_mappingMatrix.dataHolder_oscBPM = ui_musicControl.triOsc_BPM.getValue() / 60.0;
		};

		ui_musicControl.voiceCue_Enable.setToggleState(voiceCuePtr->isEnabled, sendNotificationSync);
		ui_musicControl.voiceCue_Enable.onStateChange = [this]
		{
			voiceCuePtr->isEnabled = ui_musicControl.voiceCue_Enable.getToggleState();
			ui_mappingMatrix.dataHolder_voiceCue_ON = ui_musicControl.voiceCue_Enable.getToggleState();
		};

		ui_musicControl.voiceCue_Level.setValue(voiceCuePtr->voiceGain_dB);
		ui_musicControl.voiceCue_Level.onValueChange = [this]
		{
			voiceCuePtr->voiceGain_dB = ui_musicControl.voiceCue_Level.getValue();
			ui_mappingMatrix.dataHolder_voiceCue_voldB = ui_musicControl.voiceCue_Level.getValue();
		};

		ui_musicControl.voiceCue_CountLength.setValue(voiceCuePtr->count_Length);
		ui_musicControl.voiceCue_CountLength.onValueChange = [this]
		{
			voiceCuePtr->count_Length = (int)ui_musicControl.voiceCue_CountLength.getValue();
			ui_mappingMatrix.dataHolder_voiceCue_Length = (int)ui_musicControl.voiceCue_CountLength.getValue();
		};

		for (int i = 0; i < 8; i++)
		{
			ui_musicControl.voiceCue_intervalEnable[i].setToggleState(voiceCuePtr->interval_IsEnabled[i], sendNotificationSync);
			ui_musicControl.voiceCue_intervalEnable[i].onStateChange = [this,i]
			{
				voiceCuePtr->interval_IsEnabled[i] = ui_musicControl.voiceCue_intervalEnable[i].getToggleState();
				ui_mappingMatrix.dataHolder_voiceCue_isIntervalEnabled[i] = ui_musicControl.voiceCue_intervalEnable[i].getToggleState();
			};

			ui_musicControl.voiceCue_isPosCrossing[i].setToggleState(voiceCuePtr->isPosCrossing[i], sendNotificationSync);
			ui_musicControl.voiceCue_isPosCrossing[i].onStateChange = [this,i]
			{
				voiceCuePtr->isPosCrossing[i] = ui_musicControl.voiceCue_isPosCrossing[i].getToggleState();
				ui_mappingMatrix.dataHolder_voiceCue_isPos[i] = ui_musicControl.voiceCue_isPosCrossing[i].getToggleState();
			};

			ui_musicControl.voiceCue_OscTrig_Location[i].setValue(voiceCuePtr->interval_crossThresh[i]);
			ui_musicControl.voiceCue_OscTrig_Location[i].onValueChange = [this,i]
			{
				voiceCuePtr->interval_crossThresh[i] = ui_musicControl.voiceCue_OscTrig_Location[i].getValue();
				ui_mappingMatrix.dataHolder_voiceCue_location[i] = ui_musicControl.voiceCue_OscTrig_Location[i].getValue();
			};
		}

		ui_musicControl.voiceCue_FineTimeAdjust.onValueChange = [this]
		{
			voiceCuePtr->fineOffset = ui_musicControl.voiceCue_FineTimeAdjust.getValue();
			ui_mappingMatrix.dataHolder_voiceCue_timingFine = ui_musicControl.voiceCue_FineTimeAdjust.getValue();
		};
	}

	// Initialize Mapping Matrix Elements
	void mappingMatrix_initializeControls()
	{
		Colour labelColour = Colours::black;

		for (int i = 0; i < mAnalysisPtr->numMovementParams; i++)
		{
			ui_mappingMatrix.labels_movementParams[i].setText(
				mpArrayPtr[i].name
				, dontSendNotification
			);
			if (mpArrayPtr[i].name == "Placeholder")
				ui_mappingMatrix.labels_movementParams[i].setColour(
					ui_mappingMatrix.labels_movementParams[i].textColourId, Colours::red
				);

			ui_mappingMatrix.normRange_MP[i].onValueChange = [this, i]
			{
				mpArrayPtr[i].rangeNorm_MIN = ui_mappingMatrix.normRange_MP[i].getMinValue();
				mpArrayPtr[i].rangeNorm_MAX = ui_mappingMatrix.normRange_MP[i].getMaxValue();
			};

			ui_mappingMatrix.normRange_AP[i].onValueChange = [this, i]
			{
				apArrayPtr[i].rangeNorm_MIN = ui_mappingMatrix.normRange_AP[i].getMinValue();
				apArrayPtr[i].rangeNorm_MAX = ui_mappingMatrix.normRange_AP[i].getMaxValue();
			};

			for (int j = 0; j < musControlPtr->numFbVariables; j++)
			{
				ui_mappingMatrix.mapping_Matrix[i][j].setToggleState(musControlPtr->mappingMatrix[i][j],dontSendNotification);
				ui_mappingMatrix.mapping_Matrix[i][j].setColour(ui_mappingMatrix.mapping_Matrix[i][j].tickColourId, Colours::yellow);

				ui_mappingMatrix.mapping_Matrix[i][j].onStateChange = [this, i, j]
				{
					musControlPtr->mappingMatrix[i][j] = ui_mappingMatrix.mapping_Matrix[i][j].getToggleState();
					mpArrayPtr[i].inRange = ui_mappingMatrix.mapping_Matrix[i][j].getToggleState();
					ui_mappingMatrix.mapping_Strength[i][j].setVisible(ui_mappingMatrix.mapping_Matrix[i][j].getToggleState());
				};

				ui_mappingMatrix.mapping_Strength[i][j].setRange(-2, 2, 0.5);
				ui_mappingMatrix.mapping_Strength[i][j].setValue(1);
				ui_mappingMatrix.mapping_Strength[i][j].setSliderStyle(Slider::SliderStyle::Rotary);
				ui_mappingMatrix.mapping_Strength[i][j].setColour(ui_mappingMatrix.mapping_Strength[i][j].rotarySliderFillColourId, Colours::yellow);
				ui_mappingMatrix.mapping_Strength[i][j].setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 20, 20);

				ui_mappingMatrix.mapping_Strength[i][j].onValueChange = [this, i, j]
				{
					musControlPtr->mappingStrength[i][j] = ui_mappingMatrix.mapping_Strength[i][j].getValue();
					if (ui_mappingMatrix.mapping_Strength[i][j].getValue() == 1.0)
						ui_mappingMatrix.mapping_Strength[i][j].setColour(ui_mappingMatrix.mapping_Strength[i][j].thumbColourId,Colours::green);
					else ui_mappingMatrix.mapping_Strength[i][j].setColour(ui_mappingMatrix.mapping_Strength[i][j].thumbColourId, Colours::blue);
					if (ui_mappingMatrix.mapping_Strength[i][j].getValue() == -1.0)
						ui_mappingMatrix.mapping_Strength[i][j].setColour(ui_mappingMatrix.mapping_Strength[i][j].thumbColourId, Colours::red);
					if (ui_mappingMatrix.mapping_Strength[i][j].getValue() == 0.0)
						ui_mappingMatrix.mapping_Strength[i][j].setColour(ui_mappingMatrix.mapping_Strength[i][j].thumbColourId, Colours::white);
				};
			}

			ui_mappingMatrix.preset_ListLoad.addListener(this);
		}

		for (int k = 0; k < musControlPtr->numFbVariables; k++)
		{
			switch (apArrayPtr[k].parameterType)
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
				apArrayPtr[k].name
				, dontSendNotification
			);

			ui_mappingMatrix.labels_audioParams[k].setColour(ui_mappingMatrix.labels_audioParams[k].textColourId, labelColour);

			if (apArrayPtr[k].name == "Placeholder")
				ui_mappingMatrix.labels_audioParams[k].setColour(
					ui_mappingMatrix.labels_audioParams[k].textColourId, Colours::red
				);

			// ADD MAPPING FUNCTION SHAPES TO ui_mappingMatrix.mapping_Function[k]
			ui_mappingMatrix.mapping_Function[k].addListener(this);
			for (int m = 0; m < musControlPtr->numMapFunc; m++)
				ui_mappingMatrix.mapping_Function[k].addItem(musControlPtr->mapFunc_Names[m],m + 1);
			ui_mappingMatrix.mapping_Function[k].setSelectedId(apArrayPtr[k].mapFunc);

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
				apArrayPtr[k].quantLevels_2raisedTo + 1
			);

		}

		ui_mappingMatrix.preset_Save.onClick = [this]
		{
			ui_mappingMatrix.saveAsPreset(mpArrayPtr, apArrayPtr);
			ui_mappingMatrix.populatePresets(mapPresetPtr,mpArrayPtr,apArrayPtr);
			ui_mappingMatrix.preset_Name.setText("");
		};

		ui_mappingMatrix.mp_isVisible.addListener(this);
		ui_mappingMatrix.mp_isHidden.addListener(this);
		ui_mappingMatrix.ap_isVisible.addListener(this);
		ui_mappingMatrix.ap_isHidden.addListener(this);
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
			text = sensInfoPtr->isOnline[i] ? "ON" : "OFF";

			if (sensInfoPtr->isOnline[i])
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
			isSensorOnline = sensInfoPtr->isOnline[i];
			batteryLevel = sensInfoPtr->batteryPercent[i];
			batteryText = isSensorOnline ? String(batteryLevel) : "N/A";
			ui_sensorCon.BatteryLevel[i].setText(batteryText, dontSendNotification);
			if (sensInfoPtr->isOnline[i])
				ui_sensorCon.BiasComp[i].setVisible(true);
			else
				ui_sensorCon.BiasComp[i].setVisible(false);
			if (oscSensPtr[i].isBiasComp_ON)
			{
				ui_sensorCon.BiasComp[i].setButtonText("Calibrating");
				ui_sensorCon.BiasComp[i].setColour(ui_sensorCon.BiasComp[i].buttonColourId, Colours::blue);
			}
			if (oscSensPtr[i].isBiasComp_DONE)
			{
				ui_sensorCon.BiasComp[i].setButtonText("Calibrated");
				ui_sensorCon.BiasComp[i].setColour(ui_sensorCon.BiasComp[i].buttonColourId, Colours::green);
			}

			ui_sensorCon.PacketPercent[i].setText
			(
				String(oscSensPtr[i].oscSampleReceived_Percent, 2),
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
			colour_onlineIndicator = mAnalysisPtr->locationsOnline[i] != -1 ? Colours::green : Colours::red;
			// SET COLOUR OF ONLINE INDICATOR
			ui_movementAnalysis.IMU_Online_Indicator[i].setColour(ui_movementAnalysis.IMU_Online_Indicator[i].backgroundColourId, colour_onlineIndicator);
		}

		ui_movementAnalysis.update_Indicators_SensorOrientation(mpArrayPtr);

		// JOINT ANGLE DISPLAYS
		for (int j = 0; j < 3; j++)
		{
			ui_movementAnalysis.JointAngles[j].setText(
				String(mAnalysisPtr->jointAngles_Deg[j], 2)
				, dontSendNotification
			);

			ui_movementAnalysis.JointVelocities[j].setText(
				String(mAnalysisPtr->jointAngularVel_DegPerSec[j], 2)
				, dontSendNotification
			);

			switch (j)
			{
			case 0:
				ui_movementAnalysis.Joint_Range_AP_MovementRanges[j][0].setText("HIP     " +
					String(MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Angle Hip", "Min"),2),dontSendNotification);
				ui_movementAnalysis.Joint_Range_AP_MovementRanges[j][1].setText(
					String(MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Angle Hip", "Max"), 2), dontSendNotification);
				break;
			case 1:
				ui_movementAnalysis.Joint_Range_AP_MovementRanges[j][0].setText("KNEE    " +
					String(MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Angle Knee", "Min"), 2), dontSendNotification);
				ui_movementAnalysis.Joint_Range_AP_MovementRanges[j][1].setText(
					String(MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Angle Knee", "Max"), 2), dontSendNotification);
				break;
			case 2:
				ui_movementAnalysis.Joint_Range_AP_MovementRanges[j][0].setText("ANKLE   " +
					String(MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Angle Ankle", "Min"), 2), dontSendNotification);
				ui_movementAnalysis.Joint_Range_AP_MovementRanges[j][1].setText(
					String(MovementAnalysis::getMPVal_fromArray(mpArrayPtr, "Angle Ankle", "Max"), 2), dontSendNotification);
				break;
			}

			ui_movementAnalysis.Joint_Range_AP_HyperExtendThresh[j].setText(
				"MAX FLEX: " + String(mAnalysisPtr->jointAngles_thresh_MaxFlex[j], 2),
				dontSendNotification
			);
		}

		// DATA INPUT MODE
		ui_movementAnalysis.dataInput_Mode_Status.setText(
			mAnalysisPtr->dataInput_Mode_Names[mAnalysisPtr->dataInput_Mode]
			, dontSendNotification
		);
		
		// LOG PROGRESS
		ui_movementAnalysis.mpLog_File_Progress_VAL = mAnalysisPtr->mpFile_Streaming_Progress;

		// HANDLE FWD BUTTON PRESS
		if (ui_movementAnalysis.mpLog_File_FWD.isDown())
			mAnalysisPtr->mpFile_Streaming_Line_Current = (mAnalysisPtr->mpFile_Streaming_Line_Current + 20)
			//% mAnalysisPtr->mpFile_Streaming_Lines_Total;
			% mAnalysisPtr->imuLogFile_Streaming_Lines_Total[0];

		// HANDLE RWD BUTTON PRESS
		if (ui_movementAnalysis.mpLog_File_RWD.isDown())
			mAnalysisPtr->mpFile_Streaming_Line_Current = max(0,(mAnalysisPtr->mpFile_Streaming_Line_Current - 20));

		// STS PHASE DISPLAY
		ui_movementAnalysis.STS_Phase_Disp.setText(mAnalysisPtr->STS_Phases[(int)mpArrayPtr[10].value], dontSendNotification);

		// MADGWICK BETA
		ui_movementAnalysis.madgwick_Beta.setText("Beta: " + String(mAnalysisPtr->quaternionFilters[0].beta, 4), dontSendNotification);

		// REAL TIME VISUALIZE
		ui_movementAnalysis.updateSTSAnim(mpArrayPtr);
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
			mAnalysisPtr->numMovementParams,
			musControlPtr->numFbVariables,
			mpArrayPtr,
			apArrayPtr
		);
	}

	void mappingMatrix_resetLayout()
	{
		ui_mappingMatrix.mp_isVisible.clear();
		ui_mappingMatrix.mp_isHidden.clear();
		ui_mappingMatrix.ap_isVisible.clear();
		ui_mappingMatrix.ap_isHidden.clear();

		ui_mappingMatrix.setLayout(interface_Width, interface_Height, mAnalysisPtr->numMovementParams,
			musControlPtr->numFbVariables, mpArrayPtr, apArrayPtr);

		ui_mappingMatrix.toggleVisible(true, mpArrayPtr, apArrayPtr);

		ui_mappingMatrix.mp_isVisible.addItem("VISIBLE MP", 100);	
		ui_mappingMatrix.mp_isVisible.setSelectedId(100);
		ui_mappingMatrix.mp_isHidden.addItem("HIDDEN MP", 100);		
		ui_mappingMatrix.mp_isHidden.setSelectedId(100);
		ui_mappingMatrix.ap_isVisible.addItem("VISIBLE AP", 100);	
		ui_mappingMatrix.ap_isVisible.setSelectedId(100);
		ui_mappingMatrix.ap_isHidden.addItem("HIDDEN AP", 100);		
		ui_mappingMatrix.ap_isHidden.setSelectedId(100);
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StsinteractionAudioProcessorEditor)
};
