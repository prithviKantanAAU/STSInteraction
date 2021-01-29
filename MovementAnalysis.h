#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OSC_Class.h"
#include "SensorInfo.h"
#include "MusicControl.h"
#include "complementaryFilter.h"
#include "quaternionFilters.h"
#include "GaitParam_Single.h"
#include "BiQuad.h"

#define M_PI           3.14159265358979323846  /* pi */
#define RAD_TO_DEG		180 / M_PI
#define DEG_TO_RAD		M_PI / 180.0


class MovementAnalysis
{
public:
	MovementAnalysis() 
	{
		movementParams[0].initialize(-30, 90, "Orientation Trunk AP");
		movementParams[1].initialize(-90, 90, "Orientation Thigh AP");
		movementParams[2].initialize(-90, 90, "Orientation Shank AP");
		movementParams[3].initialize(-45, 45, "Orientation Trunk ML");
		movementParams[4].initialize(0, 180, "Angle Hip");
		movementParams[5].initialize(0, 180, "Angle Knee");
		movementParams[6].initialize(0, 5, "Ang Velocity Knee");
		movementParams[7].initialize(0, 5, "Ang Velocity Hip");
		movementParams[8].initialize(0, 5, "STS Phase");

		angularVel_Smooth[0].calculateLPFCoeffs(5, 0.7, 100);
		angularVel_Smooth[1].calculateLPFCoeffs(5, 0.7, 100);

		setupReceivers();

		musicControl.numMovementParams = numMovementParams;
	};
	~MovementAnalysis() {};

	SensorInfo sensorInfo;
	short locationsOnline[3] = { -1,-1,-1 };
	OSCReceiverUDP_Sensor sensors_OSCReceivers[3];
	MusicControl musicControl;
	QuaternionFilter quaternionFilters[3];				// 0 = Trunk // 1 = Thigh // 2 = Shank
	ComplementaryFilter compFilters[3];
	BiQuad angularVel_Smooth[2];						// 1 = Hip	 // 2 = Knee
	MovementParameter movementParams[10];

	short numOrientationAlgos = 2;
	short orientAlgo_Present = 1;
	String OrientationAlgos[5] = { "Madgwick","ACCGYR CompFilt" };
	short numOperationModes = 2;
	short operationMode_Present = 1;
	String OperationModes[5] = { "Slider Simulation","Sensor" };

	// 0 = Trunk AP Orientation
	// 1 = Thigh AP Orientation
	// 2 = Shank AP Orientation
	// 3 = Trunk ML Orientation
	// 4 = Hip Angle
	// 5 = Knee Angle
	// 6 = Hip Angular Velocity
	// 7 = Knee Angular Velocity

	short numMovementParams = 10;

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

	// DETECTION THRESHOLDS
	float thresh_Stand_trunk_AP = 10;
	float range_horiz[2] = { -95, -85 };
	float range_vert[2] = {-15,15};

	// Resting Values
	float restAngle_Deg_Hip = 90;
	float restAngle_Deg_Knee = 90;
	float restAngle_Deg_Trunk = 0;

	// Phase Detection Thresholds
	float thresh_Deg_Onset_Hip = 5;
	float thresh_Deg_SeatOff_Knee = 5;
	float thresh_Deg_Standing_Both = 5;

	// IMU AP Orientation Values
	float orientation_Deg[3] = { 0.0, -90.0, 0.0 };				// AP
	float orientation_Deg_ML[3] = { 0.0, 0, 0.0 };				// ML
	float orientation_Deg_Yaw[3] = { 0.0, 0, 0.0 };				// Yaw
	
	// IMU Axis and Invert
	short sensor_Axis[3] = {1,1,1};
	short sensor_Invert[3] = { 1,1,1 };

	// Joint Bend Angles
	float jointAngles_Deg[2] = { 0.0 };
	float jointAngles_Deg_Z1[2] = { 0.0 };

	// Joint Bend Angular Velocities
	float jointAngularVel_DegPerSec[2] = { 0.0 };

	// SETUP OSC UDP RECEIVERS - PORT, LISTENER, SAMPLE RATE
	void setupReceivers()
	{
		for (int i = 0; i < sensorInfo.numSensorsMax; i++)
		{
			sensors_OSCReceivers[i].setupPortAndListener(sensorInfo.UDP_Ports[i], sensorInfo.sensors_OSCAddress[i]);
			sensors_OSCReceivers[i].setSampleRate(sensorInfo.IMU_SampleRate);
		}
	}

	void updateSensorStatus()
	{
		for (int i = 0; i < sensorInfo.numSensorsMax; i++)
		{
			sensorInfo.isOnline[i] = sensors_OSCReceivers[i].isSensorActive();
			if (sensorInfo.isOnline[i])
				sensorInfo.batteryPercent[i] = sensors_OSCReceivers[i].level_Battery;
		}
	};

	// Timed Callback
	void callback()
	{
		computeAngles();
		updateSTSPhase();
		musicControl.updateFBVariables(movementParams, numMovementParams);
	}

	// Calculate IMU Orientations and Joint Angles
	void computeAngles()
	{
		// COMPUTE ORIENTATION - Trunk, Thigh, Shank
		switch (operationMode_Present)
		{
		case 1:
			break;
		case 2: 
			computeIMUOrientations();
			break;
		}
		// COMPUTE JOINT ANGLES	
		movementParams[0].storeValue(orientation_Deg[0] * ((sensor_Invert[0] == 1) ? 1 : -1));
		movementParams[1].storeValue(orientation_Deg[1] * ((sensor_Invert[1] == 1) ? 1 : -1));
		movementParams[2].storeValue(orientation_Deg[2] * ((sensor_Invert[2] == 1) ? 1 : -1));
		jointAngles_Deg[0] = 180 - (orientation_Deg[0] + fabs(orientation_Deg[1]));
		jointAngles_Deg[1] = 180 - (fabs(orientation_Deg[1]) + orientation_Deg[2]);
		movementParams[3].storeValue(orientation_Deg_ML[0]);
		movementParams[4].storeValue(jointAngles_Deg[0]);
		movementParams[5].storeValue(jointAngles_Deg[1]);

		// COMPUTE JOINT ANGULAR VELOCITY
		jointAngularVel_DegPerSec[0] = fabs(angularVel_Smooth[0].doBiQuad(jointAngles_Deg[0] - jointAngles_Deg_Z1[0], 0.0));
		jointAngularVel_DegPerSec[1] = fabs(angularVel_Smooth[1].doBiQuad(jointAngles_Deg[1] - jointAngles_Deg_Z1[1], 0.0));
		jointAngles_Deg_Z1[0] = jointAngles_Deg[0];
		jointAngles_Deg_Z1[1] = jointAngles_Deg[1];
		movementParams[6].storeValue(jointAngularVel_DegPerSec[0]);
		movementParams[7].storeValue(jointAngularVel_DegPerSec[1]);
	}

	void computeIMUOrientations()
	{
		// FIGURE OUT INDEX OF EACH IMU LOCATION
		for (int i = 0; i < 3; i++) locationsOnline[i] = -1;
		sensorInfo.check_areSensorsOnline(locationsOnline);
		
		// FIND ORIENTATION OF ALL SENSORS
		for (int i = 0; i < 3; i++)
		{
			if (locationsOnline[i] == -1) orientation_Deg[i] = 0;			// Set to zero if offline

			else 
			{
				if (orientAlgo_Present == 1)									// Madgwick
					getOrientation_Quaternion(
						sensors_OSCReceivers[locationsOnline[i]].acc_Buf,
						sensors_OSCReceivers[locationsOnline[i]].gyr_Buf,
						sensors_OSCReceivers[locationsOnline[i]].mag_Buf,
						&quaternionFilters[i],
						(sensor_Axis[i] == 1) ? &orientation_Deg[i] : &orientation_Deg_ML[i],
						(sensor_Axis[i] == 1) ? &orientation_Deg_ML[i] : &orientation_Deg[i],
						&orientation_Deg_Yaw[i]);

				if (orientAlgo_Present == 2)									// Regular Complementary Filter
				{
					compFilters[locationsOnline[i]].getOrientation_Fused(
						sensors_OSCReceivers[locationsOnline[i]].acc_Buf,
						sensors_OSCReceivers[locationsOnline[i]].gyr_Buf,
						(sensor_Axis[i] == 1) ? &orientation_Deg_ML[i] : &orientation_Deg[i],
						(sensor_Axis[i] == 1) ? &orientation_Deg[i] : &orientation_Deg_ML[i]
					);
				}
			}
		}
	}

	void getOrientation_Quaternion(float *accBuf, float *gyrBuf,
		float *magBuf, QuaternionFilter *qFilt, float *pitch, float *roll, float *yaw)
	{
		qFilt->MadgwickQuaternionUpdate(accBuf[0], accBuf[1], accBuf[2], gyrBuf[0] * DEG_TO_RAD,
			gyrBuf[1] * DEG_TO_RAD, gyrBuf[2] * DEG_TO_RAD,
			magBuf[0], magBuf[1], magBuf[2], 1.0 / sensorInfo.IMU_SampleRate);

		*yaw = atan2(2.0f * (*(qFilt->getQ() + 1) * *(qFilt->getQ() + 2)
			+ *qFilt->getQ() * *(qFilt->getQ() + 3)), *qFilt->getQ() *
			*qFilt->getQ() + *(qFilt->getQ() + 1) * *(qFilt->getQ() + 1)
			- *(qFilt->getQ() + 2) * *(qFilt->getQ() + 2) - *(qFilt->getQ() + 3)
			* *(qFilt->getQ() + 3));

		*roll = -asin(2.0f * (*(qFilt->getQ() + 1) * *(qFilt->getQ() + 3)
			- *qFilt->getQ() * *(qFilt->getQ() + 2)));

		*pitch = atan2(2.0f * (*qFilt->getQ() * *(qFilt->getQ() + 1) +
			*(qFilt->getQ() + 2) * *(qFilt->getQ() + 3)), *qFilt->getQ() *
			*qFilt->getQ() - *(qFilt->getQ() + 1) * *(qFilt->getQ() + 1)
			- *(qFilt->getQ() + 2) * *(qFilt->getQ() + 2) + *(qFilt->getQ() + 3)
			* *(qFilt->getQ() + 3));

		*roll *= RAD_TO_DEG;
		*yaw *= RAD_TO_DEG;
		*yaw -= 8.5;
		*pitch *= RAD_TO_DEG;
		*pitch -= 90;
	}

	// Calculate STS Phase from
	void updateSTSPhase()
	{
		// COMPUTE STS PHASE BASED ON ANGLES AND PREVIOUS PHASE
		if(!updateSTSPhase_CheckTransition_POS());
		updateSTSPhase_CheckTransition_NEG();
		movementParams[8].storeValue(STS_Phase);

		// SHUFFLE PHASE
		STS_Phase_isChanged = (STS_Phase != STS_Phase_z1) ? true : false;
		STS_Phase_z1 = STS_Phase;
	}

	bool updateSTSPhase_CheckTransition_POS()
	{
		bool conditions[4] = { false, false, false, false };

		switch (STS_Phase)
		{
		case 0:				// FROM STEADY SITTING
			conditions[0] = (movementParams[0].value > thresh_Stand_trunk_AP);
			conditions[1] = (movementParams[1].value > range_horiz[0])
							&& ((movementParams[1].value < range_horiz[1]));
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 1:				// FROM STAND ONSET
			conditions[0] = (movementParams[1].value > range_horiz[1]);
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 2:				// FROM SEAT OFF
			conditions[0] = (movementParams[0].value < range_vert[1]);
			conditions[1] = (movementParams[1].value > range_vert[0])
							&& ((movementParams[1].value < range_vert[1]));
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 3:				// FROM STEADY STANDING
			conditions[0] = (movementParams[1].value < range_vert[0]);
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 4:				// FROM SIT ONSET
			conditions[0] = (movementParams[1].value > range_horiz[0])
							&& ((movementParams[1].value < range_horiz[1]));
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 5:				// FROM SEAT ON
			conditions[0] = (movementParams[0].value < thresh_Stand_trunk_AP);
			conditions[1] = (movementParams[1].value > range_horiz[0])
							&& ((movementParams[1].value < range_horiz[1]));
			conditions[2] = true;
			conditions[3] = true;
			break;
		}

		if (conditions[0] && conditions[1] && conditions[2] && conditions[3])
		{
			STS_Phase = (STS_Phase + 1) % 6;
			return true;
		}
		else return false;
	}

	bool updateSTSPhase_CheckTransition_NEG()
	{
		bool conditions[4] = { false, false, false, false };

		switch (STS_Phase)
		{
		case 0:				// FROM STEADY SITTING
			conditions[0] = false;
			conditions[1] = false;
			conditions[2] = false;
			conditions[3] = false;
			break;
		case 1:				// FROM STAND ONSET
			conditions[0] = (movementParams[0].value < thresh_Stand_trunk_AP);
			conditions[1] = (movementParams[1].value > range_horiz[0])
							&& ((movementParams[1].value < range_horiz[1]));
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 2:				// FROM SEAT OFF
			conditions[0] = (movementParams[1].value > range_horiz[0])
							&& ((movementParams[1].value < range_horiz[1]));
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 3:				// FROM STEADY STANDING
			conditions[0] = false;
			conditions[1] = false;
			conditions[2] = false;
			conditions[3] = false;
			break;
		case 4:				// FROM SIT ONSET
			conditions[0] = (movementParams[1].value > range_vert[0])
							&& ((movementParams[1].value < range_vert[1]));
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 5:				// FROM SEAT ON
			conditions[0] = (movementParams[1].value > range_horiz[1]);
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		}

		if (conditions[0] && conditions[1] && conditions[2] && conditions[3])
		{
			STS_Phase = (max(0, (STS_Phase - 1))) % 6;
			return true;
		}
		else return false;
	}

	void setSimulationAngle(int sliderIdx, float val)
	{
		if (operationMode_Present == 1)
		{
			orientation_Deg[sliderIdx] = val;
			//movementParams[sliderIdx].storeValue(val);
		}
	}

	void setMovementLimits(float minVal, float maxVal)
	{
		movementParams[0].minVal = minVal;
		movementParams[0].maxVal = maxVal;

		movementParams[4].minVal = 90 - maxVal;
		movementParams[4].maxVal = 180 - minVal;
	}
};
