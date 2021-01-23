#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class FeedbackVariable
{
public:

	String name = "Placeholder";
	float minVal = 0;
	float maxVal = 1;
	float defaultVal = 0;
	float value = 0;
	short polarity = 1;
	short mapFunc = 1;
	short quantLevels_2raisedTo = -1;
	short numSynthControls = 1;

	void initialize(String apName, float mini, float maxi, float defVal, short pol, short mapF, short numQuantLevels,
					short nSynthCtrls)
	{
		name = apName;
		minVal = mini;
		maxVal = maxi;
		defaultVal = defVal;
		polarity = pol;
		mapFunc = mapF;
		quantLevels_2raisedTo = numQuantLevels;
		numSynthControls = nSynthCtrls;
	}

	FeedbackVariable()
	{
	};
	
	~FeedbackVariable()
	{
	};
};

