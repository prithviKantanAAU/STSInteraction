#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GaitParam_Single.h"
#include "AudioParam_Single.h"

// INCLUDE FAUST

class MusicControl
{
    public:
    MusicControl() 
	{
		// Initialize Audio Params !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	};
    ~MusicControl() {};

	// FAUST OBJECT
	// CLASS FOR MUSIC TRANSFORMATION
	
	FeedbackVariable feedbackVariables[20];
	short numFbVariables = 10;

	// MAPPING FUNCTIONS
	short numMapFunc = 7;
	String mapFunc_Names[20] = 
	{
		"MapFunc 1",
		"MapFunc 2",
		"MapFunc 3",
		"MapFunc 4",
		"MapFunc 5",
		"MapFunc 6",
		"MapFunc 7",
	};

	// MAPPING MATRIX
	bool mappingMatrix[20][20] = { false };
	void updateMappingMatrix(short row, short col, bool onOff)
	{
		mappingMatrix[row][col] = onOff;
	}

	void updateFBVariables(MovementParameter[], short STSPhase, bool isSTSPhaseChanged)
	{
		// PHASE CHANGE PERC TRIGGER
		if (isSTSPhaseChanged)
		{
			// HANDLE CASEWISE DEPENDING ON ACTUAL PHASE
		}

		// MAP FB VARIABLES TO FAUST
		mapFBVariables();
	}

	void mapFBVariables()
	{
	}
};