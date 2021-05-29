#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MixerSettings
{
public:
	MixerSettings()
	{

	}

	~MixerSettings() {}

	short num_Tracks = 7;
	String names_Tracks[10] = 
	{
		"Djembe",
		"Voice",
		"Piano",
		"Flute",
		"Bass",
		"Gtr",
		"Misc"
	};

	float masterGain = -19;
	float gain_Track[10] =
	{
		13.2, -2.3, 6, 25, 7.5, 23,19,0,0,0
	};

	float comp_Track[10][4] =
	{
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100}
	};

	float eq_Track[10][12] = 
	{
		{10, 0, 3.0,		200, 3, 0.7,	1000, 0, 0.7,	20000, 0, 0.7},
		{150, 0, 0.7,	341, -3.5 , 0.33,	2709, 3, 0.7,		10000, 0, 0.7},
		{0, 0, 3.0,		100, 0, 0.7,	1000, 0, 0.7,	20000, 0, 0.7},
		{200, 0, 0.7,	200, 0, 0.7,		10000, 5, 0.4,		20000, 0, 0.7},
		{10, 0, 3.0,		100, 0, 0.7,	1000, 0, 0.7,	20000, 0, 0.7},
		{10, 0, 3.0,		100, 0, 0.7,	4341, -8, 3.7,	20000, 0, 0.7},
		{10, 0, 3.0,		100, 0, 0.7,	1000, 0, 0.7,	20000, 0, 0.7}
	};

	float fetchEQValue(int trackID, int filterID, int paramID)
	{
		return eq_Track[trackID][3 * filterID + paramID];
	};

	float fetchCompValue(int trackID, int paramID)
	{
		return comp_Track[trackID][paramID];
	};
	
};
