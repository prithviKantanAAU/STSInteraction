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
	float freq_Smoothing = 49;
	// 1 = Instrument Trigger
	// 2 = Freq
	// 3 = Dynamics
	// 4 = Timbre Modifier (Not +ve or -ve)
	// 5 = -ve Feedback Param
	// 6 = Sine Wave
	short parameterType = 1;
	bool isVisible = false;

	float rangeNorm_MIN = 0;
	float rangeNorm_MAX = 1;

	void initialize(String apName, float mini, float maxi, float defVal, short pol, short mapF, short numQuantLevels,
					short nSynthCtrls, short paramType, bool isVis = true, float freq_Smo = 49)
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
		freq_Smoothing = freq_Smo;
	}

	void setDispIdx(short dispIndex)
	{
		dispIdx = dispIndex;
	}

	void setRangeNorm(float mini, float maxi)
	{
		rangeNorm_MIN = mini;
		rangeNorm_MAX = maxi;
	}

	FeedbackVariable()
	{
	};
	
	~FeedbackVariable()
	{
	};
};

