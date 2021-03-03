#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MappingPreset
{
public:
	MappingPreset()
	{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				mappingMatrix[i][j] = false;
				mappingStrength[i][j] = 1.0;
			}
			mappingThresh[i] = 0;
		}
	};

	~MappingPreset()
	{};

	String name = "No Preset";
	bool mappingMatrix[20][20];
	float mappingStrength[20][20];
	short mapFunc_Idx[20] = 
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	short polarity[20] =
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
	short num_quantBits[20] =
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float mappingThresh[20];

	// VOICE - DATA HOLDERS
	float dataHolder_oscBPM = 1;
	bool dataHolder_voiceCue_ON = false;
	float dataHolder_voiceCue_timingFine = 0;
	float dataHolder_voiceCue_voldB = 0;
	int dataHolder_voiceCue_Length = 1;
	bool dataHolder_voiceCue_isIntervalEnabled[8] = { false, false, false };
	bool dataHolder_voiceCue_isPos[8] = { false, false, false };
	float dataHolder_voiceCue_location[8] = { 0.5, 0.5, 0.5 };
};
