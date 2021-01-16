#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OSC_Class.h"
#include "SensorInfo.h"
#include "MusicControl.h"
#include "quaternionFilters.h"
#include "GaitParam_Single.h"

class MovementAnalysis
{
public:
	MovementAnalysis() 
	{
		movementParams[0].initialize(-90, 90, "Orientation Trunk AP");
		movementParams[1].initialize(-90, 90, "Orientation Thigh AP");
		movementParams[2].initialize(-90, 90, "Orientation Shank AP");
		movementParams[3].initialize(-90, 90, "Orientation Trunk ML");
		movementParams[4].initialize(0, 180, "Angle Hip");
		movementParams[5].initialize(0, 180, "Angle Knee");
	};
	~MovementAnalysis() {};

	SensorInfo sensorInfo;
	OSCReceiverUDP_Sensor sensors_OSCReceivers[3];
	MusicControl musicControl;
	QuaternionFilter quaternionFilters[3];				// 0 = Trunk // 1 = Thigh // 2 = Shank
	MovementParameter movementParams[10];
	// 0 = Trunk AP Orientation
	// 1 = Thigh AP Orientation
	// 2 = Shank AP Orientation
	// 3 = Trunk ML Orientation
	// 4 = Hip Angle
	// 5 = Knee Angle
	short numMovementParams = 6;

	// STS Phase Variable
	// 0 = Steady Sitting
	// 1 = Stand Onset
	// 2 = Seat Off
	// 3 = Steady Standing
	// 4 = Sit Onset
	// 5 = Seat On

	String STS_Phases[6] =
	{
		"Steady Sitting",
		"Onset",
		"Seat Off",
		"Steady Standing",
		"Sit Onset",
		"Seat On"
	};

	// STS Phase Present, Past and Change
	short STS_Phase = 0;
	short STS_Phase_z1 = 0;
	bool STS_Phase_isChanged = false;

	// Resting Values
	float restAngle_Deg_Hip = 90;
	float restAngle_Deg_Knee = 90;
	float restAngle_Deg_Trunk = 0;

	// Phase Detection Thresholds
	float thresh_Deg_Onset_Hip = 5;
	float thresh_Deg_SeatOff_Knee = 5;
	float thresh_Deg_Standing_Both = 5;

	// IMU AP Orientation Values
	float orientation_Deg[3] = { 0.0 };

	// Joint Bend Angles
	float jointAngles_Deg[2] = { 0.0 };

	// Timed Callback
	void callback()
	{
		computeAngles();
		updateSTSPhase();
		musicControl.updateFBVariables(movementParams, STS_Phase, STS_Phase_isChanged);
	}

	// Calculate IMU Orientations and Joint Angles
	void computeAngles()
	{

	}

	// Calculate STS Phase from
	void updateSTSPhase()
	{
		// COMPUTE STS PHASE BASED ON ANGLES AND PREVIOUS PHASE

		// SHUFFLE PHASE
		STS_Phase_isChanged = (STS_Phase != STS_Phase_z1) ? true : false;
		STS_Phase_z1 = STS_Phase;
	}

	float getFBVar_RangedTrigger(float rangeStart, float rangeEnd, int numSteps, float presentValue)
	{
		float fbVar = 0;
		
		return fbVar;
	}
};
