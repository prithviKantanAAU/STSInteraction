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
		// initialize(String apName, float mini, float maxi, short pol, short mapF)
		feedbackVariables[0].initialize("Perc Tr", 0, 5, 1, 1, 4);
		feedbackVariables[1].initialize("Mel Tr/Fr", 0, 15, 1, 1, 4);
		feedbackVariables[2].initialize("Chord Tr/Fr", 0, 7, 1, 2, 3);
		feedbackVariables[3].initialize("Detune", 0, 1, 1, 6, 0);
		feedbackVariables[4].initialize("Pan", 0, 1, 1, 5, 0);
		feedbackVariables[5].initialize("Perc2 Tr", 0, 10, 1, 4, 4);
		feedbackVariables[6].initialize("Bass Tr", 0, 7, 1, 3, 3);
		feedbackVariables[7].initialize("Dynamics", 0, 10, 1, 2, 0);
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

	void updateFBVariables(MovementParameter mpArray[], int numMP)
	{
		double fbVar_Value_Temp = 0;
		for (int i = 0; i < numFbVariables; i++)
		{
			fbVar_Value_Temp = 0;
			if (checkIfFbVarMapped(i, numMP))		// ONLY COMPUTE IF SOMETHING IS MAPPED TO FBVAR
			{
				// FIND FRACTION SUM OF ALL MPs MAPPED TO A SINGLE AP (LIMIT TO 1)
				for (int j = 0; j < numMP; j++)
				{
					if (mappingMatrix[j][i])
					{
						fbVar_Value_Temp += (mpArray[j].value - mpArray[j].minVal) 
											/ (mpArray[j].maxVal - mpArray[j].minVal);
					}
				}
				
				// CONSTRAIN VALUE BETWEEN 0 and 1
				fbVar_Value_Temp = jlimit(0.0, 1.0, fbVar_Value_Temp);

				// CALCULATE MAPPING FUNCTION FOR EACH AP, APPLY QUANTIZATION IF NEEDED
				applyMapFunc(&fbVar_Value_Temp, i);

				// APPLY SPECIAL PROCESSING (E.G. NOTE FREQUENCY COMPUTATION)
				applySpecialMappingProcessing(&fbVar_Value_Temp, i);

				feedbackVariables[i].value = fbVar_Value_Temp;
			}
			else feedbackVariables[i].value = 0;	// SET FB VAR TO 0 IF UNMAPPED
			mapFBVariable(i);
		}
	}

	bool checkIfFbVarMapped(short fbVar_Idx, short numMp)
	{
		bool isMapped = false;
		
		for (int i = 0; i < numMp; i++)
		{
			if (mappingMatrix[i][fbVar_Idx])
			{
				isMapped = true;
				break;
			}
		}
		return isMapped;
	}

	void applyMapFunc(double *val, short fbVar_Idx)
	{
		short mapFuncIdx = feedbackVariables[fbVar_Idx].mapFunc;

		if (feedbackVariables[fbVar_Idx].polarity == 2) *val = 1 - *val;

		switch (mapFuncIdx)
		{
		case 1:
			*val *= (feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);
			break;
		case 2:
			*val = pow(*val, 1.5);
			*val *= (feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);
			break;
		case 3:
			*val = pow(*val, 2);
			*val *= (feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);
			break;
		case 4:
			*val = pow(*val, 0.75);
			*val *= (feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);
			break;
		case 5:
			*val = pow(*val, 0.5);
			*val *= (feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);
			break;
		case 6:
			*val = exp((*val - 0.5) * 12) / (1 + exp((*val - 0.5) * 12));
			*val *= (feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);
			break;
		}
		
		*val = quantizeParam(*val, feedbackVariables[fbVar_Idx].quantLevels_2raisedTo,
			(feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal));
	}

	double quantizeParam(float currentParamValue, int quantLevels_2raisedTo, float range)
	{
		float quantizedParam = 0;
		if (quantLevels_2raisedTo < 1)
			return currentParamValue;

		else
		{
			int numQuantizationSteps = pow(2, quantLevels_2raisedTo);
			float quantizationStepSize = range / (float)numQuantizationSteps;

			float diff = 100;
			for (int i = 0; i < numQuantizationSteps + 1; i++)
			{
				float currentStepForTest = 0;
				currentStepForTest += i * quantizationStepSize;
				if (diff > fabs(currentParamValue - currentStepForTest))
				{
					diff = currentParamValue - currentStepForTest;
				}
			}
			quantizedParam = currentParamValue - diff;
			return quantizedParam;
		}
	};

	void applySpecialMappingProcessing(double *val, short fbVar_Idx)
	{

	}

	void mapFBVariable(int fbVar_Idx)
	{
	}
};