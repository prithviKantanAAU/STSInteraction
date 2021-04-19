#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MappingPreset
{
public:
	MappingPreset()
	{
		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				mappingMatrix[i][j] = false;
				mappingStrength[i][j] = 1.0;
			}
			mappingThresh_MP_Min[i] = 0;
			mappingThresh_AP_Min[i] = 0;
			mappingThresh_MP_Max[i] = 1;
			mappingThresh_AP_Max[i] = 1;
		}
	};

	~MappingPreset()
	{};

	String name = "No Preset";
	bool mappingMatrix[40][40];
	float mappingStrength[40][40];
	short mapFunc_Idx[40] = 
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	short polarity[40] =
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
	short num_quantBits[40] =
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float mappingThresh_MP_Min[40];
	float mappingThresh_MP_Max[40];
	float mappingThresh_AP_Min[40];
	float mappingThresh_AP_Max[40];

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
