#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class FaustStrings
{
public:
	FaustStrings()
	{

	}

	~FaustStrings(){}
	
	String baseName = "/STS_Synth";

	String masterGain = baseName + "/Mapping_Tabs/Mixer_And_Master/Master_Gain/Master_Gain";

	std::string getFBVar_FAUSTAddress_Full(short fbVar_Idx, short synthCtrl_Idx)
	{
		std::string out = "";

		out = baseName.toStdString() + "/Mapping_Tabs/Sonification_Control/Soni_" 
			+ String(fbVar_Idx + 1).toStdString() + "/Control_" + String(synthCtrl_Idx).toStdString();

		return out;
	}

	std::string getTrackGainAddress(short trackIdx)
	{
		return baseName.toStdString() + "/Mapping_Tabs/Mixer_And_Master/Track_Gain/Track_"
			+ String(trackIdx + 1).toStdString();
	}
};
