#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GaitParam_Single.h"

// INCLUDE FAUST

class MusicControl
{
    public:
    MusicControl() {};
    ~MusicControl() {};

	// FAUST OBJECT
	// SOMETHING FOR MUSIC TRANSFORMATION

	// Feedback Variable Array
	float fbVariables[20] = { 0.0 };
	// 0 = Instrument 1 Triggering / Timbre ....
	String fbVariables_Names[20] = 
	{
		"Aud Param 1",
		"Aud Param 2",
		"Aud Param 3",
		"Aud Param 4",
		"Aud Param 5",
		"Aud Param 6",
		"Aud Param 7",
		"Aud Param 8",
		"Aud Param 9",
		"Aud Param 10",
	};
	short numFbVariables = 10;
	float fbVariable_MAX = 10;
	float fbVariable_MIN = 0;

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