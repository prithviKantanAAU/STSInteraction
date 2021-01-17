#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class FeedbackVariable
{
public:

	String name = "";
	float minVal = 0;
	float maxVal = 1;
	float value = 0;
	short polarity = 1;
	short mapFunc = 1;

	void initialize(float mini, float maxi, short pol, short mapF)
	{
		minVal = mini;
		maxVal = maxi;
		polarity = pol;
		mapFunc = mapF;
	}

	FeedbackVariable()
	{
	};
	
	~FeedbackVariable()
	{
	};
};

