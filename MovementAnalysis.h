#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OSC_Class.h"
#include "SensorInfo.h"
#include "MusicControl.h"
#include "quaternionFilters.h"

class MovementAnalysis
{
public:
	MovementAnalysis() {};
	~MovementAnalysis() {};

	SensorInfo sensorInfo;
	OSCReceiverUDP_Sensor sensors_OSCReceivers[3];
	MusicControl musicControl;
	QuaternionFilter quaternionFilters[3];				// 0 = Trunk // 1 = Thigh // 2 = Shank

	// STS Phase Variable
	// 0 = Steady Sitting
	// 1 = Onset
	// 2 = Seat Off
	// 3 = Steady Standing
	String STS_Phases[4] =
	{
		"Steady Sitting",
		"Onset",
		"Seat Off",
		"Steady Standing"
	};

	short STS_Phase = 0;

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

	// Feedback Variable Array
	float fbVariables[20] = { 0.0 };
	// 0 = ...

	// Calculate IMU Orientations and Joint Angles
	void computeAngles()
	{

	}

	// Convert Joint Angles to respective FB Variables
	void jointAngleToFBVariable()
	{

	}

	// Calculate STS Phase from
	void updateSTSPhase()
	{

	}

	float getFBVar_RangedTrigger(float rangeStart, float rangeEnd, int numSteps, float presentValue)
	{
		float fbVar = 0;
		
		return fbVar;
	}
};
