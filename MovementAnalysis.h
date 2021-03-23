#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OSC_Class.h"
#include "SensorInfo.h"
#include "MusicControl.h"
#include "complementaryFilter.h"
#include "quaternionFilters.h"
#include "GaitParam_Single.h"
#include "BiQuad.h"
#include "medianFilter.h"
#include "VoiceCue_Settings.h"

#define M_PI           3.14159265358979323846  /* pi */
#define RAD_TO_DEG		180 / M_PI
#define DEG_TO_RAD		M_PI / 180.0


class MovementAnalysis
{
public:
	MovementAnalysis()
	{
		// SEGMENT ORIENTATION
		movementParams[0].initialize(-5, 40, "Orientation Trunk AP",false);
		movementParams[1].initialize(-90, 10, "Orientation Thigh AP", false);
		movementParams[2].initialize(-40, 40, "Orientation Shank AP",false);
		movementParams[3].initialize(0, 40, "Orientation Trunk ML",false);
		movementParams[4].initialize(0, 60, "Orientation Thigh ML",false);
		movementParams[5].initialize(0, 80, "Orientation Shank ML",false);

		// JOINT ANGLES
		movementParams[6].initialize(-40, 180, "Angle Hip");
		movementParams[7].initialize(-40, 180, "Angle Knee");
		movementParams[8].initialize(-40, 0, "Angle Ankle");

		// JOINT HYPEREXTENSION
		movementParams[9].initialize(0, 1, "Hyperextend Hip");
		movementParams[10].initialize(0, 1, "Hyperextend Knee");

		// JOINT ANGULAR VELOCITY
		movementParams[11].initialize(0, 3, "Ang Velocity Knee");
		movementParams[12].initialize(0, 3, "Ang Velocity Hip");
		movementParams[13].initialize(0, 3, "Ang Velocity Ankle");

		// STS MOVEMENT PHASE CLASSIFICATIONS
		movementParams[14].initialize(-1, 6, "STS Phase");
		
		// CoM NORMALIZED DISPLACEMENTS
		movementParams[15].initialize(0, 3, "Horiz Disp");
		movementParams[16].initialize(0, 3, "Verti Disp");

		// CoM NORMALIZED VELOCITY
		movementParams[17].initialize(0, 3, "Horiz Vel");
		movementParams[18].initialize(0, 3, "Verti Vel");

		// ANGULAR JERK MEASURES
		movementParams[19].initialize(0, 100, "Trunk Jerk - Ang",false);
		movementParams[20].initialize(0, 100, "Thigh Jerk - Ang",false);
		movementParams[21].initialize(0, 100, "Shank Jerk - Ang",false);

		// TRIANGLE OSCILLATOR
		movementParams[22].initialize(0, 1, "Tri Osc");

		setupReceivers();
		eulerSmoothing_INIT();

		musicControl.numMovementParams = numMovementParams;
	};
	~MovementAnalysis() {};
	short numMovementParams = 23;
	SensorInfo sensorInfo;
	short locationsOnline[3] = { -1,-1,-1 };
	OSCReceiverUDP_Sensor sensors_OSCReceivers[3];
	MusicControl musicControl;
	QuaternionFilter quaternionFilters[3];				// 0 = Trunk // 1 = Thigh // 2 = Shank
	ComplementaryFilter compFilters[3];
	MovementParameter movementParams[40];
	VoiceCues voiceCue;
	bool voice_isTrigger = false;

	// EULER ANGLE POST-SMOOTHING FILTERS
	MedianFilter eulerMedianFilt[3][2];
	BiQuad eulerSmoothing[3][3][2];						// 3 Stages // 3 Locations // 2 Directions
	short eulerMedianFilt_L = 3;
	short eulerSmoothing_Order = 6;
	short eulerSmoothing_Fc = 49;
	double eulerSmoothing_Q = 0.7;

	void eulerSmoothing_INIT()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				eulerMedianFilt[i][j].filterLength = eulerMedianFilt_L;
				for (int k = 0; k < 3; k++)
				{
					eulerSmoothing[k][i][j].flushDelays();
					eulerSmoothing[k][i][j].calculateLPFCoeffs(eulerSmoothing_Fc, eulerSmoothing_Q, 100);
				}
			}
		}
	}

	void eulerSmoothing_APPLY()
	{
		for (int i = 0; i < 3; i++)
		{
			if (locationsOnline[i] != -1)
			{
				for (int j = 0; j < 2; j++)
				{
					if (j == 0)
					{
						// APPLY MEDIAN FILTER
						orientation_Deg[i] = eulerMedianFilt[i][j].doFiltering(orientation_Deg[i]);

						// APPLY NECESSARY STAGES OF SMOOTHING
						for (int k = 0; k < eulerSmoothing_Order / 2; k++)
						{
							orientation_Deg[i] = eulerSmoothing[k][i][j].doBiQuad(orientation_Deg[i], 0);
						}
					}

					if (j == 1)
					{
						// APPLY MEDIAN FILTER
						orientation_Deg_ML[i] = eulerMedianFilt[i][j].doFiltering(orientation_Deg_ML[i]);

						// APPLY NECESSARY STAGES OF SMOOTHING
						for (int k = 0; k < eulerSmoothing_Order / 2; k++)
						{
							orientation_Deg_ML[i] = eulerSmoothing[k][i][j].doBiQuad(orientation_Deg_ML[i], 0);
						}
					}
				}
			}
		}
	}

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

	// STS Phase Present, Past and Change
	String STS_Phases[6] = 	{
		"Steady Sitting",
		"Onset",
		"Seat Off",
		"Steady Standing",
		"Sit Onset",
		"Seat On"
	};
	short STS_Phase = 0;
	short STS_Phase_z1 = 0;
	bool STS_Phase_isChanged = false;

	// DETECTION THRESHOLDS
	float thresh_Stand_trunk_AP = 10;
	float range_horiz[2] = { -130, -50 };
	float range_vert[2] = {-30,30};

	// BODY SEGMENT ORIENTATION VALUES // 0 = TRUNK // 1 = THIGH // 2 = SHANK
	float orientation_Deg[3] = { 0.0, -90.0, 0.0 };				// AP
	float orientation_Deg_ML[3] = { 0.0, 0, 0.0 };				// ML
	float orientation_Deg_Yaw[3] = { 0.0, 0, 0.0 };				// Yaw
	
	// IMU Axis and Invert
	short IMU_Mount_Side[3] = {1,1,1};
	short IMU_Polarity[3] = { 1,1,1 };

	// Joint Hyperextend Thresholds
	float jointAngles_thresh_Hyper[3] = { 0.0, 0.0, 0.0 };

	// Joint Bend Angles	// 0 = HIP // 1 = KNEE // 2 = ANKLE
	float jointAngles_Deg[3] = { 0.0 };
	float jointAngles_Deg_Z1[3] = { 0.0 };
	float jointAngularVel_DegPerSec[3] = { 0.0 };

	// Jerk - Delay Registers
	double forJerk_Acc_z1[3][3];
	double forJerk_Gyr_z1[3][3];
	double forJerk_Gyr_z2[3][3];

	// Triangle Oscillator
	double triOsc_Freq = 1;
	long ticksElapsed = 0;
	bool voice_trig_SusArray[4] = { false,false,false,false };
	bool shuffleArray_voiceTrig(bool newVal)
	{
		for (int i = 1; i < 4; i++)
			voice_trig_SusArray[i] = voice_trig_SusArray[i - 1];
		voice_trig_SusArray[0] = newVal;
		for (int i = 0; i < 4; i++)
		{
			if (voice_trig_SusArray[i] == true)		return true;
		}
		return false;
	}

	void triOsc_Update()
	{
		double triOsc_Period = 1 / triOsc_Freq;
		long t = ticksElapsed;
		int D = (int)(triOsc_Period / 0.01);
		double funcVal = abs((t + D - 1) % ((D - 1) * 2) - (D - 1)) / (float)D;
		voice_isTrigger = shuffleArray_voiceTrig(voiceCue.getVoiceTriggerSignal(funcVal));
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
			computeJerkParams(0, "Trunk Jerk - Ang");
			computeJerkParams(1, "Thigh Jerk - Ang");
			computeJerkParams(2, "Shank Jerk - Ang");
			triOsc_Update();
		}

		else stream_mpLogFile();
		//   triOsc_Update();
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

	double fetch_MP_Value(String mpName)
	{
		for (int i = 0; i < numMovementParams; i++)
		{
			if (movementParams[i].name == mpName)
			{
				return movementParams[i].value;
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
		
		eulerSmoothing_APPLY();

		// STORE ML AND AP ORIENTATIONS
		store_MP_Value("Orientation Trunk AP", orientation_Deg[0] * ((IMU_Polarity[0] == 1) ? 1 : -1));
		store_MP_Value("Orientation Thigh AP", orientation_Deg[1] * ((IMU_Polarity[1] == 1) ? 1 : -1));
		store_MP_Value("Orientation Shank AP", orientation_Deg[2] * ((IMU_Polarity[2] == 1) ? 1 : -1));

		store_MP_Value("Orientation Trunk ML", fabs(orientation_Deg_ML[0]));
		store_MP_Value("Orientation Thigh ML", fabs(orientation_Deg_ML[1]));
		store_MP_Value("Orientation Shank ML", fabs(orientation_Deg_ML[2]));
		
		// COMPUTE JOINT ANGLES, CHECK HYPEREXTENSION
		jointAngles_Deg[0] = orientation_Deg[0] - orientation_Deg[1];
		jointAngles_Deg[1] = orientation_Deg[2] - orientation_Deg[1];
		jointAngles_Deg[2] = -orientation_Deg[2];
		
		store_MP_Value("Angle Hip", jointAngles_Deg[0]);
		store_MP_Value("Angle Knee", jointAngles_Deg[1]);
		store_MP_Value("Angle Ankle", jointAngles_Deg[2]);

		store_MP_Value("Hyperextend Hip", (jointAngles_Deg[0] <= jointAngles_thresh_Hyper[0]) ? 1 : 0);
		store_MP_Value("Hyperextend Knee", (jointAngles_Deg[1] <= jointAngles_thresh_Hyper[1]) ? 1 : 0);

		// COMPUTE JOINT ANGULAR VELOCITY
		jointAngularVel_DegPerSec[0] = fabs(jointAngles_Deg[0] - jointAngles_Deg_Z1[0]);
		jointAngularVel_DegPerSec[1] = fabs(jointAngles_Deg[1] - jointAngles_Deg_Z1[1]);
		jointAngularVel_DegPerSec[2] = fabs(jointAngles_Deg[2] - jointAngles_Deg_Z1[2]);
		jointAngles_Deg_Z1[0] = jointAngles_Deg[0];
		jointAngles_Deg_Z1[1] = jointAngles_Deg[1];
		jointAngles_Deg_Z1[2] = jointAngles_Deg[2];

		store_MP_Value("Ang Velocity Hip", jointAngularVel_DegPerSec[0]);
		store_MP_Value("Ang Velocity Knee", jointAngularVel_DegPerSec[1]);
		store_MP_Value("Ang Velocity Ankle", jointAngularVel_DegPerSec[2]);
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
			}
		}
	}

	void getOrientation_Quaternion(float *accBuf, float *gyrBuf,
		float *magBuf, QuaternionFilter *qFilt, float *pitch, float *roll, float *yaw)
	{
		qFilt->MadgwickAHRSupdateIMU(gyrBuf[0] * DEG_TO_RAD, gyrBuf[1] * DEG_TO_RAD, gyrBuf[2] * DEG_TO_RAD,
									accBuf[0], accBuf[1], accBuf[2]);

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

	void computeJerkParams(int locationIdx, String jerkParamName)
	{
		if (locationsOnline[locationIdx] != -1)
		{
			// ANGULAR TRUNK JERK
			float gyrX = sensors_OSCReceivers[locationsOnline[locationIdx]].gyr_Buf[0];
			float gyrY = sensors_OSCReceivers[locationsOnline[locationIdx]].gyr_Buf[1];
			float gyrZ = sensors_OSCReceivers[locationsOnline[locationIdx]].gyr_Buf[2];

			float angAcc_X = gyrX - forJerk_Gyr_z1[locationIdx][0];
			float angAcc_Y = gyrY - forJerk_Gyr_z1[locationIdx][1];
			float angAcc_Z = gyrZ - forJerk_Gyr_z1[locationIdx][2];

			float angAcc_X_z1 = forJerk_Gyr_z1[locationIdx][0] - forJerk_Gyr_z2[locationIdx][0];
			float angAcc_Y_z1 = forJerk_Gyr_z1[locationIdx][1] - forJerk_Gyr_z2[locationIdx][1];
			float angAcc_Z_z1 = forJerk_Gyr_z1[locationIdx][2] - forJerk_Gyr_z2[locationIdx][2];

			float angJerk_X = fabs(angAcc_X - angAcc_X_z1);
			float angJerk_Y = fabs(angAcc_Y - angAcc_Y_z1);
			float angJerk_Z = fabs(angAcc_Z - angAcc_Z_z1);

			float scalarJerk_ANG = sqrt(angJerk_X * angJerk_X + angJerk_Y * angJerk_Y + angJerk_Z * angJerk_Z);
			store_MP_Value(jerkParamName, scalarJerk_ANG);

			for (int i = 0; i < 3; i++)											// SHUFFLE DELAYS
			{
				forJerk_Gyr_z2[locationIdx][i] = forJerk_Gyr_z1[locationIdx][i];
				forJerk_Gyr_z1[locationIdx][i] = sensors_OSCReceivers[locationsOnline[locationIdx]].gyr_Buf[i];
			}
		}
	}

	// Calculate STS Phase from
	void updateSTSPhase()
	{
		// COMPUTE STS PHASE BASED ON ANGLES AND PREVIOUS PHASE
		if(!updateSTSPhase_CheckTransition_POS());
		updateSTSPhase_CheckTransition_NEG();
		store_MP_Value("STS Phase", STS_Phase);

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
		columnIdx_Log = 0;
		mpFile_Streaming_Lines_Total = 0;

		int mpFile_Streaming_Bytes_Total = mpStream.getTotalLength();
		while (lineRem != "")
		{
			columnNames_Log[columnIdx_Log] = lineRem.upToFirstOccurrenceOf(",", false, true);
			lineRem = lineRem.fromFirstOccurrenceOf(",", false, true);
			columnIdx_Log++;
		}
		
		
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

		voice_isTrigger = shuffleArray_voiceTrig(voiceCue.getVoiceTriggerSignal(fetch_MP_Value("Tri Osc")));
		mpFile_Streaming_Progress = mpFile_Streaming_Line_Current / (double)mpFile_Streaming_Lines_Total;
		mpFile_Streaming_Line_Current = (mpFile_Streaming_Line_Current + 1) % mpFile_Streaming_Lines_Total;
	}

	static float getMPVal_fromArray(MovementParameter mpArray[], String mpName, String valType)
	{
		for (int i = 0; i < 20; i++)
		{
			if (mpArray[i].name == mpName)
			{
				if (valType == "Val")
					return mpArray[i].value;

				if (valType == "Max")
					return mpArray[i].maxVal;

				if (valType == "Min")
					return mpArray[i].minVal;
			}
		}
	}
};
