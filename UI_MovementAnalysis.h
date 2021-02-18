#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class UI_MovementAnalysis
{
public:
	UI_MovementAnalysis()
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
	int IMU_Config_StartY = 50;
	int IMU_Config_Column_StartPos[10] =
	{ 50, 150, 250, 280, 380, 430, 610, 670, 720, 890};
	int IMU_Config_Column_Width[10] =
	{ 90, 90, 20, 90, 45, 175, 45, 45, 175, 45};
	int IMU_Config_Row_Height = 20;
	int IMU_Config_Row_Offset = 35;
	int IMU_Config_Indicator_Side = 5;

	Label IMU_Online_Indicator[3];				// 1
	ComboBox IMU_Mount_Side[3];					// 2
	ComboBox IMU_Polarity[3];					// 3
	Label IMU_Location[3];						// 4
	
	Label AP;
	Slider IMU_range_segmentAngles_AP[3];		// 6
	Label IMU_segmentAngles_AP_Indicator[3];
	Label IMU_segmentRanges_AP_Bounds[3][2];	// 5, 7
	
	Label ML;
	Slider IMU_range_segmentAngles_ML[3];		// 9
	Label IMU_segmentAngles_ML_Indicator[3];
	Label IMU_segmentRanges_ML_Bounds[3][2];	// 8, 10

	void update_Indicators_SensorOrientation(MovementParameter mpArray[])
	{
		for (int i = 0; i < 3; i++)
		{
			// ADJUST INDICATOR POSITION

			for (int j = 0; j < 2; j++)
			{
				// ADJUST BOUNDS TEXT
			}
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
			stsAnim_joint[i].setColour(stsAnim_joint[i].backgroundColourId, Colours::yellow);
		for (int j = 0; j < 20; j++)
		{
			stsAnim_trunk[j].setColour(stsAnim_trunk[j].backgroundColourId, Colours::blue);
			stsAnim_thigh[j].setColour(stsAnim_thigh[j].backgroundColourId, Colours::blue);
			stsAnim_shank[j].setColour(stsAnim_shank[j].backgroundColourId, Colours::blue);
		}

		// HEADERS
		ML.setText("ML", dontSendNotification);
		AP.setText("AP", dontSendNotification);

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

			// 7 - ML Lower Bound - No Config Needed

			// 8 - ML Range Setter
			IMU_range_segmentAngles_ML[i].setRange(-90, 90);
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

		// Record Movement Log
		record_MovementLog.setColour(record_MovementLog.buttonColourId, Colours::red);
		record_MovementLog.setButtonText("Record");

		JointAngles[0].attachToComponent(&stsAnim_joint[1], true);	
	}

	void toggleVisible(bool on)
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

		// MISCELLANEOUS
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

		// MISCELLANEOUS
		operationMode.setBounds(10, 160, 200, 20);
		orientationAlgo.setBounds(10, 190, 200, 20);
		record_MovementLog.setBounds(10, 220, 200, 20);
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

	void updateSTSAnim(float angle_Trunk, float angle_Thigh, float angle_Shank)
	{
		// Foot X Offset
		int offsetAmplitude_Max_Foot_X = 20 * stsAnim_widths_segments[2];
		int offset_Foot_X = offsetAmplitude_Max_Foot_X * angle_Shank / 90.0 * -1;		// -1 to reverse direction
		// Knee Constant
		//
		// Hip Y Offset
		int offsetAmplitude_Max_Hip_Y = 20 * stsAnim_heights_segments[1];
		int offset_Hip_Y = offsetAmplitude_Max_Hip_Y * (angle_Thigh + 90) / 90.0 * -1; // -1 to reverse direction
		// Head X Offset
		int offsetAmplitude_Max_Head_X = 20 * stsAnim_widths_segments[0];
		int offset_Head_X = offsetAmplitude_Max_Head_X * (angle_Trunk) / 90.0;
		// Head Y Offset
		int offset_Head_Y = offset_Hip_Y;

		// Hip Rotation Radius
		int radius_hipRotation = stsAnim_width_joint + 20 * stsAnim_widths_segments[1];
		float offset_hipRotation_X = abs(radius_hipRotation * cos(angle_Thigh * M_PI / 180.0));

		// SET BOUNDS

		// HEAD
		stsAnim_joint[0].setBounds(
			stsAnim_topCorner_X + offset_Head_X + offset_hipRotation_X,
			stsAnim_topCorner_Y + offset_Head_Y,
			stsAnim_width_joint,
			stsAnim_height_joint
		);

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
		}

		// HIP
		stsAnim_joint[1].setBounds(
			stsAnim_topCorner_X + offset_hipRotation_X,
			stsAnim_topCorner_Y + offset_Hip_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0],
			stsAnim_width_joint,
			stsAnim_height_joint
		);

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
			stsAnim_shank[i].setBounds(
				stsAnim_topCorner_X + stsAnim_width_joint + 20 * stsAnim_widths_segments[1]
				+ stsAnim_width_joint / 2.0 - stsAnim_widths_segments[0] / 2.0
				+ (i) / 20.0 * offset_Foot_X,
				stsAnim_topCorner_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0]
				+ stsAnim_height_joint + i * stsAnim_heights_segments[2],
				stsAnim_widths_segments[2],
				stsAnim_heights_segments[2]
			);
		}

		// FOOT
		stsAnim_joint[3].setBounds(
			stsAnim_topCorner_X + stsAnim_width_joint + 20 * stsAnim_widths_segments[1] + offset_Foot_X,
			stsAnim_topCorner_Y + stsAnim_height_joint + 20 * stsAnim_heights_segments[0] + 
			stsAnim_height_joint + 20 * stsAnim_heights_segments[2],
			stsAnim_width_joint,
			stsAnim_height_joint
		);
	}
};