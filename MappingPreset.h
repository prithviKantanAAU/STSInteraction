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
};
