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
	short dispIdx = -1;

	double thresh_min_NORM = 0;
	bool inRange = false;
	float rangeNorm_MIN = 0;
	float rangeNorm_MAX = 1;

	void initialize(float mini, float maxi, String mpName, bool isVis = true)
	{
		name = mpName;
		minVal = mini;
		maxVal = maxi;
		isVisible = isVis;
	}

	void setDispIdx(short dispIndex)
	{
		dispIdx = dispIndex;
	}

	void storeValue(double newVal)
	{
		if (!isnan(newVal))
		value = jlimit(minVal, maxVal, newVal);
	}

	void setRangeNorm(float mini, float maxi)
	{
		rangeNorm_MIN = mini;
		rangeNorm_MAX = maxi;
	}

	MovementParameter() {};
	~MovementParameter() {};
};

