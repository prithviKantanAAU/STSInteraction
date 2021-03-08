#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

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
	String JointNames[2] = {"Hip","Knee"};
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

	Slider Joint_Range_AP[2];					// 0 = HIP, 1 = KNEE
	Label Joint_Range_AP_MovementRanges[2][2];   // 0 = HIP, 1 = KNEE
	Label Joint_Range_AP_HyperExtendThresh[2]; // o = HIP, 1 = KNEE

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

		for (int i = 0; i < 3; i++)
		{
			indicator_offset_Px = (mpArray[i].value - (-90)) / 180.0 * IMU_Config_Column_Width[5];
			// ADJUST INDICATOR POSITION - AP
			IMU_segmentAngles_AP_Indicator[i].setBounds(
				IMU_Config_Column_StartPos[5] + indicator_offset_Px,
				IMU_Config_StartY + IMU_Config_Row_Offset * i - 10,
				indicatorWidth,
				indicatorHeight
			);
			
			indicator_offset_Px = (mpArray[i + 3].value) / 90.0 * IMU_Config_Column_Width[8];
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
	}

	// RECORD LOG, OPERATION MODE AND ORIENTATION CALCULATION ALGORITHM
	TextButton record_MovementLog;
	ComboBox operationMode;
	ComboBox orientationAlgo;

	// HIGH LEVEL PARAMETERS
	Label JointAngles[2];						// 0 = HIP	 // 1 = KNEE
	Label JointVelocities[2];					// 0 = HIP	 // 1 = KNEE
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
			stsAnim_trunk[j].setColour(stsAnim_trunk[j].backgroundColourId, Colours::blue);
			stsAnim_thigh[j].setColour(stsAnim_thigh[j].backgroundColourId, Colours::blue);
			stsAnim_shank[j].setColour(stsAnim_shank[j].backgroundColourId, Colours::blue);
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
			IMU_Mount_Side[i].setSelectedId(1);

			// 1 - IMU POLARITY
			for (int k = 0; k < IMU_Polarity_Options_NUM; k++)
				IMU_Polarity[i].addItem(IMU_Polarity_Options[k], k + 1);
			IMU_Polarity[i].setSelectedId(1);

			// 2 - IMU STATUS INDICATOR - NO CONFIG NEEDED

			// 3 - IMU LOCATION LABELS
			IMU_Location[i].setText(IMULocations[i], dontSendNotification);

			// 4 - AP Lower Bound - No Config Needed

			// 5 - AP Range Setter
			IMU_range_segmentAngles_AP[i].setRange(-90, 90);
			IMU_segmentAngles_AP_Indicator[i].attachToComponent(&IMU_range_segmentAngles_AP[i], true);
			IMU_range_segmentAngles_AP[i].setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
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
		for (int i = 0; i < 2; i++)
		{
			Joint_Range_AP[i].setRange(0, 220);
			Joint_Range_AP[i].setSliderStyle(Slider::SliderStyle::ThreeValueHorizontal);
			Joint_Range_AP[i].setColour(Joint_Range_AP[i].trackColourId, Colours::yellow);
			Joint_Range_AP[i].setColour(Joint_Range_AP[i].backgroundColourId, Colours::blue);
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

		JointAngles[0].attachToComponent(&stsAnim_joint[1], true);	

		// Madgwick Gyr Error
		gyrMeasError.setRange(0.01, 100);
		gyrMeasError.setValue(40);
		gyrMeasError.setColour(gyrMeasError.backgroundColourId, Colours::blue);
		gyrMeasError.setColour(gyrMeasError.trackColourId, Colours::yellow);
		gyrMeasError.setSkewFactor(0.2);
		gyrMeasError.setTextBoxStyle(Slider::NoTextBox, false, 10, 10);
		madgwick_Beta.attachToComponent(&gyrMeasError, false);
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

		for (int i = 0; i < 2; i++)
		{
			Joint_Range_AP[i].setVisible(on);
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

		// STS VISUALIZER
		STS_Phase_Disp.setVisible(on);
		for (int j = 0; j < 2; j++)
		{
			JointAngles[j].setVisible(on);
			JointVelocities[j].setVisible(on);
		}
		for (int i = 0; i < 4; i++)
			stsAnim_joint[i].setVisible(on);
		for (int j = 0; j < 20; j++)
		{
			stsAnim_trunk[j].setVisible(on);
			stsAnim_thigh[j].setVisible(on);
			stsAnim_shank[j].setVisible(on);
		}
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
		for (int i = 0; i < 2; i++)
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

		// MISCELLANEOUS
		gyrMeasError.setBounds(10, 600, 200, 50);
		operationMode.setBounds(10, 175, 200, 20);
		orientationAlgo.setBounds(10, 205, 200, 20);
		record_MovementLog.setBounds(10, 235, 200, 20);
	}

	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
	// REAL TIME VISUALIZER

	Label stsAnim_joint[4];
	Label stsAnim_trunk[20];
	Label stsAnim_thigh[20];
	Label stsAnim_shank[20];

	int stsAnim_width_joint = 20;
	int stsAnim_height_joint = 20;

	int stsAnim_widths_segments[3] = { 5, 1, 5 };
	int stsAnim_heights_segments[3] = { 2, 5, 2 };

	int stsAnim_topCorner_X = 50;
	int stsAnim_topCorner_Y = 380;

	int stsAnim_Offset_Joint_X[4] = { 0 };
	int stsAnim_Offset_Joint_Y[4] = { 0 };
	int stsAnim_Offset_Trunk_X[20] = { 0 };
	int stsAnim_Offset_Trunk_Y[20] = { 0 };
	int stsAnim_Offset_Thigh_X[20] = { 0 };
	int stsAnim_Offset_Thigh_Y[20] = { 0 };
	int stsAnim_Offset_Shank_X[20] = { 0 };
	int stsAnim_Offset_Shank_Y[20] = { 0 };

	void updateSTSAnim(MovementParameter mpArray[])
	{
		float angle_Trunk_AP = mpArray[0].value;
		float angle_Thigh_AP = mpArray[1].value * sqrt (abs((mpArray[1].value + 0.001)/ mpArray[1].minVal));
		float angle_Shank_AP = mpArray[2].value * 0.55;
		float angle_Trunk_ML = mpArray[3].value;
		float angle_Thigh_ML = mpArray[4].value;
		float angle_Shank_ML = mpArray[5].value;

		float mlFrac_Trunk = angle_Trunk_ML / mpArray[3].maxVal;
		float mlFrac_Thigh = angle_Thigh_ML / mpArray[4].maxVal;
		float mlFrac_Shank = angle_Shank_ML / mpArray[5].maxVal;

		float jerkFrac = fmin(((mpArray[12].value / mpArray[12].maxVal) - mpArray[12].thresh_min_NORM) * 10, 1.0);

		// Foot X Offset
		int offsetAmplitude_Max_Foot_X = 20 * stsAnim_widths_segments[2];
		int offset_Foot_X = offsetAmplitude_Max_Foot_X * angle_Shank_AP / 90.0 * -1;		// -1 to reverse direction
		// Knee Constant
		//
		// Hip Y Offset
		int offsetAmplitude_Max_Hip_Y = 20 * stsAnim_heights_segments[1];
		int offset_Hip_Y = offsetAmplitude_Max_Hip_Y * (angle_Thigh_AP + 90) / 90.0 * -1; // -1 to reverse direction
		// Head X Offset
		int offsetAmplitude_Max_Head_X = 20 * stsAnim_widths_segments[0];
		int offset_Head_X = offsetAmplitude_Max_Head_X * (angle_Trunk_AP) / 90.0;
		// Head Y Offset
		int offset_Head_Y = offset_Hip_Y;

		// Hip Rotation Radius
		int radius_hipRotation = stsAnim_width_joint + 20 * stsAnim_widths_segments[1];
		float offset_hipRotation_X = abs(radius_hipRotation * cos(angle_Thigh_AP * M_PI / 180.0));

		// Knee Rotation Radius
		int radius_kneeRotation = stsAnim_height_joint + 20 * stsAnim_heights_segments[2];
		float offset_kneeRotation_Y = -abs(radius_kneeRotation * sin(angle_Shank_AP * M_PI / 180.0));

		// SET BOUNDS

		// HEAD
		stsAnim_joint[0].setBounds(
			stsAnim_topCorner_X + offset_Head_X + offset_hipRotation_X,
			stsAnim_topCorner_Y + offset_Head_Y,
			stsAnim_width_joint,
			stsAnim_height_joint
		);

		stsAnim_joint[0].setColour(stsAnim_joint[0].backgroundColourId, 
			Colour::fromFloatRGBA(jerkFrac,0,1-jerkFrac,1));

		STS_Phase_Disp.setBounds(
			stsAnim_topCorner_X + offset_Head_X + offset_hipRotation_X + 20,
			stsAnim_topCorner_Y + offset_Head_Y + 2,
			200,
			15
		);

		// TRUNK
		for (int i = 0; i < 20; i++)
		{
			stsAnim_trunk[i].setBounds(
				stsAnim_topCorner_X + stsAnim_width_joint / 2.0 - stsAnim_widths_segments[0] / 2.0
				+ (20-i) / 20.0 * offset_Head_X + offset_hipRotation_X,
				stsAnim_topCorner_Y + offset_Head_Y + i * stsAnim_heights_segments[0] + stsAnim_height_joint,
				stsAnim_widths_segments[0],
				stsAnim_heights_segments[0]
			);

			stsAnim_trunk[i].setColour(stsAnim_trunk[i].backgroundColourId,Colour::fromFloatRGBA(mlFrac_Trunk, 0.7, mlFrac_Trunk, 1));
		}

		// HIP
		stsAnim_joint[1].setBounds(
			stsAnim_topCorner_X + offset_hipRotation_X,
			stsAnim_topCorner_Y + offset_Hip_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0],
			stsAnim_width_joint,
			stsAnim_height_joint
		);

		stsAnim_joint[1].setColour(stsAnim_joint[1].backgroundColourId,
			Colour::fromFloatRGBA(jerkFrac, 0, 1 - jerkFrac, 1));

		JointVelocities[0].setBounds(
			stsAnim_topCorner_X + offset_hipRotation_X - 45,
			stsAnim_topCorner_Y + offset_Hip_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0] + 17,
			100,
			15
		);

		// THIGH
		for (int i = 0; i < 20; i++)
		{
			stsAnim_thigh[i].setBounds(
				stsAnim_topCorner_X + stsAnim_width_joint + i * stsAnim_widths_segments[1] 
				+ offset_hipRotation_X * stsAnim_width_joint /
				(stsAnim_width_joint + 20.0 * stsAnim_widths_segments[1]) * (20-i)/20.0,
				stsAnim_topCorner_Y + stsAnim_height_joint + (20-i)/20.0 * offset_Hip_Y
				+ 20 * stsAnim_heights_segments[0] + stsAnim_height_joint / 2.0 - 
				stsAnim_heights_segments[1] / 2.0,
				stsAnim_widths_segments[1],
				stsAnim_heights_segments[1]
			);

			stsAnim_thigh[i].setColour(stsAnim_thigh[i].backgroundColourId, Colour::fromFloatRGBA(mlFrac_Thigh, 0.7, mlFrac_Thigh, 1));
		}

		// KNEE
		stsAnim_joint[2].setBounds(
			stsAnim_topCorner_X + stsAnim_width_joint + 20 * stsAnim_widths_segments[1],
			stsAnim_topCorner_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0],
			stsAnim_width_joint,
			stsAnim_height_joint
		);

		JointAngles[1].setBounds(
			stsAnim_topCorner_X + stsAnim_width_joint + 20 * stsAnim_widths_segments[1] + 20,
			stsAnim_topCorner_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0] + 2,
			100,
			15
		);

		JointVelocities[1].setBounds(
			stsAnim_topCorner_X + stsAnim_width_joint + 20 * stsAnim_widths_segments[1] + 20,
			stsAnim_topCorner_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0] + 18,
			100,
			15
		);

		// SHANK
		for (int i = 0; i < 20; i++)
		{
			float vertOffset_ROTATION = offset_kneeRotation_Y * (i) / 20.0;

			stsAnim_shank[i].setBounds(
				stsAnim_topCorner_X + stsAnim_width_joint + 20 * stsAnim_widths_segments[1]
				+ stsAnim_width_joint / 2.0 - stsAnim_widths_segments[0] / 2.0
				+ (i) / 20.0 * offset_Foot_X,
				stsAnim_topCorner_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0]
				+ stsAnim_height_joint + i * stsAnim_heights_segments[2] + vertOffset_ROTATION,
				stsAnim_widths_segments[2],
				stsAnim_heights_segments[2]
			);

			stsAnim_shank[i].setColour(stsAnim_shank[i].backgroundColourId, Colour::fromFloatRGBA(mlFrac_Shank, 0.7, mlFrac_Shank, 1));
		}

		// FOOT
		stsAnim_joint[3].setBounds(
			stsAnim_topCorner_X + stsAnim_width_joint + 20 * stsAnim_widths_segments[1] + offset_Foot_X,
			stsAnim_topCorner_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0] + 
			stsAnim_height_joint + 20 * stsAnim_heights_segments[2] + offset_kneeRotation_Y,
			stsAnim_width_joint,
			stsAnim_height_joint
		);
	}
};