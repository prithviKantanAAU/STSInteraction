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
	String IMULocations[3] = { "Trunk","Thigh","Shank" };
	String JointNames[2] = {"Hip","Knee"};
	Label IMUOnline[3];							// 0 = TRUNK // 1 = THIGH // 2 = SHANK
	Label IMUOrientations[3];					// 0 = TRUNK // 1 = THIGH // 2 = SHANK
	Label JointAngles[2];						// 0 = HIP	 // 1 = KNEE
	Label JointVelocities[2];					// 0 = HIP	 // 1 = KNEE
	Label STSPhasePresent;
	ComboBox operationMode;
	ComboBox orientationAlgo;
	Slider range_Vert;
	Label range_Vert_LAB;
	Slider range_TrunkAP;
	Label range_TrunkAP_LAB;
	Slider range_Horiz;
	Label range_Horiz_LAB;
	Slider thresh_AP_preStand;
	Label thresh_AP_preStand_LAB;
	TextButton record_MovementLog;

	// SENSOR INVERT
	ComboBox sensor_Axis[3];
	ComboBox sensor_Invert[3];
	String sensor_Axis_Options[2] = { "AP", "ML" };
	String sensor_Invert_Options[2] = { "+", "-" };
	short sensor_Axis_Options_NUM = 2;
	short sensor_Invert_Options_NUM = 2;

	// Simulation Sliders
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
		for (int i = 0; i < 4; i++)
			stsAnim_joint[i].setColour(stsAnim_joint[i].backgroundColourId, Colours::yellow);
		for (int j = 0; j < 20; j++)
		{
			stsAnim_trunk[j].setColour(stsAnim_trunk[j].backgroundColourId, Colours::blue);
			stsAnim_thigh[j].setColour(stsAnim_thigh[j].backgroundColourId, Colours::blue);
			stsAnim_shank[j].setColour(stsAnim_shank[j].backgroundColourId, Colours::blue);
		}

		for (int i = 0; i < 3; i++)
		{
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

		// Horizontal Classification Range
		range_Horiz.setRange(-130, -50);
		range_Horiz.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		range_Horiz.setNumDecimalPlacesToDisplay(0);
		range_Horiz.setColour(range_Horiz.trackColourId, Colours::yellow);
		range_Horiz.setColour(range_Horiz.backgroundColourId, Colours::blue);
		range_Horiz_LAB.attachToComponent(&range_Horiz, true);
		range_Horiz_LAB.setText("Horiz Range", dontSendNotification);
		range_Horiz.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox,false,30, 20);

		// Vertical Classification Range
		range_Vert.setRange(-30, 30);
		range_Vert.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		range_Vert.setNumDecimalPlacesToDisplay(0);
		range_Vert.setColour(range_Vert.trackColourId, Colours::yellow);
		range_Vert.setColour(range_Vert.backgroundColourId, Colours::blue);
		range_Vert_LAB.attachToComponent(&range_Vert, true);
		range_Vert_LAB.setText("Vert Range", dontSendNotification);
		range_Vert.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 30, 20);

		// Trunk AP Limits
		range_TrunkAP.setRange(-90, 90);
		range_TrunkAP.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		range_TrunkAP.setNumDecimalPlacesToDisplay(0);
		range_TrunkAP.setColour(range_TrunkAP.trackColourId, Colours::yellow);
		range_TrunkAP.setColour(range_TrunkAP.backgroundColourId, Colours::blue);
		range_TrunkAP_LAB.attachToComponent(&range_TrunkAP, true);
		range_TrunkAP.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 30, 20);

		// Onset AP Threshold
		thresh_AP_preStand.setRange(0,40);
		thresh_AP_preStand.setNumDecimalPlacesToDisplay(0);
		thresh_AP_preStand.setColour(thresh_AP_preStand.trackColourId, Colours::yellow);
		thresh_AP_preStand.setColour(thresh_AP_preStand.backgroundColourId, Colours::blue);
		thresh_AP_preStand_LAB.attachToComponent(&thresh_AP_preStand, true);
		thresh_AP_preStand_LAB.setText("Onset AP Thresh",dontSendNotification);

		// Record Movement Log
		record_MovementLog.setColour(record_MovementLog.buttonColourId, Colours::red);
		record_MovementLog.setButtonText("Record");

		// Sensor Axis
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < sensor_Axis_Options_NUM; j++)
				sensor_Axis[i].addItem(sensor_Axis_Options[j],j+1);
			sensor_Axis[i].setSelectedId(1);
		}

		// Sensor Invert
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < sensor_Invert_Options_NUM; j++)
				sensor_Invert[i].addItem(sensor_Invert_Options[j], j + 1);
			sensor_Invert[i].setSelectedId(1);
		}
	}

	void toggleVisible(bool on)
	{
		for (int i = 0; i < 3; i++)
		{
			IMUOnline[i].setVisible(on);
			IMUOrientations[i].setVisible(on);
		}

		for (int j = 0; j < 2; j++)
		{
			JointAngles[j].setVisible(on);
			JointVelocities[j].setVisible(on);
		}
		STSPhasePresent.setVisible(on);
		orientationAlgo.setVisible(on);
		operationMode.setVisible(on);

		for (int i = 0; i < 3; i++)
			simulation_OrientAngles[i].setVisible(on);

		range_Horiz.setVisible(on);
		range_Vert.setVisible(on);
		range_TrunkAP.setVisible(on);
		thresh_AP_preStand.setVisible(on);
		record_MovementLog.setVisible(on);

		for (int i = 0; i < 3; i++)
		{
			sensor_Axis[i].setVisible(on);
			sensor_Invert[i].setVisible(on);
		}

		// STS VISUALIZER
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
		for (int i = 0; i < 3; i++)
		{
			sensor_Axis[i].setBounds(10, 52 + 15 * i, 60, 10);
			sensor_Invert[i].setBounds(80, 52 + 15 * i, 60, 10);
			IMUOnline[i].setBounds(150, 52 + 15 * i, 15, 10);
			IMUOrientations[i].setBounds(180, 50 + 15 * i, 150, 15);
		}

		for (int j = 0; j < 2; j++)
		{
			JointAngles[j].setBounds(350, 57.5 + 15 * j, 150, 15);
			JointVelocities[j].setBounds(525, 57.5 + 15 * j, 150, 15);
		}
		STSPhasePresent.setBounds(700, 65, 200, 15);
		record_MovementLog.setBounds(700, 85, 100, 20);
		operationMode.setBounds(10, 105, 150, 20);
		orientationAlgo.setBounds(180, 105, 150, 20);

		range_Horiz.setBounds(100, 135, 200, 20);
		range_Vert.setBounds(100, 160, 200, 20);
		thresh_AP_preStand.setBounds(100, 185, 200, 20);
		range_TrunkAP.setBounds(100, 210, 200, 20);
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

	int stsAnim_topCorner_X = 100;
	int stsAnim_topCorner_Y = 340;

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