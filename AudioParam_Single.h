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

	FeedbackVariable()
	{
	};
	
	~FeedbackVariable()
	{
	};
};

