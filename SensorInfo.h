#pragma once
class SensorInfo
{
public:
	SensorInfo() {};
	~SensorInfo() {};

	short numSensorsMax = 3;
	float IMU_SampleRate = 100;
	int UDP_Ports[5] = { 9999, 9998, 9997 };
	int UDP_Ports_REMOTE[5] = {8888, 8887, 8886};
	OSCSender connectionVerify_IP[3];
	std::string sensors_OSCAddress[3] = { "/S1","/S2","/S3" };
	short bodyLocation[5] = { 4, 4, 4 };
	bool isOnline[5] = { false, false, false };
	int batteryPercent[5] = { 0 };
	String remoteIP[5] = { "" };

	void updateStatus(int port, bool status)
	{
		short sensorIndex = -1;
		for (int i = 0; i < numSensorsMax; i++)
		{
			if (port == UDP_Ports[i])
				sensorIndex = i;
		}
		isOnline[sensorIndex] = status;
	};

	void check_areSensorsOnline(short locationsOnline[]) // [TRUNK THIGH SHANK] LOCATIONS
	{
		short out = 0;
		for (int i = 0; i < numSensorsMax; i++)
		{
			if (bodyLocation[i] == 1 && isOnline[i])
				locationsOnline[0] = i;
			if (bodyLocation[i] == 2 && isOnline[i])
				locationsOnline[1] = i;
			if (bodyLocation[i] == 3 && isOnline[i])
				locationsOnline[2] = i;
		}
	}
};

