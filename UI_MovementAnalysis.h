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
	Label STSPhasePresent;	


	void configure()
	{
		
	}

	void toggleVisible(bool on)
	{
		for (int i = 0; i < 3; i++)
		{
			IMUOnline[i].setVisible(on);
			IMUOrientations[i].setVisible(on);
		}

		for (int j = 0; j < 2; j++)		JointAngles[j].setVisible(on);
		STSPhasePresent.setVisible(on);
	}

	void setLayout()
	{
		for (int i = 0; i < 3; i++)
		{
			IMUOnline[i].setBounds(50, 52 + 15 * i, 15, 10);
			IMUOrientations[i].setBounds(80, 50 + 15 * i, 150, 15);
		}

		for (int j = 0; j < 2; j++)		JointAngles[j].setBounds(250, 57.5 + 15 * j, 150, 15);
		STSPhasePresent.setBounds(400, 65, 200, 15);
	}
};