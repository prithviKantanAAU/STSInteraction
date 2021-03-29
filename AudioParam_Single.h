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
	short dispIdx = -1;
	// 1 = Instrument Trigger
	// 2 = Freq
	// 3 = Sound Effect
	// 4 = Timbre Modifier
	short parameterType = 1;
	bool isVisible = false;

	void initialize(String apName, float mini, float maxi, float defVal, short pol, short mapF, short numQuantLevels,
					short nSynthCtrls, short paramType, bool isVis)
	{
		name = apName;
		minVal = mini;
		maxVal = maxi;
		defaultVal = defVal;
		polarity = pol;
		mapFunc = mapF;
		quantLevels_2raisedTo = numQuantLevels;
		numSynthControls = nSynthCtrls;
		parameterType = paramType;
		isVisible = isVis;
	}

	void setDispIdx(short dispIndex)
	{
		dispIdx = dispIndex;
	}

	FeedbackVariable()
	{
	};
	
	~FeedbackVariable()
	{
	};
};

