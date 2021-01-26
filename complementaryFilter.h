#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#define M_PI           3.14159265358979323846  /* pi */
#define RAD_TO_DEG		180 / M_PI
#define DEG_TO_RAD		M_PI / 180.0

class ComplementaryFilter
{
public:
	ComplementaryFilter()
	{}

	~ComplementaryFilter()
	{}

	// Complementary Filter Variables
	float accX_z1 = 0.0;	float accY_z1 = 0.0;	float accZ_z1 = 0.0;	// DELAYED ACC SAMPLES - XYZ
	float R_acc_est_z1[3] = { 0.0 };										// DELAYED ACC ESTIMATE - XYZ
	float R_acc_est[3] = { 0.0 };											// ACC ESTIMATE - XYZ
	float R_gyr_z1[3] = { 0.0 };											// DELAYED GYR SAMPLES - XYZ
	bool isCalibrated_Orientation = false;									// CHECK IF ORIENTATION CALIBRATED
	float wt_Gyr = 5;
	float fs = 100;

	// FUNCTION TO CALIBRATE TRUNK REST WHEN MEASURING ORIENTATION (AUTOMATIC AND PERIODIC)
	void trunk_CalibrateRest(float *accBuf)
	{
		float magnitude = sqrt(pow(accBuf[0], 2) + pow(accBuf[1], 2) + pow(accBuf[2], 2));

		R_acc_est[0] = accBuf[0] / magnitude;
		R_acc_est[1] = accBuf[2] / magnitude;
		R_acc_est[2] = accBuf[1] / magnitude;
	}

	// COMPLEMENTARY FILTER ALGO
	void getOrientation_Fused(float *accBuf, float *gyrBuf, float *mlAngleVar, float *apAngleVar)
	{
		//Adjust Coordinates and Units
		float R_acc_X = accBuf[0];	float gyrReading_X = gyrBuf[0] * M_PI / 180;
		float R_acc_Y = accBuf[2];	float gyrReading_Y = gyrBuf[2] * M_PI / 180;
		float R_acc_Z = accBuf[1];	float gyrReading_Z = gyrBuf[1] * M_PI / 180;
		float R_gyro_Inter[3] = { 0.0 };
		float R_acc_Vector[3] = { R_acc_X,R_acc_Y,R_acc_Z };

		//Normalize
		float R_acc = sqrt(R_acc_X*R_acc_X + R_acc_Y * R_acc_Y + R_acc_Z * R_acc_Z);
		R_acc_X /= R_acc;	R_acc_Y /= R_acc;	R_acc_Z /= R_acc;

		//Get Angles
		R_acc_est[2] = fmax(0.0001, R_acc_est[2]);
		float angle_xz_z1 = atan2(R_acc_est[0], R_acc_est[2]);
		float angle_yz_z1 = atan2(R_acc_est[1], R_acc_est[2]);

		//Average Gyr Reading and store buffer
		float gyrReading_Y_avg = 0.5*(gyrReading_Y + R_gyr_z1[1]);
		float gyrReading_X_avg = 0.5*(gyrReading_X + R_gyr_z1[0]);
		R_gyr_z1[0] = gyrReading_X;
		R_gyr_z1[1] = gyrReading_Y;
		R_gyr_z1[2] = gyrReading_Z;

		float angle_xz = angle_xz_z1 + gyrReading_Y_avg * 1.0 / fs;
		float angle_yz = angle_yz_z1 + gyrReading_X_avg * 1.0 / fs;
		*mlAngleVar = (isnan(angle_xz) ? 0 : -angle_xz * 180 / M_PI);
		*apAngleVar = (isnan(angle_yz) ? 0 : -angle_yz * 180 / M_PI); // AP

		//Calculate R_Gyro_Inter
		R_gyro_Inter[0] = sin(angle_xz) / sqrt(1 + pow(cos(angle_xz), 2)*pow(tan(angle_yz), 2));
		R_gyro_Inter[1] = sin(angle_yz) / sqrt(1 + pow(cos(angle_yz), 2)*pow(tan(angle_xz), 2));
		R_gyro_Inter[2] = sqrt(1 - R_gyro_Inter[0] * R_gyro_Inter[0] - R_gyro_Inter[1] * R_gyro_Inter[1]);

		for (int i = 0; i < 3; i++)
		{
			R_acc_est_z1[i] = R_acc_est[i];
			R_acc_est[i] = (R_acc_Vector[i] + wt_Gyr * R_gyro_Inter[i]) / (1 + wt_Gyr);
		}
	}
};

