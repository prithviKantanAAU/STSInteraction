#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OSC_Class.h"
#include "SensorInfo.h"
#include "MusicControl.h"
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
		movementParams[0].initialize(-90, 90, "Orientation Trunk AP",false);
		movementParams[1].initialize(-90, 0, "Orientation Thigh AP", false);
		movementParams[2].initialize(0, 40, "Orientation Shank AP",false);
		movementParams[3].initialize(0, 40, "Orientation Trunk ML");
		movementParams[4].initialize(0, 60, "Orientation Thigh ML",false);
		movementParams[5].initialize(0, 80, "Orientation Shank ML",false);

		// JOINT ANGLES
		movementParams[6].initialize(-10, 180, "Angle Hip");
		movementParams[7].initialize(-10, 180, "Angle Knee");
		movementParams[8].initialize(-10, 40, "Angle Ankle");

		// JOINT HYPEREXTENSION
		movementParams[9].initialize(0, 1, "Extend Hip");
		movementParams[10].initialize(0, 1, "Extend Knee");

		// JOINT ANGULAR VELOCITY
		movementParams[11].initialize(0, 3, "Ang Velocity Knee");
		movementParams[12].initialize(0, 3, "Ang Velocity Hip");
		movementParams[13].initialize(0, 3, "Ang Velocity Ankle",false);

		// STS MOVEMENT PHASE CLASSIFICATIONS
		movementParams[14].initialize(-1, 6, "STS Phase");
		movementParams[23].initialize(0, 1, "Up Progress");
		
		// CoM NORMALIZED DISPLACEMENTS
		movementParams[15].initialize(0.34, 0.49, "Horiz Disp");
		movementParams[16].initialize(0.67, 0.94, "Verti Disp");

		// CoM NORMALIZED VELOCITY
		movementParams[17].initialize(0, 0.3, "Horiz Vel");
		movementParams[18].initialize(0, 0.45, "Verti Vel");

		// ANGULAR JERK MEASURES
		movementParams[19].initialize(0, 100, "Trunk Jerk - Ang",false);
		movementParams[20].initialize(0, 100, "Thigh Jerk - Ang",false);
		movementParams[21].initialize(0, 100, "Shank Jerk - Ang",false);

		// TRIANGLE OSCILLATOR
		movementParams[22].initialize(0, 1, "Tri Osc");

		populateDispIndex_MP();
		setupReceivers();
		eulerSmoothing_INIT();
		LPF_Init();

		musicControl.numMovementParams = numMovementParams;
	};
	~MovementAnalysis() {};

	void setDispIndex_MP(String mpName, short dispIndex)
	{
		for (int i = 0; i < numMovementParams; i++)
		{
			if (movementParams[i].name == mpName)
			{
				movementParams[i].dispIdx = dispIndex;
				break;
			}
		}
	}

	void populateDispIndex_MP()
	{
		// SEGMENT ORIENTATION
		setDispIndex_MP("Orientation Trunk AP", 0);
		setDispIndex_MP("Orientation Thigh AP", 0);
		setDispIndex_MP("Orientation Shank AP", 0);
		setDispIndex_MP("Orientation Trunk ML", 1);
		setDispIndex_MP("Orientation Thigh ML", 1);
		setDispIndex_MP("Orientation Shank ML", 1);

		// JOINT ANGLES
		setDispIndex_MP("Angle Hip", 2);
		setDispIndex_MP("Angle Knee", 2);
		setDispIndex_MP("Angle Ankle", 2);

		// JOINT HYPEREXTENSION
		setDispIndex_MP("Extend Hip", 3);
		setDispIndex_MP("Extend Knee", 3);

		// JOINT ANGULAR VELOCITY
		setDispIndex_MP("Ang Velocity Knee", 4);
		setDispIndex_MP("Ang Velocity Hip", 4);
		setDispIndex_MP("Ang Velocity Ankle", 4);

		// STS MOVEMENT PHASE CLASSIFICATIONS
		setDispIndex_MP("STS Phase", 5);
		setDispIndex_MP("Up Progress", 5);

		// CoM NORMALIZED DISPLACEMENTS
		setDispIndex_MP("Horiz Disp", 6);
		setDispIndex_MP("Verti Disp", 6);

		// CoM NORMALIZED VELOCITY
		setDispIndex_MP("Horiz Vel", 7);
		setDispIndex_MP("Verti Vel", 7);

		// ANGULAR JERK MEASURES
		setDispIndex_MP("Trunk Jerk - Ang", 8);
		setDispIndex_MP("Thigh Jerk - Ang", 8);
		setDispIndex_MP("Shank Jerk - Ang", 8);

		// TRIANGLE OSCILLATOR
		setDispIndex_MP("Tri Osc",9);
	}
	
	short numMovementParams = 24;
	
	SensorInfo sensorInfo;
	short locationsOnline[3] = { -1,-1,-1 };
	
	OSCReceiverUDP_Sensor sensors_OSCReceivers[3];
	MusicControl musicControl;
	QuaternionFilter quaternionFilters[3];				// 0 = Trunk // 1 = Thigh // 2 = Shank
	MovementParameter movementParams[40];
	VoiceCues voiceCue;

	// EULER ANGLE POST-SMOOTHING FILTERS
	MedianFilter eulerMedianFilt[3][2];
	BiQuad eulerSmoothing[3][3][2];						// 3 Stages // 3 Locations // 2 Directions
	short eulerMedianFilt_L = 3;
	short eulerSmoothing_Order = 6;
	short eulerSmoothing_Fc = 49;
	double eulerSmoothing_Q = 0.7;

	// ANGULAR VELOCITY, CoM VELOCITY BIQUADS
	BiQuad LPF_JointAngularVel[3];
	BiQuad LPF_CoM_Vel[2];

	void LPF_Init()
	{
		for (int i = 0; i < 3; i++)
		{
			LPF_JointAngularVel[i].flushDelays();
			LPF_JointAngularVel[i].calculateLPFCoeffs(7, 0.7, 100);
		}

		for (int j = 0; j < 2; j++)
		{
			LPF_CoM_Vel[j].flushDelays();
			LPF_CoM_Vel[j].calculateLPFCoeffs(7, 0.7, 100);
		}
	}

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
	
	// Operation Modes - Sensor v/s Slider
	short numOperationModes = 2;
	short operationMode_Present = 2;
	String OperationModes[5] = { "Slider Simulation","Sensor" };
	
	// Data Input Modes - Sensor v/s File
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
	String imuFile_LogData_RAW[200000][3];
	double imuFile_LogData_LINE[3][9];		
	long mpFile_Streaming_Lines_Total = 0;
	long imuLogFile_Streaming_Lines_Total[3] = { 0,0,0 };
	int mpFile_Streaming_Columns_Total = 0;
	long mpFile_Streaming_Line_Current = 0;
	int mpFile_Streaming_map_Col_to_MP[20];
	double mpFile_Streaming_Progress = 0;
	String columnNames_Log[40];
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
	float thresh_Stand_trunk_AP = 10;
	float range_horiz[2] = { -130, -50 };
	float range_vert[2] = {-30,30};

	// BODY SEGMENT ORIENTATION VALUES // 0 = TRUNK // 1 = THIGH // 2 = SHANK
	float orientation_Deg[3] = { 0.0, -90.0, 0.0 };				// AP
	float orientation_Deg_ML[3] = { 0.0, 0, 0.0 };				// ML
	float orientation_Deg_Yaw[3] = { 0.0, 0, 0.0 };				// Yaw
	
	// IMU Axis and Invert
	short IMU_Mount_Side[3] = {1,2,2};
	short IMU_Polarity[3] = { 1,1,2 };

	// Joint Extention Thresholds
	float jointAngles_thresh_Extend[3] = { 0.0, 0.0, 0.0 };
	float jointAngles_thresh_MaxFlex[3] = { 0.0, 0.0, 0.0 };

	// Joint Bend Angles	// 0 = HIP // 1 = KNEE // 2 = ANKLE
	float jointAngles_Deg[3] = { 0.0 };
	float jointAngles_Deg_Z1[3] = { 0.0 };
	float jointAngularVel_DegPerSec[3] = { 0.0 };

	// Jerk - Delay Registers
	double forJerk_Acc_z1[3][3];
	double forJerk_Gyr_z1[3][3];
	double forJerk_Gyr_z2[3][3];

	// CoP Displacement Calculation
	float segmentWise_WtPct[3] = { 0.624, 0.111, 0.05 };
	float segmentWise_HtPct[3] = { 0.402, 0.241, 0.25 };
	float CoM_Horiz_AP_z1 = 0.0;
	float CoM_Vert_z1 = 0.0;

	// Triangle Oscillator
	double triOsc_Freq = 1;
	long ticksElapsed = 0;
	bool voice_isTrigger = false;
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

	// Buffers for present line of each sensor type - Row = Trunk, Thigh, Shank // Col = Axis
	float acc_Buf[3][3];
	float gyr_Buf[3][3];
	float mag_Buf[3][3];

	void populateIMUBuffers()
	{
		if (dataInput_Mode != 2)				// TAKE INPUT FROM SENSOR OSC OBJECT
		{
			// FIGURE OUT INDEX OF EACH IMU LOCATION - TRUNK THIGH SHANK SENSOR INDEX
			for (int i = 0; i < 3; i++) locationsOnline[i] = -1;
			sensorInfo.check_areSensorsOnline(locationsOnline);

			for (int i = 0; i < 3; i++)
			{
				// SET BUFFERS TO ZERO IF NO SENSOR ONLINE AT THAT LOCATION
				if (locationsOnline[i] == -1)
				{
					for (int j = 0; j < 3; j++)
					{
						acc_Buf[i][j] = 0;
						gyr_Buf[i][j] = 0;
						mag_Buf[i][j] = 0;
					}
				}

				// TAKE INPUT FROM SENSORS IF ONLINE AT THAT LOCATION
				else
				{
					for (int j = 0; j < 3; j++)
					{
						acc_Buf[i][j] = sensors_OSCReceivers[locationsOnline[i]].acc_Buf[j];
						gyr_Buf[i][j] = sensors_OSCReceivers[locationsOnline[i]].gyr_Buf[j];
						mag_Buf[i][j] = sensors_OSCReceivers[locationsOnline[i]].mag_Buf[j];
					}
				}
			}
		}

		else                                    // TAKE INPUT FROM LOG FILE
		{
			stream_imuLogFile();

			for (int i = 0; i < 3; i++)			// FOR EACH BODY SEGMENT
			{
				for (int j = 0; j < 3; j++)		// FOR EACH AXIS
				{
					acc_Buf[i][j] = imuFile_LogData_LINE[i][j];
					gyr_Buf[i][j] = imuFile_LogData_LINE[i][3 + j];
					mag_Buf[i][j] = imuFile_LogData_LINE[i][6 + j];
				}
			}
		}
	}

	void callback()
	{
		ticksElapsed++;

		populateIMUBuffers();
		computeAngles();
		updateSTSPhase();
		computeJerkParams(0, "Trunk Jerk - Ang");
		computeJerkParams(1, "Thigh Jerk - Ang");
		computeJerkParams(2, "Shank Jerk - Ang");
		computeCoM_Disp_Vel();
		computeProgress();
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

		store_MP_Value("Extend Hip", (jointAngles_Deg[0] <= getMPVal_fromArray(movementParams,"Angle Hip", "Min")) ? 1 : 0);
		store_MP_Value("Extend Knee", (jointAngles_Deg[1] <= getMPVal_fromArray(movementParams, "Angle Knee", "Min")) ? 1 : 0);

		// COMPUTE JOINT ANGULAR VELOCITY
		for (int i = 0; i < 3; i++)
		{
			jointAngularVel_DegPerSec[i] = LPF_JointAngularVel[i].doBiQuad(fabs(jointAngles_Deg[i] - jointAngles_Deg_Z1[i]),0);
			jointAngles_Deg_Z1[i] = jointAngles_Deg[i];
		}

		store_MP_Value("Ang Velocity Hip", jointAngularVel_DegPerSec[0]);
		store_MP_Value("Ang Velocity Knee", jointAngularVel_DegPerSec[1]);
		store_MP_Value("Ang Velocity Ankle", jointAngularVel_DegPerSec[2]);
	}

	void computeIMUOrientations()
	{
		// GET QUATERNION OF ALL SENSORS
		for (int i = 0; i < 3; i++)
			getOrientation_Quaternion(acc_Buf[i],gyr_Buf[i],mag_Buf[i],	&quaternionFilters[i],
						(IMU_Mount_Side[i] == 1) ? &orientation_Deg[i] : &orientation_Deg_ML[i],
						(IMU_Mount_Side[i] == 1) ? &orientation_Deg_ML[i] : &orientation_Deg[i],
						&orientation_Deg_Yaw[i]);
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

		*roll *= RAD_TO_DEG;	*yaw *= RAD_TO_DEG;		*yaw -= 8.5;	*pitch *= RAD_TO_DEG;	*pitch -= 90;
		*roll = isnan(*roll) ? 0 : *roll;		*pitch = isnan(*pitch) ? 0 : *pitch;		*yaw = isnan(*yaw) ? 0 : *yaw;
	}

	void computeJerkParams(int locationIdx, String jerkParamName)
	{
		// ANGULAR TRUNK JERK
		float gyrX = gyr_Buf[locationIdx][0];
		float gyrY = gyr_Buf[locationIdx][1];
		float gyrZ = gyr_Buf[locationIdx][2];

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

	void computeCoM_Disp_Vel()
	{
		float seg_AP_Ang_Vals[3] = {
			getMPVal_fromArray(movementParams, "Orientation Trunk AP", "Val"),
			getMPVal_fromArray(movementParams, "Orientation Thigh AP", "Val"),
			getMPVal_fromArray(movementParams, "Orientation Shank AP", "Val")
		};

		float seg_AP_Ang_Mins[3] = { -80, -80, -80 };
		float seg_AP_Ang_Maxs[3] = { 80, 80, 80 };

		// DISPLACEMENT FRACTIONS
		float horiz_DispFracs_AP[3] = { 0.0,0.0,0.0 };
		float vert_DispFracs[3] = { 0.0,0.0,0.0 };

		// HELPER VARIABLES
		float horiz_DispMax_AP_Lin[3] = { 0.0,0.0,0.0 };
		float vert_DispMax_Lin[3] = { 0.0,0.0,0.0 };
		float horiz_DispMin_AP_Lin[3] = { 0.0,0.0,0.0 };
		float vert_DispMin_Lin[3] = { 0.0,0.0,0.0 };

		float horiz_DispRanges_AP[3] = { 0.0,0.0,0.0 };
		float vert_DispRanges[3] = { 0.0,0.0,0.0 };

		// SEGMENT WEIGHTS - MODIFY FOR BEST EFFECT
		float horiz_DispWts_Seg[3] = { 1.2, 1, 1 };
		float verti_DispWts_Seg[3] = { 0.8, 1.3, 1 };

		for (int i = 0; i < 3; i++)						// FOR EACH BODY SEGMENT
		{
			horiz_DispMax_AP_Lin[i] = sin(M_PI / 180.0 * seg_AP_Ang_Maxs[i]);
			horiz_DispMin_AP_Lin[i] = sin(M_PI / 180.0 * seg_AP_Ang_Mins[i]);

			horiz_DispRanges_AP[i] = horiz_DispMax_AP_Lin[i] - horiz_DispMin_AP_Lin[i];

			vert_DispMax_Lin[i] = cos(M_PI / 180.0 * seg_AP_Ang_Maxs[i]);
			vert_DispMin_Lin[i] = cos(M_PI / 180.0 * seg_AP_Ang_Mins[i]);

			vert_DispRanges[i] = (vert_DispMax_Lin[i] > vert_DispMin_Lin[i]) ? 
				(1 - vert_DispMax_Lin[i]) : (1 - vert_DispMin_Lin[i]);

			horiz_DispFracs_AP[i] = (sin(M_PI / 180.0 * seg_AP_Ang_Vals[i]) - sin(M_PI / 180.0 * seg_AP_Ang_Mins[i])) / horiz_DispRanges_AP[i];
			vert_DispFracs[i] = (cos(M_PI / 180.0 * seg_AP_Ang_Vals[i]) - cos(M_PI / 180.0 * seg_AP_Ang_Mins[i])) / vert_DispRanges[i];
		}

		float horiz_Disp_AP = horiz_DispFracs_AP[2] * horiz_DispWts_Seg[2] * (segmentWise_HtPct[2] * segmentWise_WtPct[2] +
			segmentWise_HtPct[1] * segmentWise_WtPct[1] + segmentWise_HtPct[0] * segmentWise_WtPct[0]) +

			horiz_DispFracs_AP[1] * horiz_DispWts_Seg[1] * (segmentWise_HtPct[1] * segmentWise_WtPct[1]
				+ segmentWise_HtPct[0] * segmentWise_WtPct[0]) +

			horiz_DispFracs_AP[0] * horiz_DispWts_Seg[0] * (segmentWise_HtPct[0] * segmentWise_WtPct[0]);

		float vert_Disp = vert_DispFracs[2] * verti_DispWts_Seg[2] * (segmentWise_HtPct[2] * segmentWise_WtPct[2] +
			segmentWise_HtPct[1] * segmentWise_WtPct[1] + segmentWise_HtPct[0] * segmentWise_WtPct[0]) +

			vert_DispFracs[1] * verti_DispWts_Seg[1] * (segmentWise_HtPct[1] * segmentWise_WtPct[1]
				+ segmentWise_HtPct[0] * segmentWise_WtPct[0]) +

			vert_DispFracs[0] * horiz_DispWts_Seg[0] * (segmentWise_HtPct[0] * segmentWise_WtPct[0]);

		store_MP_Value("Horiz Disp", horiz_Disp_AP);
		store_MP_Value("Verti Disp", vert_Disp);

		// CALCULATE NORMALIZED CoM Velocity

		float horiz_Vel_AP = LPF_CoM_Vel[0].doBiQuad(fabs(horiz_Disp_AP - CoM_Horiz_AP_z1) * 100,0);
		float vert_Vel = LPF_CoM_Vel[1].doBiQuad(fabs(vert_Disp - CoM_Vert_z1) * 100,0);

		CoM_Horiz_AP_z1 = horiz_Disp_AP;
		CoM_Vert_z1 = vert_Disp;

		store_MP_Value("Horiz Vel", horiz_Vel_AP);
		store_MP_Value("Verti Vel", vert_Vel);
	}

	void computeProgress()
	{
		float upProgress = 0;

		float CoM_H_MIN = getMPVal_fromArray(movementParams, "Horiz Disp", "Min");
		float CoM_H_MAX = getMPVal_fromArray(movementParams, "Horiz Disp", "Max");
		float CoM_H_VAL = getMPVal_fromArray(movementParams, "Horiz Disp", "Val");
		float CoM_V_MIN = getMPVal_fromArray(movementParams, "Verti Disp", "Min");
		float CoM_V_MAX = getMPVal_fromArray(movementParams, "Verti Disp", "Max");
		float CoM_V_VAL = getMPVal_fromArray(movementParams, "Verti Disp", "Val");

		float norm_Prog_H = (CoM_H_VAL - CoM_H_MIN) / (CoM_H_MAX - CoM_H_MIN);
		float norm_Prog_V = (CoM_V_VAL - CoM_V_MIN) / (CoM_V_MAX - CoM_V_MIN);

		//upProgress = 1.0 / 1.414 * sqrt(pow(norm_Prog_H,2) + pow(norm_Prog_V, 2));
		upProgress = 0.5 * (norm_Prog_H + norm_Prog_V);

		store_MP_Value("Up Progress", upProgress);
	}

	void updateSTSPhase()
	{
		// COMPUTE STS PHASE BASED ON ANGLES AND PREVIOUS PHASE
		if(!updateSTSPhase_CheckTransition_POS());
		updateSTSPhase_CheckTransition_NEG();
		store_MP_Value("STS Phase", STS_Phase);
	}

	bool updateSTSPhase_CheckTransition_POS()
	{
		bool conditions[4] = { false, false, false, false };

		float trunk_AP = getMPVal_fromArray(movementParams, "Orientation Trunk AP", "Val");
		float thigh_AP = getMPVal_fromArray(movementParams, "Orientation Thigh AP", "Val");
		float shank_AP = getMPVal_fromArray(movementParams, "Orientation Shank AP", "Val");

		switch (STS_Phase)
		{
		case 0:				// FROM STEADY SITTING
			conditions[0] = (trunk_AP > thresh_Stand_trunk_AP);
			conditions[1] = (thigh_AP > range_horiz[0])
							&& ((thigh_AP < range_horiz[1]));
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 1:				// FROM STAND ONSET
			conditions[0] = (thigh_AP > range_horiz[1]);
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 2:				// FROM SEAT OFF
			conditions[0] = (trunk_AP < range_vert[1]);
			conditions[1] = (thigh_AP > range_vert[0])
							&& ((thigh_AP < range_vert[1]));
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 3:				// FROM STEADY STANDING
			conditions[0] = (thigh_AP < range_vert[0]);
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 4:				// FROM SIT ONSET
			conditions[0] = (thigh_AP > range_horiz[0])
							&& ((thigh_AP < range_horiz[1]));
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 5:				// FROM SEAT ON
			conditions[0] = (trunk_AP < thresh_Stand_trunk_AP);
			conditions[1] = (thigh_AP > range_horiz[0])
							&& ((thigh_AP < range_horiz[1]));
			conditions[2] = true;
			conditions[3] = true;
			break;
		}

		if (conditions[0] && conditions[1] && conditions[2] && conditions[3])
		{
			STS_Phase = (STS_Phase + 1) % 6;

			if (STS_Phase == 0)
			{
				// INCREMENT REPETITIONS
			}

			return true;
		}
		else return false;
	}

	bool updateSTSPhase_CheckTransition_NEG()
	{
		bool conditions[4] = { false, false, false, false };

		float trunk_AP = getMPVal_fromArray(movementParams, "Orientation Trunk AP", "Val");
		float thigh_AP = getMPVal_fromArray(movementParams, "Orientation Thigh AP", "Val");
		float shank_AP = getMPVal_fromArray(movementParams, "Orientation Shank AP", "Val");

		switch (STS_Phase)
		{
		case 0:				// FROM STEADY SITTING
			conditions[0] = false;
			conditions[1] = false;
			conditions[2] = false;
			conditions[3] = false;
			break;
		case 1:				// FROM STAND ONSET
			conditions[0] = (trunk_AP < thresh_Stand_trunk_AP);
			conditions[1] = (thigh_AP > range_horiz[0])
							&& ((thigh_AP < range_horiz[1]));
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 2:				// FROM SEAT OFF
			conditions[0] = (thigh_AP > range_horiz[0])
							&& ((thigh_AP < range_horiz[1]));
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
			conditions[0] = (thigh_AP > range_vert[0])
							&& ((thigh_AP < range_vert[1]));
			conditions[1] = true;
			conditions[2] = true;
			conditions[3] = true;
			break;
		case 5:				// FROM SEAT ON
			conditions[0] = (thigh_AP > range_horiz[1]);
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

	bool open_mpLogDir_IMU_Raw(String path)
	{
		File imuLog_RAW[3];
		String paths[3] =
		{
			path + "\\Raw IMU Data - Trunk.csv",
			path + "\\Raw IMU Data - Thigh.csv",
			path + "\\Raw IMU Data - Shank.csv",
		};

		bool isFileOpened = false;
		String line = "";

		for (int i = 0; i < 3; i++)					// FOR EACH LOG FILE
		{
			imuLogFile_Streaming_Lines_Total[i] = 0;

			imuLog_RAW[i] = File(paths[i]);
			juce::FileInputStream imuLogStream(imuLog_RAW[i]);
			
			if (imuLogStream.openedOk())
			{
				isFileOpened = true;
				while (!imuLogStream.isExhausted())
				{
					imuFile_LogData_RAW[imuLogFile_Streaming_Lines_Total[i]][i] = imuLogStream.readNextLine();
					imuLogFile_Streaming_Lines_Total[i]++;
				}
			}
		}

		return isFileOpened;
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

	void stream_imuLogFile()
	{
		String line = "";

		for (int i = 0; i < 3; i++)
		{
			line = imuFile_LogData_RAW[mpFile_Streaming_Line_Current][i];
			line = (line == "") ? "0,0,0,0,0,0,0,0,0" : line;

			for (int j = 0; j < 9; j++)
			{
				imuFile_LogData_LINE[i][j] = line.upToFirstOccurrenceOf(",", false, true).getDoubleValue();
				line = line.fromFirstOccurrenceOf(",", false, true);
			}
		}

		// AUTO INCREASE AND DECREASE BETA TO COMPENSATE INITIAL DRIFT
		switch (mpFile_Streaming_Line_Current)
		{
		case 1:
			for (int i = 0; i < 3; i++) quaternionFilters[i].updateBeta(40);
			break;
		case 200:
			for (int i = 0; i < 3; i++) quaternionFilters[i].updateBeta(2.1833);
			break;
		default:
			break;
		}

		mpFile_Streaming_Progress = mpFile_Streaming_Line_Current / (double)imuLogFile_Streaming_Lines_Total[0];
		mpFile_Streaming_Line_Current = (mpFile_Streaming_Line_Current + 1) % imuLogFile_Streaming_Lines_Total[0];
	}

	void stream_mpLogFile()
	{
		double log_lineData[40];
		String line = mpFile_LogData_RAW[mpFile_Streaming_Line_Current];
		for (int i = 0; i < columnIdx_Log - 1; i++)
		{
			log_lineData[i] = line.upToFirstOccurrenceOf(",", false, true).getDoubleValue();
			line = line.fromFirstOccurrenceOf(",", false, true);
		}

		for (int i = 0; i < columnIdx_Log - 1; i++)
			store_MP_Value(columnNames_Log[i], log_lineData[i]);

		voice_isTrigger = shuffleArray_voiceTrig(voiceCue.getVoiceTriggerSignal(
							getMPVal_fromArray(movementParams,"Tri Osc","Val")));
		mpFile_Streaming_Progress = mpFile_Streaming_Line_Current / (double)mpFile_Streaming_Lines_Total;
		mpFile_Streaming_Line_Current = (mpFile_Streaming_Line_Current + 1) % mpFile_Streaming_Lines_Total;
	}

	static float getMPVal_fromArray(MovementParameter mpArray[], String mpName, String valType)
	{
		for (int i = 0; i < 30; i++)
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

	void setBounds_MP(MovementParameter mpArray[], String mpName, float mini, float maxi)
	{
		for (int i = 0; i < numMovementParams; i++)
		{
			if (mpArray[i].name == mpName)
			{
				mpArray[i].minVal = mini;
				mpArray[i].maxVal = maxi;
			}
		}
	}
};