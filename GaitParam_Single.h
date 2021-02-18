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

	double thresh_min_NORM = 0;

	void initialize(float mini, float maxi, String mpName, bool isVis = true)
	{
		name = mpName;
		minVal = mini;
		maxVal = maxi;
		isVisible = isVis;
	}

	void storeValue(double newVal)
	{
		if (!isnan(newVal))
		value = jlimit(minVal, maxVal, newVal);
	}

	MovementParameter() {};
	~MovementParameter() {};
};

