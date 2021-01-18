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
			IMUOnline[i].setBounds(50, 52 + 15 * i, 15, 10);
			IMUOrientations[i].setBounds(80, 50 + 15 * i, 150, 15);
		}

		for (int j = 0; j < 2; j++)
		{
			JointAngles[j].setBounds(250, 57.5 + 15 * j, 150, 15);
			JointVelocities[j].setBounds(425, 57.5 + 15 * j, 150, 15);
		}
		STSPhasePresent.setBounds(600, 65, 200, 15);
		operationMode.setBounds(50, 105, 150, 20);
		orientationAlgo.setBounds(220, 105, 150, 20);

		for (int i = 0; i < 3; i++)
			simulation_OrientAngles[i].setBounds(50, 135 + 25 * i, 200, 20);
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