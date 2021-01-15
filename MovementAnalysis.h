#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OSC_Class.h"
#include "SensorInfo.h"

class MovementAnalysis
{
public:
	MovementAnalysis() {};
	~MovementAnalysis() {};

	SensorInfo sensorInfo;
	OSCReceiverUDP_Sensor sensors_OSCReceivers[3];
};