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
		movementParams[0].initialize(-5, 40, "Orientation Trunk AP");
		movementParams[1].initialize(-90, 10, "Orientation Thigh AP");
		movementParams[2].initialize(-90, 90, "Orientation Shank AP",false);
		movementParams[3].initialize(0, 40, "Orientation Trunk ML");
		movementParams[4].initialize(0, 60, "Orientation Thigh ML",false);
		movementParams[5].initialize(0, 80, "Orientation Shank ML",false);

		movementParams[6].initialize(0, 180, "Angle Hip");
		movementParams[7].initialize(0, 180, "Angle Knee");
		movementParams[8].initialize(0, 5, "Ang Velocity Knee",false);
		movementParams[9].initialize(0, 5, "Ang Velocity Hip");
		movementParams[10].initialize(0, 5, "STS Phase");
		movementParams[11].initialize(0, 1, "Tri Osc");
		movementParams[12].initialize(0, 200, "Trunk Jerk - Ang");

		angularVel_Smooth[0].calculateLPFCoeffs(5, 0.7, 100);
		angularVel_Smooth[1].calculateLPFCoeffs(5, 0.7, 100);

		setupReceivers();

		musicControl.numMovementParams = numMovementParams;
	};
	~MovementAnalysis() {};
	short numMovementParams = 13;
	SensorInfo sensorInfo;
	short locationsOnline[3] = { -1,-1,-1 };
	OSCReceiverUDP_Sensor sensors_OSCReceivers[3];
	MusicControl musicControl;
	QuaternionFilter quaternionFilters[3];				// 0 = Trunk // 1 = Thigh // 2 = Shank
	ComplementaryFilter compFilters[3];
	BiQuad angularVel_Smooth[2];						// 1 = Hip	 // 2 = Knee
	MovementParameter movementParams[20];

	short numOrientationAlgos = 2;
	short orientAlgo_Present = 1;
	String OrientationAlgos[5] = { "Madgwick","ACCGYR CompFilt" };
	short numOperationModes = 2;
	short operationMode_Present = 1;
	String OperationModes[5] = { "Slider Simulation","Sensor" };
	
	short dataInput_Mode = 0;
	String dataInput_Mode_Names[5] =
	{
		"Sensor Stream || No Log Loaded",
		"Sensor Stream || Log Loaded",
		"Sensor Ignored || Log Stream"
	};

	// MP Streaming Helper Variables
	File mpFile_Streaming;
	String mpFile_LogData_RAW[200000];

	long mpFile_Streaming_Lines_Total = 0;
	int mpFile_Streaming_Columns_Total = 0;
	long mpFile_Streaming_Line_Current = 0;
	int mpFile_Streaming_map_Col_to_MP[20];
	double mpFile_Streaming_Progress = 0;
	String columnNames_Log[20];
	int columnIdx_Log = 0;

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
	float range_horiz[2] = { -130, -50 };
	float range_vert[2] = {-30,30};

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
	short IMU_Mount_Side[3] = {1,1,1};
	short IMU_Polarity[3] = { 1,1,1 };

	// Joint Bend Angles
	float jointAngles_Deg[2] = { 0.0 };
	float jointAngles_Deg_Z1[2] = { 0.0 };

	// Joint Bend Angular Velocities
	float jointAngularVel_DegPerSec[2] = { 0.0 };

	// Jerk - Delay Registers
	double forJerk_Acc_z1[3];
	double forJerk_Gyr_z1[3];
	double forJerk_Gyr_z2[3];

	// Triangle Oscillator
	double triOsc_Freq = 1;
	long ticksElapsed = 0;
	void triOsc_Update()
	{
		double triOsc_Period = 1 / triOsc_Freq;
		long t = ticksElapsed;
		int D = (int)(triOsc_Period / 0.01);
		double funcVal = abs((t + D - 1) % ((D - 1) * 2) - (D - 1)) / (float)D;
		store_MP_Value("Tri Osc",funcVal);
	}

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
		ticksElapsed++;
		if (dataInput_Mode != 2)
		{
			computeAngles();
			updateSTSPhase();
			if (locationsOnline[0] != -1) computeJerkParams();
		}

		else stream_mpLogFile();
		triOsc_Update();
		musicControl.updateFBVariables(movementParams, numMovementParams);
	}

	// Store Value By Name
	void store_MP_Value(String mpName, double value)
	{
		for (int i = 0; i < numMovementParams; i++)
		{
			if (movementParams[i].name == mpName)
			{
				movementParams[i].storeValue(value);
				break;
			}
		}
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
		
		// STORE ML AND AP ORIENTATIONS
		store_MP_Value("Orientation Trunk AP", orientation_Deg[0] * ((IMU_Polarity[0] == 1) ? 1 : -1));
		store_MP_Value("Orientation Thigh AP", orientation_Deg[1] * ((IMU_Polarity[1] == 1) ? 1 : -1));
		store_MP_Value("Orientation Shank AP", orientation_Deg[2] * ((IMU_Polarity[2] == 1) ? 1 : -1));

		store_MP_Value("Orientation Trunk ML", fabs(orientation_Deg_ML[0]));
		store_MP_Value("Orientation Thigh ML", fabs(orientation_Deg_ML[1]));
		store_MP_Value("Orientation Shank ML", fabs(orientation_Deg_ML[2]));
		
		// COMPUTE JOINT ANGLES	
		jointAngles_Deg[0] = 180 - (orientation_Deg[0] + fabs(orientation_Deg[1]));
		jointAngles_Deg[1] = 180 - (fabs(orientation_Deg[1]) + orientation_Deg[2]);
		
		store_MP_Value("Angle Hip", jointAngles_Deg[0]);
		store_MP_Value("Angle Knee", jointAngles_Deg[1]);

		// COMPUTE JOINT ANGULAR VELOCITY
		jointAngularVel_DegPerSec[0] = fabs(angularVel_Smooth[0].doBiQuad(jointAngles_Deg[0] - jointAngles_Deg_Z1[0], 0.0));
		jointAngularVel_DegPerSec[1] = fabs(angularVel_Smooth[1].doBiQuad(jointAngles_Deg[1] - jointAngles_Deg_Z1[1], 0.0));
		jointAngles_Deg_Z1[0] = jointAngles_Deg[0];
		jointAngles_Deg_Z1[1] = jointAngles_Deg[1];

		store_MP_Value("Ang Velocity Hip", jointAngularVel_DegPerSec[0]);
		store_MP_Value("Ang Velocity Knee", jointAngularVel_DegPerSec[1]);
	}

	void computeIMUOrientations()
	{
		// FIGURE OUT INDEX OF EACH IMU LOCATION - TRUNK THIGH SHANK SENSOR INDEX
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
						(IMU_Mount_Side[i] == 1) ? &orientation_Deg[i] : &orientation_Deg_ML[i],
						(IMU_Mount_Side[i] == 1) ? &orientation_Deg_ML[i] : &orientation_Deg[i],
						&orientation_Deg_Yaw[i]);

				if (orientAlgo_Present == 2)									// Regular Complementary Filter
				{
					compFilters[locationsOnline[i]].getOrientation_Fused(
						sensors_OSCReceivers[locationsOnline[i]].acc_Buf,
						sensors_OSCReceivers[locationsOnline[i]].gyr_Buf,
						(IMU_Mount_Side[i] == 1) ? &orientation_Deg_ML[i] : &orientation_Deg[i],
						(IMU_Mount_Side[i] == 1) ? &orientation_Deg[i] : &orientation_Deg_ML[i]
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

		*roll = isnan(*roll) ? 0 : *roll;
		*pitch = isnan(*pitch) ? 0 : *pitch;
		*yaw = isnan(*yaw) ? 0 : *yaw;
	}

	void computeJerkParams()
	{
		// ANGULAR TRUNK JERK
		float gyrX = sensors_OSCReceivers[locationsOnline[0]].gyr_Buf[0];
		float gyrY = sensors_OSCReceivers[locationsOnline[0]].gyr_Buf[1];
		float gyrZ = sensors_OSCReceivers[locationsOnline[0]].gyr_Buf[2];

		float angAcc_X = gyrX - forJerk_Gyr_z1[0];
		float angAcc_Y = gyrY - forJerk_Gyr_z1[1];
		float angAcc_Z = gyrZ - forJerk_Gyr_z1[2];

		float angAcc_X_z1 = forJerk_Gyr_z1[0] - forJerk_Gyr_z2[0];
		float angAcc_Y_z1 = forJerk_Gyr_z1[1] - forJerk_Gyr_z2[1];
		float angAcc_Z_z1 = forJerk_Gyr_z1[2] - forJerk_Gyr_z2[2];

		float angJerk_X = fabs(angAcc_X - angAcc_X_z1);
		float angJerk_Y = fabs(angAcc_Y - angAcc_Y_z1);
		float angJerk_Z = fabs(angAcc_Z - angAcc_Z_z1);

		float scalarJerk_ANG = sqrt(angJerk_X * angJerk_X + angJerk_Y * angJerk_Y + angJerk_Z * angJerk_Z);
		store_MP_Value("Trunk Jerk - Ang", scalarJerk_ANG);
		
		for (int i = 0; i < 3; i++)											// SHUFFLE DELAYS
		{
			forJerk_Gyr_z2[i] = forJerk_Gyr_z1[i];
			forJerk_Gyr_z1[i] = sensors_OSCReceivers[locationsOnline[0]].gyr_Buf[i];
		}
	}

	// Calculate STS Phase from
	void updateSTSPhase()
	{
		// COMPUTE STS PHASE BASED ON ANGLES AND PREVIOUS PHASE
		if(!updateSTSPhase_CheckTransition_POS());
		updateSTSPhase_CheckTransition_NEG();
		store_MP_Value("STS Phase",STS_Phase);

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

	bool open_mpLogFile_forStream(String path)
	{
		mpFile_Streaming = File(path);

		juce::FileInputStream mpStream(mpFile_Streaming);

		if (!mpStream.openedOk())
			return false;									// failed to open

		// Map Log Columns To MP Array Members
		String line = mpStream.readNextLine();
		String lineRem = line;

		int mpFile_Streaming_Bytes_Total = mpStream.getTotalLength();
		while (lineRem != "")
		{
			columnNames_Log[columnIdx_Log] = lineRem.upToFirstOccurrenceOf(",", false, true);
			lineRem = lineRem.fromFirstOccurrenceOf(",", false, true);
			columnIdx_Log++;
		}
		
		columnIdx_Log--;
		for (int i = 0; i < columnIdx_Log; i++)
		{
			for (int j = 0; j < numMovementParams; j++)
			{
				if (movementParams[j].name == columnNames_Log[i])
					mpFile_Streaming_map_Col_to_MP[i] = j;
			}
		}

		while (!mpStream.isExhausted())
		{
			mpFile_LogData_RAW[mpFile_Streaming_Lines_Total] = mpStream.readNextLine();
			mpFile_Streaming_Lines_Total++;
		}

		return true;
	}

	void stream_mpLogFile()
	{
		double log_lineData[20];
		String line = mpFile_LogData_RAW[mpFile_Streaming_Line_Current];
		for (int i = 0; i < columnIdx_Log - 1; i++)
		{
			log_lineData[i] = line.upToFirstOccurrenceOf(",", false, true).getDoubleValue();
			line = line.fromFirstOccurrenceOf(",", false, true);
		}

		for (int i = 0; i < columnIdx_Log - 1; i++)
			store_MP_Value(columnNames_Log[i], log_lineData[i]);

		mpFile_Streaming_Progress = mpFile_Streaming_Line_Current / (double)mpFile_Streaming_Lines_Total;
		mpFile_Streaming_Line_Current = (mpFile_Streaming_Line_Current + 1) % mpFile_Streaming_Lines_Total;
	}
};
