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
	short numMapFunc = 6;
	String mapFunc_Names[20] = 
	{
		"Linear",
		"Exp 1",
		"Exp 2",
		"Log 1",
		"Log 2",
		"Sigmoid"
	};

	// MAPPING MATRIX
	bool mappingMatrix[20][20] = { false };
	void updateMappingMatrix(short row, short col, bool onOff)
	{
		mappingMatrix[row][col] = onOff;
	}

	void updateFBVariables(MovementParameter[])
	{
		// FIND FRACTION SUM OF ALL MPs MAPPED TO A SINGLE AP (LIMIT TO 1)

		// CALCULATE MAPPING FUNCTION FOR EACH AP, APPLY QUANTIZATION IF NEEDED

		// APPLY SPECIAL PROCESSING (E.G. NOTE FREQUENCY COMPUTATION)

		// MAP FB VARIABLES TO FAUST
		mapFBVariables();
	}

	void mapFBVariables()
	{
	}
};