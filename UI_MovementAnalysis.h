#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MovementAnalysis.h"

class UI_MovementAnalysis
{
public:
	UI_MovementAnalysis() : mpLog_File_Progress(mpLog_File_Progress_VAL)
	{
		configure();
	}

	~UI_MovementAnalysis()
	{}

	// MEMBER UI ELEMENTS
	// STEPS TO ADD NEW CONTROLS
	// Declare here
	// Add and make visible in PluginEditor.h (add to existing function)
	// Initialize here
	// Initialize in PluginEditor.h
	// Add SetVisible to ToggleVisible() here
	// Set position in SetLayout() here

	// STRINGS CONTAINING TEXT INFO
	String IMULocations[3] = { "Trunk","Thigh","Shank" };
	String JointNames[3] = {"Hip","Knee","Ankle"};
	String IMU_Mount_Side_Options[2] = { "Front", "Side" };
	String IMU_Polarity_Options[2] = { "+", "-" };
	short IMU_Mount_Side_Options_NUM = 2;
	short IMU_Polarity_Options_NUM = 2;

	// IMU STATUS, AXIS SETTING, POLARITY, RANGES AND ANGLE DISPLAY

	// POSITION AND BOUNDS
	int IMU_Config_StartY = 70;
	int IMU_Config_Column_StartPos[10] =
	{ 50, 150, 250, 280, 380, 430, 610, 670, 720, 890};
	int IMU_Config_Column_Width[10] =
	{ 90, 90, 20, 90, 45, 175, 45, 55, 175, 45};
	int IMU_Config_Row_Height = 20;
	int IMU_Config_Row_Offset = 35;
	int IMU_Config_Indicator_Side = 5;

	Label IMU_Online_Indicator[3];				// 1
	ComboBox IMU_Mount_Side[3];					// 2
	ComboBox IMU_Polarity[3];					// 3
	Label IMU_Location[3];						// 4

	// QUATERNION BETA SLIDER
	Slider gyrMeasError;
	Label madgwick_Beta;
	
	Label AP;
	Slider IMU_range_segmentAngles_AP[3];		// 6
	Label IMU_segmentAngles_AP_Indicator[3];
	Label IMU_segmentRanges_AP_Bounds[3][2];	// 5, 7
	
	Label ML;
	Slider IMU_range_segmentAngles_ML[3];		// 9
	Label IMU_segmentAngles_ML_Indicator[3];
	Label IMU_segmentRanges_ML_Bounds[3][2];	// 8, 10

	Label JointAngles_Indicators[3];
	Slider Joint_Range_AP[3];					// 0 = HIP, 1 = KNEE
	Label Joint_Range_AP_MovementRanges[3][2];   // 0 = HIP, 1 = KNEE
	Label Joint_Range_AP_HyperExtendThresh[3]; // o = HIP, 1 = KNEE

	// CoM

	Slider CoM_Disp_Bounds_H;
	Label CoM_Disp_Bounds_H_Lab;
	Label CoM_Disp_Bounds_H_MIN;
	Label CoM_Disp_Bounds_H_MAX;
	Label CoM_Disp_INDIC_VAL_H;

	Slider CoM_Disp_Bounds_V;
	Label CoM_Disp_Bounds_V_Lab;
	Label CoM_Disp_Bounds_V_MIN;
	Label CoM_Disp_Bounds_V_MAX;
	Label CoM_Disp_INDIC_VAL_V;

	Slider BoS_Thresh_Adjust;

	TextButton cal_CoM_SIT;
	TextButton cal_CoM_STAND;

	Slider CoM_Wt_Dist;
	Label CoM_Wt_Dist_LAB;
	Label CoM_Wt_Dist_Vals[3];

	// MP LOG STREAMING CONTROLS
	Label dataInput_Mode_Status;
	TextButton mpLog_File_Load_Unload;
	TextButton mpLog_File_Play_Pause;
	TextButton mpLog_File_Stop;
	ProgressBar mpLog_File_Progress;
	double mpLog_File_Progress_VAL = 0;
	TextButton mpLog_File_FWD;
	TextButton mpLog_File_RWD;

	void update_Indicators_SensorOrientation(MovementParameter mpArray[])
	{
		int indicatorWidth = 5;
		int indicatorHeight = 8;

		double indicator_offset_Px = 0;

		double ap_Array[3] =
		{
			MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Trunk AP", "Val"),
			MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Thigh AP", "Val"),
			MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Shank AP", "Val"),
		};

		double ml_Array[3] =
		{
			MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Trunk ML", "Val"),
			MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Thigh ML", "Val"),
			MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Shank ML", "Val"),
		};

		double jointAngle_Array[3] = 
		{
			MovementAnalysis::getMPVal_fromArray(mpArray, "Angle Hip", "Val"),
			MovementAnalysis::getMPVal_fromArray(mpArray, "Angle Knee", "Val"),
			MovementAnalysis::getMPVal_fromArray(mpArray, "Angle Ankle", "Val"),
		};

		for (int i = 0; i < 3; i++)
		{
			indicator_offset_Px = (ap_Array[i] - (-90)) / 180.0 * IMU_Config_Column_Width[5];
			// ADJUST INDICATOR POSITION - AP
			IMU_segmentAngles_AP_Indicator[i].setBounds(
				IMU_Config_Column_StartPos[5] + indicator_offset_Px,
				IMU_Config_StartY + IMU_Config_Row_Offset * i - 10,
				indicatorWidth,
				indicatorHeight
			);
			
			indicator_offset_Px = (ml_Array[i]) / 90.0 * IMU_Config_Column_Width[8];
			// ADJUST INDICATOR POSITION - ML
			IMU_segmentAngles_ML_Indicator[i].setBounds(
				IMU_Config_Column_StartPos[8] + indicator_offset_Px,
				IMU_Config_StartY + IMU_Config_Row_Offset * i - 10,
				indicatorWidth,
				indicatorHeight
			);

			// ADJUST BOUNDS TEXT
			IMU_segmentRanges_AP_Bounds[i][0].setText(String(mpArray[i].minVal, 2), dontSendNotification);
			IMU_segmentRanges_AP_Bounds[i][1].setText(String(mpArray[i].maxVal, 2), dontSendNotification);

			IMU_segmentRanges_ML_Bounds[i][0].setText(String(mpArray[i+3].minVal, 2), dontSendNotification);
			IMU_segmentRanges_ML_Bounds[i][1].setText(String(mpArray[i+3].maxVal, 2), dontSendNotification);
		}

		// JOINT ANGLE INDICATORS
		for (int i = 0; i < 3; i++)
		{
			indicator_offset_Px = (jointAngle_Array[i] - (-10)) / 190.0 * IMU_Config_Column_Width[5];
			JointAngles_Indicators[i].setBounds(
				IMU_Config_Column_StartPos[5] + indicator_offset_Px,
				IMU_Config_StartY + IMU_Config_Row_Offset * 2 * i + 160,
				indicatorWidth,
				indicatorHeight
			);
		}

		float CoM_H_MIN = MovementAnalysis::getMPVal_fromArray(mpArray, "Horiz Disp", "Min");
		float CoM_H_MAX = MovementAnalysis::getMPVal_fromArray(mpArray, "Horiz Disp", "Max");
		float CoM_V_MIN = MovementAnalysis::getMPVal_fromArray(mpArray, "Verti Disp", "Min");
		float CoM_V_MAX = MovementAnalysis::getMPVal_fromArray(mpArray, "Verti Disp", "Max");

		CoM_Disp_Bounds_H_MIN.setText("CoM_H " + String(CoM_H_MIN, 2), dontSendNotification);
		CoM_Disp_Bounds_H_MAX.setText(String(CoM_H_MAX, 2), dontSendNotification);
		CoM_Disp_Bounds_V_MIN.setText("CoM_V " + String(CoM_V_MIN, 2), dontSendNotification);
		CoM_Disp_Bounds_V_MAX.setText(String(CoM_V_MAX, 2), dontSendNotification);

		float horiz_Disp_Px_Offset = (MovementAnalysis::getMPVal_fromArray(mpArray, "Horiz Disp", "Val") + 1) * 0.5
									 * IMU_Config_Column_Width[5];

		float verti_Disp_Px_Offset = MovementAnalysis::getMPVal_fromArray(mpArray, "Verti Disp", "Val")
			* IMU_Config_Column_Width[5];

		// ADJUST INDICATOR POSITION - HORIZ DISP
		CoM_Disp_INDIC_VAL_H.setBounds(
			IMU_Config_Column_StartPos[5] + horiz_Disp_Px_Offset,
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 3 + 160,
			indicatorWidth,
			indicatorHeight
		);

		// ADJUST INDICATOR POSITION - VERTI DISP
		CoM_Disp_INDIC_VAL_V.setBounds(
			IMU_Config_Column_StartPos[5] + verti_Disp_Px_Offset,
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 4 + 160,
			indicatorWidth,
			indicatorHeight
		);
	}

	void update_WeightDist(float wtFracs[])
	{
		for (int i = 0; i < 3; i++)
			CoM_Wt_Dist_Vals[i].setText(String(wtFracs[i], 2), dontSendNotification);
	}

	// RECORD LOG, OPERATION MODE AND ORIENTATION CALCULATION ALGORITHM
	TextButton record_MovementLog;
	ComboBox operationMode;
	ComboBox orientationAlgo;

	// HIGH LEVEL PARAMETERS
	Label JointAngles[3];						// 0 = HIP	 // 1 = KNEE	// 2 = ANKLE
	Label JointVelocities[3];					// 0 = HIP	 // 1 = KNEE	// 2 = ANKLE
	Label STS_Phase_Disp;						

	// STS SIMULATION
	Slider simulation_OrientAngles[3];
	Label  simulation_OrientAngles_LAB[3];
	int simulation_OrientAngles_Ranges[3][2] =
	{
		{-30,90},
		{-90,0},
		{-90,90}
	};

	void configure()
	{
		// STS ANIMATION COLOURS
		for (int i = 0; i < 4; i++)
			stsAnim_joint[i].setColour(stsAnim_joint[i].backgroundColourId, Colours::blue);
		for (int j = 0; j < 20; j++)
		{
			for (int k = 0; k < 3; k++)
			stsAnim_Segments[k][j].setColour(stsAnim_Segments[k][j].backgroundColourId, Colours::blue);
		}

		// HEADERS
		ML.setText("ML", dontSendNotification);
		ML.setJustificationType(Justification::centred);
		AP.setText("AP", dontSendNotification);
		AP.setJustificationType(Justification::centred);

		// IMU CONFIG CONTROLS INITIALIZATION
		for (int i = 0; i < 3; i++)
		{
			// 0 - MOUNTING SIDE
			for (int j = 0; j < IMU_Mount_Side_Options_NUM; j++)
				IMU_Mount_Side[i].addItem(IMU_Mount_Side_Options[j], j + 1);

			// 1 - IMU POLARITY
			for (int k = 0; k < IMU_Polarity_Options_NUM; k++)
				IMU_Polarity[i].addItem(IMU_Polarity_Options[k], k + 1);

			// 2 - IMU STATUS INDICATOR - NO CONFIG NEEDED

			// 3 - IMU LOCATION LABELS
			IMU_Location[i].setText(IMULocations[i], dontSendNotification);

			// 4 - AP Lower Bound - No Config Needed

			// 5 - AP Range Setter
			IMU_range_segmentAngles_AP[i].setRange(-90, 90);
			IMU_segmentAngles_AP_Indicator[i].attachToComponent(&IMU_range_segmentAngles_AP[i], true);
			IMU_range_segmentAngles_AP[i].setSliderStyle(Slider::SliderStyle::ThreeValueHorizontal);
			IMU_range_segmentAngles_AP[i].setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 30, 20);
			IMU_range_segmentAngles_AP[i].setColour(IMU_range_segmentAngles_AP[i].trackColourId, Colours::yellow);
			IMU_range_segmentAngles_AP[i].setColour(IMU_range_segmentAngles_AP[i].backgroundColourId, Colours::blue);

			// 6 - AP Upper Bound - No Config Needed

			// INDICATORS
			IMU_segmentAngles_AP_Indicator[i].setColour(IMU_segmentAngles_AP_Indicator[i].backgroundColourId, Colours::yellow);
			IMU_segmentAngles_ML_Indicator[i].setColour(IMU_segmentAngles_ML_Indicator[i].backgroundColourId, Colours::yellow);

			// 7 - ML Lower Bound - No Config Needed

			// 8 - ML Range Setter
			IMU_range_segmentAngles_ML[i].setRange(0, 90);
			IMU_segmentAngles_ML_Indicator[i].attachToComponent(&IMU_range_segmentAngles_ML[i], true);
			IMU_range_segmentAngles_ML[i].setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
			IMU_range_segmentAngles_ML[i].setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 30, 20);
			IMU_range_segmentAngles_ML[i].setColour(IMU_range_segmentAngles_ML[i].trackColourId, Colours::yellow);
			IMU_range_segmentAngles_ML[i].setColour(IMU_range_segmentAngles_ML[i].backgroundColourId, Colours::blue);

			// 9 - ML Upper Bound - No Config Needed

			// IMU Config - Slider Simulation Config
			simulation_OrientAngles[i].setRange(simulation_OrientAngles_Ranges[i][0], 
				simulation_OrientAngles_Ranges[i][1]);
			if (i != 1) simulation_OrientAngles[i].setValue(0);
			else simulation_OrientAngles[i].setValue(-90);
			simulation_OrientAngles_LAB[i].attachToComponent(&simulation_OrientAngles[i], true);
			simulation_OrientAngles_LAB[i].setText(IMULocations[i], dontSendNotification);
			simulation_OrientAngles[i].setNumDecimalPlacesToDisplay(0);
			simulation_OrientAngles[i].setTextBoxStyle(
				Slider::TextEntryBoxPosition::TextBoxLeft,
				false,
				30, 20
			);
		}

		// JOINT RANGE INITIALIZATION
		for (int i = 0; i < 3; i++)
		{
			JointAngles_Indicators[i].setColour(JointAngles_Indicators[i].backgroundColourId, Colours::yellow);
			Joint_Range_AP[i].setRange(-10, 180);
			Joint_Range_AP[i].setSliderStyle(Slider::SliderStyle::ThreeValueHorizontal);
			Joint_Range_AP[i].setColour(Joint_Range_AP[i].trackColourId, Colours::yellow);
			Joint_Range_AP[i].setColour(Joint_Range_AP[i].backgroundColourId, Colours::yellow);
			Joint_Range_AP[i].setTextBoxStyle(Slider::NoTextBox, true, 10, 10);
		}

		// MP Log Streaming
		mpLog_File_Load_Unload.setButtonText("Load Log");
		mpLog_File_Load_Unload.setColour(mpLog_File_Load_Unload.buttonColourId, Colours::blue);
		mpLog_File_Play_Pause.setButtonText("Loop Play Data");
		mpLog_File_Play_Pause.setColour(mpLog_File_Play_Pause.buttonColourId, Colours::green);
		mpLog_File_Stop.setButtonText("Stop");
		mpLog_File_Stop.setColour(mpLog_File_Stop.buttonColourId, Colours::red);
		mpLog_File_Progress.setColour(mpLog_File_Progress.foregroundColourId, Colours::yellow);
		mpLog_File_Progress.setColour(mpLog_File_Progress.backgroundColourId, Colours::blue);
		mpLog_File_Progress.setPercentageDisplay(false);
		mpLog_File_FWD.setColour(mpLog_File_FWD.buttonColourId, Colours::blue);
		mpLog_File_FWD.setButtonText("FWD");
		mpLog_File_RWD.setColour(mpLog_File_RWD.buttonColourId, Colours::blue);
		mpLog_File_RWD.setButtonText("RWD");

		// Record Movement Log
		record_MovementLog.setColour(record_MovementLog.buttonColourId, Colours::red);
		record_MovementLog.setButtonText("Record");

		//JointAngles[0].attachToComponent(&stsAnim_joint[1], false);	

		// Madgwick Gyr Error
		gyrMeasError.setRange(2.1833, 40);
		gyrMeasError.setValue(40);
		gyrMeasError.setColour(gyrMeasError.backgroundColourId, Colours::blue);
		gyrMeasError.setColour(gyrMeasError.trackColourId, Colours::yellow);
		gyrMeasError.setSkewFactor(0.2);
		gyrMeasError.setTextBoxStyle(Slider::NoTextBox, false, 10, 10);
		madgwick_Beta.attachToComponent(&gyrMeasError, false);

		stsAnim_CoM_Indicator.setColour(stsAnim_CoM_Indicator.backgroundColourId, Colours::yellow);

		// CoM DISP SLIDERS

		// HORIZONTAL
		CoM_Disp_Bounds_H.setRange(-1, 1);
		CoM_Disp_Bounds_H.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		CoM_Disp_Bounds_H.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 30, 20);
		CoM_Disp_Bounds_H.setColour(CoM_Disp_Bounds_H.trackColourId, Colours::yellow);
		CoM_Disp_Bounds_H.setColour(CoM_Disp_Bounds_H.backgroundColourId, Colours::blue);
		CoM_Disp_INDIC_VAL_H.setColour(CoM_Disp_INDIC_VAL_H.backgroundColourId, Colours::yellow);

		// VERTICAL
		CoM_Disp_Bounds_V.setRange(0, 1);
		CoM_Disp_Bounds_V.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		CoM_Disp_Bounds_V.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 30, 20);
		CoM_Disp_Bounds_V.setColour(CoM_Disp_Bounds_V.trackColourId, Colours::yellow);
		CoM_Disp_Bounds_V.setColour(CoM_Disp_Bounds_V.backgroundColourId, Colours::blue);
		CoM_Disp_INDIC_VAL_V.setColour(CoM_Disp_INDIC_VAL_V.backgroundColourId, Colours::yellow);

		CoM_Wt_Dist.setRange(0, 0.97);
		CoM_Wt_Dist.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		CoM_Wt_Dist.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 30, 20);
		CoM_Wt_Dist.setColour(CoM_Wt_Dist.trackColourId, Colours::yellow);
		CoM_Wt_Dist.setColour(CoM_Wt_Dist.backgroundColourId, Colours::blue);
		CoM_Wt_Dist_LAB.setText("Weight Dist", dontSendNotification);
		CoM_Wt_Dist_LAB.attachToComponent(&CoM_Wt_Dist, true);

		// CALIBRATE CoM
		cal_CoM_SIT.setButtonText("Calibrate Sit");
		cal_CoM_SIT.setColour(cal_CoM_SIT.buttonColourId, Colours::blue);

		cal_CoM_STAND.setButtonText("Calibrate Stand");
		cal_CoM_STAND.setColour(cal_CoM_STAND.buttonColourId, Colours::blue);

		stsAnim_CoM_RangeZone.setColour(stsAnim_CoM_RangeZone.backgroundColourId, Colour::fromFloatRGBA(1, 1, 0, 0.25));

		stsAnim_Line_BoS.setColour(stsAnim_Line_BoS.backgroundColourId, Colours::white);
		stsAnim_Line_BoS_Warning.setColour(stsAnim_Line_BoS_Warning.backgroundColourId, Colours::orange);

		// BoS Thresh Adjust
		BoS_Thresh_Adjust.setRange(-0.5, 0.5);
		BoS_Thresh_Adjust.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		BoS_Thresh_Adjust.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 30, 20);
		BoS_Thresh_Adjust.setColour(CoM_Disp_Bounds_V.trackColourId, Colours::yellow);
		BoS_Thresh_Adjust.setColour(CoM_Disp_Bounds_V.backgroundColourId, Colours::blue);
	}

	void toggleVisible(bool on, short dataInputMode)
	{
		for (int i = 0; i < 3; i++)
		{
			IMU_Mount_Side[i].setVisible(on);					// 0
			IMU_Polarity[i].setVisible(on);						// 1
			IMU_Online_Indicator[i].setVisible(on);				// 2
			IMU_Location[i].setVisible(on);						// 3
			IMU_segmentRanges_AP_Bounds[i][0].setVisible(on);	// 4
			IMU_range_segmentAngles_AP[i].setVisible(on);		// 5
			IMU_segmentRanges_AP_Bounds[i][1].setVisible(on);	// 6
			IMU_segmentRanges_ML_Bounds[i][0].setVisible(on);	// 7
			IMU_range_segmentAngles_ML[i].setVisible(on);		// 8
			IMU_segmentRanges_ML_Bounds[i][1].setVisible(on);	// 9
			simulation_OrientAngles[i].setVisible(on);			// X
		}

		for (int i = 0; i < 3; i++)
		{
			Joint_Range_AP[i].setVisible(on);
			JointAngles_Indicators[i].setVisible(on);
			Joint_Range_AP_MovementRanges[i][0].setVisible(on);
			Joint_Range_AP_MovementRanges[i][1].setVisible(on);
			Joint_Range_AP_HyperExtendThresh[i].setVisible(on);
		}

		gyrMeasError.setVisible(on);

		// MP LOG STREAMING
		dataInput_Mode_Status.setVisible(on);
		mpLog_File_Load_Unload.setVisible(on);
		mpLog_File_Play_Pause.setVisible(on && (dataInputMode > 0));
		mpLog_File_Stop.setVisible(on && (dataInputMode == 2));
		mpLog_File_Progress.setVisible(on && (dataInputMode > 0));
		mpLog_File_FWD.setVisible(on && (dataInputMode == 2));
		mpLog_File_RWD.setVisible(on && (dataInputMode == 2));

		// MISCELLANEOUS
		ML.setVisible(on);
		AP.setVisible(on);
		orientationAlgo.setVisible(on);
		operationMode.setVisible(on);	
		record_MovementLog.setVisible(on);

		// CoM BOUND SLIDERS
		CoM_Disp_Bounds_H.setVisible(on);
		CoM_Disp_Bounds_H_Lab.setVisible(on);
		CoM_Disp_Bounds_H_MIN.setVisible(on);
		CoM_Disp_Bounds_H_MAX.setVisible(on);
		CoM_Disp_INDIC_VAL_H.setVisible(on);
		
		CoM_Disp_Bounds_V.setVisible(on);
		CoM_Disp_Bounds_V_Lab.setVisible(on);
		CoM_Disp_Bounds_V_MIN.setVisible(on);
		CoM_Disp_Bounds_V_MAX.setVisible(on);
		CoM_Disp_INDIC_VAL_V.setVisible(on);
		CoM_Wt_Dist.setVisible(on);
		CoM_Wt_Dist_LAB.setVisible(on);
		for (int i = 0; i < 3; i++) CoM_Wt_Dist_Vals[i].setVisible(on);

		// STS VISUALIZER
		STS_Phase_Disp.setVisible(on);
		for (int j = 0; j < 3; j++)
		{
			JointAngles[j].setVisible(on);
			JointVelocities[j].setVisible(on);
		}
		for (int i = 0; i < 4; i++)
			stsAnim_joint[i].setVisible(on);
		for (int j = 0; j < 20; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				stsAnim_Segments[k][j].setVisible(on);
			}
		}
		stsAnim_CoM_RangeZone.setVisible(on);
		stsAnim_CoM_Indicator.setVisible(on);
		stsAnim_Line_BoS.setVisible(on);
		stsAnim_Line_BoS_Warning.setVisible(on);
		BoS_Thresh_Adjust.setVisible(on);
		cal_CoM_SIT.setVisible(on);
		cal_CoM_STAND.setVisible(on);
	}

	void setLayout()
	{
		for (int i = 0; i < 10; i++) IMU_Config_Column_StartPos[i] -= 40;

		// IMU CONFIG CONTROLS
		for (int i = 0; i < 3; i++)
		{
			IMU_Mount_Side[i].setBounds(
				IMU_Config_Column_StartPos[0],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[0],
				IMU_Config_Row_Height
			);

			IMU_Polarity[i].setBounds(
				IMU_Config_Column_StartPos[1],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[1],
				IMU_Config_Row_Height
			);

			IMU_Online_Indicator[i].setBounds(
				IMU_Config_Column_StartPos[2],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[2],
				IMU_Config_Row_Height
			);

			IMU_Location[i].setBounds(
				IMU_Config_Column_StartPos[3],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[3],
				IMU_Config_Row_Height
			);

			IMU_segmentRanges_AP_Bounds[i][0].setBounds(
				IMU_Config_Column_StartPos[4],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[4],
				IMU_Config_Row_Height
			);

			AP.setBounds(
				IMU_Config_Column_StartPos[5],
				IMU_Config_StartY - 35,
				IMU_Config_Column_Width[5],
				IMU_Config_Row_Height
			);

			IMU_range_segmentAngles_AP[i].setBounds(
				IMU_Config_Column_StartPos[5],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[5],
				IMU_Config_Row_Height
			);

			IMU_segmentRanges_AP_Bounds[i][1].setBounds(
				IMU_Config_Column_StartPos[6],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[6],
				IMU_Config_Row_Height
			);

			IMU_segmentRanges_ML_Bounds[i][0].setBounds(
				IMU_Config_Column_StartPos[7],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[7],
				IMU_Config_Row_Height
			);

			ML.setBounds(
				IMU_Config_Column_StartPos[8],
				IMU_Config_StartY - 35,
				IMU_Config_Column_Width[8],
				IMU_Config_Row_Height
			);

			IMU_range_segmentAngles_ML[i].setBounds(
				IMU_Config_Column_StartPos[8],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[8],
				IMU_Config_Row_Height
			);

			IMU_segmentRanges_ML_Bounds[i][1].setBounds(
				IMU_Config_Column_StartPos[9],
				IMU_Config_StartY + IMU_Config_Row_Offset * i,
				IMU_Config_Column_Width[9],
				IMU_Config_Row_Height
			);
		}

		// JOINT RANGE CONTROLS
		for (int i = 0; i < 3; i++)
		{
			Joint_Range_AP_MovementRanges[i][0].setBounds(
				IMU_Config_Column_StartPos[4] - 40,
				IMU_Config_StartY + IMU_Config_Row_Offset * 2 * i + 170,
				IMU_Config_Column_Width[4] + 40,
				IMU_Config_Row_Height
			);

			Joint_Range_AP[i].setBounds(
				IMU_Config_Column_StartPos[5],
				IMU_Config_StartY + IMU_Config_Row_Offset * 2 * i + 170,
				IMU_Config_Column_Width[5],
				IMU_Config_Row_Height
			);

			Joint_Range_AP_MovementRanges[i][1].setBounds(
				IMU_Config_Column_StartPos[6],
				IMU_Config_StartY + IMU_Config_Row_Offset * 2 * i + 170,
				IMU_Config_Column_Width[6],
				IMU_Config_Row_Height
			);

			Joint_Range_AP_HyperExtendThresh[i].setBounds(
				IMU_Config_Column_StartPos[5] + IMU_Config_Column_Width[5] / 2 - 60,
				IMU_Config_StartY + IMU_Config_Row_Offset * 2 * i + IMU_Config_Row_Offset + 160,
				120,
				IMU_Config_Row_Height
			);
		}

		BoS_Thresh_Adjust.setBounds(
			stsAnim_startX - 116,
			stsAnim_startY - 232,
			232,
			10
		);

		//// MP STREAMING
		dataInput_Mode_Status.setBounds(
			IMU_Config_Column_StartPos[4],
			175,
			IMU_Config_Column_StartPos[5] + IMU_Config_Column_Width[5] - IMU_Config_Column_StartPos[4],
			20
		);
		
		mpLog_File_Load_Unload.setBounds(
			IMU_Config_Column_StartPos[6],
			175,
			IMU_Config_Column_StartPos[7] + IMU_Config_Column_Width[7] - IMU_Config_Column_StartPos[6] - 20,
			20
		);

		mpLog_File_Play_Pause.setBounds(
			IMU_Config_Column_StartPos[4],
			205,
			IMU_Config_Column_StartPos[5] + IMU_Config_Column_Width[5] - IMU_Config_Column_StartPos[4],
			20
		);

		mpLog_File_Stop.setBounds(
			IMU_Config_Column_StartPos[6],
			205,
			IMU_Config_Column_StartPos[7] + IMU_Config_Column_Width[7] - IMU_Config_Column_StartPos[6] - 20,
			20
		);
		
		mpLog_File_Progress.setBounds(
			IMU_Config_Column_StartPos[8],
			175,
			IMU_Config_Column_StartPos[9] + IMU_Config_Column_Width[9] - IMU_Config_Column_StartPos[8] - 20,
			20
		);

		mpLog_File_RWD.setBounds(
			IMU_Config_Column_StartPos[8],
			205,
			60,
			20
		);

		mpLog_File_FWD.setBounds(
			IMU_Config_Column_StartPos[8] + 130,
			205,
			60,
			20
		);

		// CoM DISP BOUNDS
		CoM_Disp_Bounds_H.setBounds(
			IMU_Config_Column_StartPos[5],
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 3 + 170,
			IMU_Config_Column_Width[5],
			IMU_Config_Row_Height
		);

		CoM_Disp_Bounds_V.setBounds(
			IMU_Config_Column_StartPos[5],
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 4 + 170,
			IMU_Config_Column_Width[5],
			IMU_Config_Row_Height
		);

		CoM_Disp_Bounds_H_MIN.setBounds(
			IMU_Config_Column_StartPos[4] - 40,
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 3 + 170,
			IMU_Config_Column_Width[4] + 40,
			IMU_Config_Row_Height
		);

		CoM_Disp_Bounds_H_MAX.setBounds(
			IMU_Config_Column_StartPos[6],
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 3 + 170,
			IMU_Config_Column_Width[6],
			IMU_Config_Row_Height
		);

		CoM_Disp_Bounds_V_MIN.setBounds(
			IMU_Config_Column_StartPos[4] - 40,
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 4 + 170,
			IMU_Config_Column_Width[4] + 40,
			IMU_Config_Row_Height
		);

		CoM_Disp_Bounds_V_MAX.setBounds(
			IMU_Config_Column_StartPos[6],
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 4 + 170,
			IMU_Config_Column_Width[6],
			IMU_Config_Row_Height
		);

		CoM_Wt_Dist.setBounds(
			IMU_Config_Column_StartPos[5],
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 4 + 210,
			IMU_Config_Column_Width[5],
			IMU_Config_Row_Height
		);

		for (int i = 0; i < 3; i++)
			CoM_Wt_Dist_Vals[i].setBounds(
				IMU_Config_Column_StartPos[5] + i / 3.0 * IMU_Config_Column_Width[5],
				IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 4 + 230,
				40,
				15
			);

		cal_CoM_SIT.setBounds(
			IMU_Config_Column_StartPos[5],
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 5 + 210,
			IMU_Config_Column_Width[5],
			IMU_Config_Row_Height
		);

		cal_CoM_STAND.setBounds(
			IMU_Config_Column_StartPos[5],
			IMU_Config_StartY + IMU_Config_Row_Offset * 2 * 5 + 235,
			IMU_Config_Column_Width[5],
			IMU_Config_Row_Height
		);

		// MISCELLANEOUS
		gyrMeasError.setBounds(10, 600, 200, 50);
		operationMode.setBounds(10, 175, 200, 20);
		orientationAlgo.setBounds(10, 205, 200, 20);
		record_MovementLog.setBounds(10, 220, 200, 40);
	}

	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
	// REAL TIME VISUALIZER

	float stsAnim_startX = 150;
	float stsAnim_startY = 530;

	// JOINTS
	Label stsAnim_joint[4];
	int stsAnim_Offset_Joint_X[4] = { 0 };
	int stsAnim_Offset_Joint_Y[4] = { 0 };
	int stsAnim_joint_Width = 20;
	int stsAnim_joint_Height = 20;

	// BODY SEGMENTS

	int stsAnim_Segment_numPix = 20;
	Label stsAnim_Segments[3][20];
	int stsAnim_Segments_PixelSide = 13;
	int stsAnim_Segments_Offset_X[3][20] = { 0 };
	int stsAnim_Segments_Offset_Y[3][20] = { 0 };
	float segmentWise_HtPct[3] = { 0.25, 0.241, 0.402 };		// Shank Thigh Trunk

	Label stsAnim_CoM_Indicator;
	Label stsAnim_CoM_RangeZone;

	Label stsAnim_Line_BoS;
	Label stsAnim_Line_BoS_Warning;

	void updateSTSAnim(MovementParameter mpArray[], float thresh_BoS, float thresh_BoS_Warning)
	{
		float angle_Trunk_AP = MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Trunk AP", "Val");
		float angle_Thigh_AP = MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Thigh AP", "Val");
		float angle_Shank_AP = MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Shank AP", "Val");
		float angle_Trunk_ML = MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Trunk ML", "Val");
		float angle_Thigh_ML = MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Thigh ML", "Val");
		float angle_Shank_ML = MovementAnalysis::getMPVal_fromArray(mpArray, "Orientation Shank ML", "Val");

		float CoM_Disp_Horiz_AP = MovementAnalysis::getMPVal_fromArray(mpArray, "Horiz Disp", "Val");
		float CoM_Disp_Vert = MovementAnalysis::getMPVal_fromArray(mpArray, "Verti Disp", "Val");
		float CoM_H_Min = MovementAnalysis::getMPVal_fromArray(mpArray, "Horiz Disp", "Min");
		float CoM_H_Max = MovementAnalysis::getMPVal_fromArray(mpArray, "Horiz Disp", "Max");
		float CoM_V_Min = MovementAnalysis::getMPVal_fromArray(mpArray, "Verti Disp", "Min");
		float CoM_V_Max = MovementAnalysis::getMPVal_fromArray(mpArray, "Verti Disp", "Max");

		bool isExt_Knee = MovementAnalysis::getMPVal_fromArray(mpArray, "Extend Knee", "Val") == 0 ? false : true;
		stsAnim_joint[1].setColour(stsAnim_joint[1].backgroundColourId, isExt_Knee ? Colours::yellow : Colours::red);
		bool isExt_Hip = MovementAnalysis::getMPVal_fromArray(mpArray, "Extend Hip", "Val") == 0 ? false : true;
		stsAnim_joint[2].setColour(stsAnim_joint[2].backgroundColourId, isExt_Hip ? Colours::yellow : Colours::red);

		float bodySeg_PxSide[3] = { 0.0, 0.0, 0.0 };
		float bodyLengthPx = 0;
		for (int i = 0; i < 3; i++)
		{
			bodySeg_PxSide[i] = stsAnim_Segments_PixelSide * segmentWise_HtPct[i];
			bodyLengthPx += stsAnim_Segment_numPix * bodySeg_PxSide[i];
		}

		stsAnim_CoM_Indicator.setBounds(
			stsAnim_startX + CoM_Disp_Horiz_AP * bodyLengthPx,
			stsAnim_startY - CoM_Disp_Vert * bodyLengthPx
			, 5, 5
		);

		stsAnim_CoM_RangeZone.setBounds(
			stsAnim_startX + CoM_H_Min * bodyLengthPx,
			stsAnim_startY - CoM_V_Max * bodyLengthPx,
			(CoM_H_Max - CoM_H_Min) * bodyLengthPx + 5,
			(CoM_V_Max - CoM_V_Min) * bodyLengthPx + 5
		);

		// VECTORIZATION OF ANGLES AND SEGMENT LENGTHS
		float seg_AngleVec[3] = {angle_Shank_AP,angle_Thigh_AP,angle_Trunk_AP};
		float seg_Lengths[3] = { 0,0,0 };
		for (int i = 0; i < 3; i++)
			seg_Lengths[i] = bodySeg_PxSide[i] * stsAnim_Segment_numPix;
		

		float cumOffsets_X[4] = { 0.0,0.0,0.0,0.0 };
		float cumOffsets_Y[4] = { 0.0,0.0,0.0,0.0 };

		float jointStarts_X[4] = { 0.0,0.0,0.0,0.0 };
		float jointStarts_Y[4] = { 0.0,0.0,0.0,0.0 };

		for (int i = 0; i < 4; i++)				// SET POSITION OF EACH JOINT
		{
			float cumOffset_X = 0;
			float cumOffset_Y = 0;

			// CALCULATE CUMULATIVE X AND Y OFFSET
			for (int j = 0; j < i; j++)
			{
				cumOffset_X += seg_Lengths[j] * sin(seg_AngleVec[j] * M_PI / 180.0);
				cumOffset_Y -= seg_Lengths[j] * cos(seg_AngleVec[j] * M_PI / 180.0);
			}

			cumOffsets_X[i] = cumOffset_X;
			cumOffsets_Y[i] = cumOffset_Y;

			float jointStart_X = stsAnim_startX + cumOffset_X;
			float jointStart_Y = stsAnim_startY + cumOffset_Y;

			jointStarts_X[i] = jointStart_X;
			jointStarts_Y[i] = jointStart_Y;

			// SET BOUNDS
			stsAnim_joint[i].setBounds(	jointStart_X, jointStart_Y, stsAnim_joint_Width, stsAnim_joint_Height);

			if (i == 1)
			{
				JointAngles[1].setBounds(jointStart_X + stsAnim_joint_Width, jointStart_Y, 50, 20);
				JointVelocities[1].setBounds(jointStart_X + stsAnim_joint_Width, jointStart_Y + 15, 50, 20);
			}
			if (i == 2)
			{
				JointAngles[0].setBounds(jointStart_X - stsAnim_joint_Width - 30, jointStart_Y, 50, 20);
				JointVelocities[0].setBounds(jointStart_X - stsAnim_joint_Width - 30, jointStart_Y + 15, 50, 20);
			}

			if (i == 0)
			{
				JointAngles[2].setBounds(jointStart_X + stsAnim_joint_Width, jointStart_Y, 50, 20);
				JointVelocities[2].setBounds(jointStart_X + stsAnim_joint_Width, jointStart_Y + 15, 50, 20);
			}
		}

		for (int i = 0; i < 3; i++)				// SET POSITION OF EACH SEGMENT
		{
			float cumOffset_Px_X = 0.0;
			float cumOffset_Px_Y = 0.0;

			for (int j = 0; j < stsAnim_Segment_numPix; j++)		// SET POSITION OF EACH SEGMENT PIXEL
			{
				// CALCULATE X AND Y OFFSET
				cumOffset_Px_X = (cumOffsets_X[i+1] - cumOffsets_X[i]) * j / (double)stsAnim_Segment_numPix;
				cumOffset_Px_Y = (cumOffsets_Y[i+1] - cumOffsets_Y[i]) * j / (double)stsAnim_Segment_numPix;

				float pxStart_X = jointStarts_X[i] + cumOffset_Px_X;
				float pxStart_Y = jointStarts_Y[i] + cumOffset_Px_Y;

				// SET BOUNDS
				stsAnim_Segments[i][j].setBounds(pxStart_X, pxStart_Y, bodySeg_PxSide[i], bodySeg_PxSide[i]);
			}
		}

		// ADD CODE FOR BoS LINES
		stsAnim_Line_BoS.setBounds(stsAnim_startX + thresh_BoS* bodyLengthPx, stsAnim_startY - bodyLengthPx, 1, bodyLengthPx);
		
		stsAnim_Line_BoS_Warning.setBounds(stsAnim_startX + thresh_BoS_Warning* bodyLengthPx, stsAnim_startY - bodyLengthPx, 1, bodyLengthPx);
	}
		
};