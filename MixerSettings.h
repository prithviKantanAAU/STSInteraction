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
		"Perc 1",
		"Melody",
		"Chords",
		"Perc 2",
		"Bass",
		"Gtr",
		"Warnings"
	};

	float masterGain = -6;
	float gain_Track[10] =
	{
		17.2, 1.7, 1.2, 18.3, 11.5, 21.7,10,0,0,0
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
		{2575, 0, 2.8,	200, 0, 0.7,		9000, 3, 0.7,		20000, 0, 0.7},
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
