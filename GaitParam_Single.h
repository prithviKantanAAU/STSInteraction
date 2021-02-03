#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MovementParameter
{
public:

	String name = "Placeholder";
	double value = 0;		
	double minVal = 0;
	double maxVal = 1;
	bool isVisible = false;

	void initialize(float mini, float maxi, String mpName, bool isVis)
	{
		name = mpName;
		minVal = mini;
		maxVal = maxi;
		isVisible = isVis;
	}

	void storeValue(double newVal)
	{
		value = jlimit(minVal, maxVal, newVal);
	}

	MovementParameter() {};
	~MovementParameter() {};
};

