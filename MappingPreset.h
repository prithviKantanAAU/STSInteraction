#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MappingPreset
{
public:
	MappingPreset()
	{};

	~MappingPreset()
	{};

	String name = "";
	bool mappingMatrix[20][20];
	short mapFunc_Idx[20];
	short polarity[20];
	short num_quantBits[20];
};
