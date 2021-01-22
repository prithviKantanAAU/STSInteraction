#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MixerSettings
{
public:
	MixerSettings()
	{

	}

	~MixerSettings() {}

	short num_Tracks = 4;
	String names_Tracks[10] = 
	{
		"Perc 1",
		"Melody",
		"Chords",
		"Perc 2"
	};

	float masterGain = -6;
	float gain_Track[10] =
	{
		0,0,0,0,0,0,0,0,0,0
	};

	
};
