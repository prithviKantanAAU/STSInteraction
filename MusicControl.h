#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GaitParam_Single.h"
#include "AudioParam_Single.h"
#include "FaustStrings.h"
#include "MusicInfoCompute.h"
#include "MixerSettings.h"
#include "MappingPreset.h"

class MusicControl
{
public:
	MusicControl()
	{
		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 40; j++)
				mappingStrength[i][j] = 1.0;
		}
		// Initialize Audio Params !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// initialize(String apName, float mini, float maxi, float defaultVal,
		//            short pol, short mapF, short numQuantLevels short numSynthControls,
		//            short paramType, bool isVis)
		feedbackVariables[0].initialize("Perc Tr", 150, 600, 150, 0, 1, 4, 1, 1,true);
		feedbackVariables[1].initialize("Mel Fr", 100, 700, 100, 1, 1, 3, 1, 2, true);
		feedbackVariables[2].initialize("Mel Tr", 0, 200, 0, 0, 1, 3, 1, 1, true);
		feedbackVariables[3].initialize("Chord Fr", 50, 1000, 50, 1, 2, 3, 4, 2, true);
		feedbackVariables[4].initialize("Chord Tr", 0, 10, 0, 1, 2, 3, 4, 1, true);
		feedbackVariables[5].initialize("Detune", 0, 1, 0, 1, 6, 0, 1, 3, true);
		feedbackVariables[6].initialize("Pan", 0, 1, 0.5, 1, 5, 0, 1, 3, false);
		feedbackVariables[7].initialize("Perc2 Tr", 700, 1200, 700, 1, 4, 4, 1, 1, true);
		feedbackVariables[8].initialize("Dynamics", 7, 10, 7, 1, 2, 0, 1, 4, false);
		feedbackVariables[9].initialize("Pitch Warp", 0.5, 1, 0.5, 1, 2, 0, 1, 3, true);
		feedbackVariables[10].initialize("Vowel", 0, 2, 0, 1, 2, 0, 1, 4, true);
		feedbackVariables[11].initialize("Gtr Tr", 0, 200, 1, 0, 1, 4, 1, 1, true);
		feedbackVariables[12].initialize("Gtr Stf", 0.01, 0.4, 0.1, 0, 1, 0, 1, 4, false);
		feedbackVariables[13].initialize("Voice Fric", 0, 1, 0, 0, 1, 0, 1, 4, false);
		feedbackVariables[14].initialize("Djmb Shrp", 0.4, 10, 1, 0, 1, 0, 1, 4, true);
		feedbackVariables[15].initialize("Mrmb Shrp", 1, 10, 1, 0, 1, 0, 1, 4, false);
		feedbackVariables[16].initialize("Bell Tr", 0, 1, 0, 0, 1, 0, 1, 1, true);
		feedbackVariables[17].initialize("Gtr Dyn", 50, 1500, 1500, 1, 3, 0, 1, 4, true);
		feedbackVariables[18].initialize("Gtr Fr", 50, 1500, 1500, 1, 3, 0, 1, 4, true);
		feedbackVariables[19].initialize("Sin1 F", 10, 400, 10, 1, 3, 0, 1, 4, true);
		feedbackVariables[20].initialize("Sin2 F", 10, 1600, 10, 1, 3, 0, 1, 4, true);
		feedbackVariables[21].initialize("Sin3 F", 10, 1600, 10, 1, 3, 0, 1, 4, true);
		feedbackVariables[22].initialize("Prox", 0, 1, 1, 1, 2, 0, 1, 3, true);

		populateDispIndex_AP();
	};
	~MusicControl() 
	{
	};

	void setDispIndex_AP(String apName, short dispIndex)
	{
		for (int i = 0; i < numFbVariables; i++)
		{
			if (feedbackVariables[i].name == apName)
			{
				feedbackVariables[i].dispIdx = dispIndex; 
				break;
			}
		}
	}

	void populateDispIndex_AP()
	{
		setDispIndex_AP("Perc Tr", 0);
		setDispIndex_AP("Mel Tr", 0);
		setDispIndex_AP("Chord Tr", 0);
		setDispIndex_AP("Perc2 Tr", 0);
		setDispIndex_AP("Gtr Tr", 0);
		setDispIndex_AP("Bell Tr", 0);
		
		setDispIndex_AP("Mel Fr", 1);
		setDispIndex_AP("Chord Fr", 1);
		setDispIndex_AP("Gtr Fr", 1);

		setDispIndex_AP("Dynamics", 2);
		setDispIndex_AP("Djmb Shrp", 2);
		setDispIndex_AP("Mrmb Shrp", 2);
		setDispIndex_AP("Gtr Dyn", 2);

		setDispIndex_AP("Vowel", 3);
		setDispIndex_AP("Gtr Stf", 3);
		setDispIndex_AP("Voice Fric", 3);
		
		setDispIndex_AP("Detune", 4);
		setDispIndex_AP("Pan", 4);
		setDispIndex_AP("Pitch Warp", 4);
		setDispIndex_AP("Prox", 4);

		setDispIndex_AP("Sin1 F", 5);
		setDispIndex_AP("Sin2 F", 5);
		setDispIndex_AP("Sin3 F", 5);
	}

	// FAUST OBJECT
	bool isMusicDSP_On = false;

	FeedbackVariable feedbackVariables[40];
	short numFbVariables = 23;
	short numMovementParams = 0;

	// HELPER CLASSES
	FaustStrings faustStrings;
	MusicInfoCompute musicInfoCompute;
	MixerSettings mixerSettings;
	MappingPreset mappingPresets[20];

	// MAPPING FUNCTIONS
	short numMapFunc = 7;
	String mapFunc_Names[20] =
	{
		"Ln",
		"Ex1",
		"Ex2",
		"Lg1",
		"Lg2",
		"Sgm",
		"Lgt"
	};

	// MAPPING MATRIX
	bool mappingMatrix[40][40] = { false };
	float mappingStrength[40][40];
	void updateMappingMatrix(short row, short col, bool onOff)
	{
		mappingMatrix[row][col] = onOff;
	}

	double fbVar_FinalVals[40][4] = { 
		{0,0,0,0}
	};

	// MAIN CALLBACK FUNCTION IN CLASS
	void updateFBVariables(MovementParameter mpArray[], int numMP)
	{
		double fbVar_Value_Temp = 0;
		double fbVar_Values_Final[4] = { 0,0,0,0 };
		int numMP_Mapped = 0;
		float mappingStrength_AbsSum = 0;
		float mapVal_Indiv = 0;

		for (int i = 0; i < numFbVariables; i++)
		{
			fbVar_Value_Temp = 0;
			numMP_Mapped = 0;
			mappingStrength_AbsSum = 0;

			// ONLY COMPUTE IF SOMETHING IS MAPPED TO FBVAR
			if (checkIfFbVarMapped(i, numMP, &numMP_Mapped, &mappingStrength_AbsSum))
			{

				// FIND FRACTION SUM OF ALL MPs MAPPED TO A SINGLE AP (LIMIT TO 1)
				for (int j = 0; j < numMP; j++)
				{
					if (mappingMatrix[j][i])
					{
						mapVal_Indiv = (mpArray[j].value - mpArray[j].minVal)
							/ (mpArray[j].maxVal - mpArray[j].minVal) * mappingStrength[j][i];

						// HANDLE NORMALIZED MP BOUNDING
						mpArray[j].inRange = true;

						if (mapVal_Indiv <= mpArray[j].rangeNorm_MIN) {
							mapVal_Indiv = 0; mpArray[j].inRange = false;
						}
						
						if (mapVal_Indiv >= mpArray[j].rangeNorm_MAX) { 
							mapVal_Indiv = 1; mpArray[j].inRange = false;
						}

						mapVal_Indiv = (mapVal_Indiv - mpArray[j].rangeNorm_MIN) /
										(mpArray[j].rangeNorm_MAX - mpArray[j].rangeNorm_MIN);

						fbVar_Value_Temp += mapVal_Indiv / mappingStrength_AbsSum;
					}
				}

				// CONSTRAIN VALUE BETWEEN 0 and 1
				fbVar_Value_Temp = jlimit(0.0, 1.0, fbVar_Value_Temp);

				// CALCULATE MAPPING FUNCTION FOR EACH AP, APPLY QUANTIZATION IF NEEDED
				applyMapFunc(&fbVar_Value_Temp, i);

				// APPLY SPECIAL PROCESSING (E.G. NOTE FREQUENCY COMPUTATION)
				applySpecialMappingProcessing(&fbVar_Value_Temp, i, fbVar_Values_Final);
			}
			else // SET FB VAR TO 0 IF UNMAPPED
			{
				feedbackVariables[i].value = feedbackVariables[i].defaultVal;
				for (int k = 0; k < 4; k++) fbVar_Values_Final[k] = feedbackVariables[i].defaultVal;
			}

			for (int k = 0; k < 4; k++)
			fbVar_FinalVals[i][k] = fbVar_Values_Final[k];
		}
	}

	bool checkIfFbVarMapped(short fbVar_Idx, short numMp, int *numMP_Mapped, float *mappingStrength_SUM)
	{
		bool isMapped = false;

		for (int i = 0; i < numMp; i++)
		{
			if (mappingMatrix[i][fbVar_Idx] && feedbackVariables[fbVar_Idx].name != "Placeholder")
			{
				isMapped = true;
				*numMP_Mapped += 1;
				*mappingStrength_SUM += fabs(mappingStrength[i][fbVar_Idx]);
			}
		}
		*mappingStrength_SUM = (*numMP_Mapped == 1) ? 1 : *mappingStrength_SUM;
		return isMapped;
	}

	void applyMapFunc(double *val, short fbVar_Idx)
	{
		short mapFuncIdx = feedbackVariables[fbVar_Idx].mapFunc;

		// INVERT POLARITY IF NEEDED
		if (feedbackVariables[fbVar_Idx].polarity == 2) *val = 1 - *val;

		float orig_Range = (feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);
		float orig_MinVal = feedbackVariables[fbVar_Idx].minVal;

		float norm_Range = feedbackVariables[fbVar_Idx].rangeNorm_MAX - feedbackVariables[fbVar_Idx].rangeNorm_MIN;
		float norm_MinVal = feedbackVariables[fbVar_Idx].rangeNorm_MIN;

		float new_Min = orig_MinVal + norm_MinVal * orig_Range;
		float new_Max = new_Min + norm_Range * orig_Range;
		float new_Range = new_Max - new_Min;

		switch (mapFuncIdx)
		{
		case 1:
			break;
		case 2:
			*val = pow(*val, 1.5);
			break;
		case 3:
			*val = pow(*val, 2);
			break;
		case 4:
			*val = pow(*val, 0.75);
			break;
		case 5:
			*val = pow(*val, 0.5);
			break;
		case 6:
			*val = exp((*val - 0.5) * 12) / (1 + exp((*val - 0.5) * 12));
			break;
		case 7:
			break;
		}

		*val = new_Min + *val * (new_Max - new_Min);

		*val = quantizeParam(*val, feedbackVariables[fbVar_Idx].quantLevels_2raisedTo, new_Range, new_Min);

		feedbackVariables[fbVar_Idx].value = jlimit(new_Min, new_Max, (float)*val);
	}

	double quantizeParam(float currentParamValue, int quantLevels_2raisedTo, float range, float min_Final)
	{
		float quantizedParam = 0;
		if (quantLevels_2raisedTo < 1)
			return currentParamValue;

		else
		{
			currentParamValue -= min_Final;
			int numQuantizationSteps = pow(2, quantLevels_2raisedTo);
			float quantizationStepSize = range / (float)numQuantizationSteps;

			float diff = 1000;
			bool stepFound = false;
			for (int i = 0; i < numQuantizationSteps + 1; i++)
			{
				float currentStepForTest = i * quantizationStepSize;
				if (fabs(currentParamValue - currentStepForTest) <= diff && currentParamValue > currentStepForTest)
				{
					stepFound = true;
					diff = currentParamValue - currentStepForTest;
				}
			}
			if (stepFound) quantizedParam = currentParamValue - diff + min_Final;
			return quantizedParam;
		}
	};

	void applySpecialMappingProcessing(double *val, short fbVar_Idx, double fbVar_finalArray[])
	{
		// DEFAULT
		fbVar_finalArray[0] = *val;

		// MEL FREQ
		if (feedbackVariables[fbVar_Idx].name == "Mel Fr" || feedbackVariables[fbVar_Idx].name == "Gtr Fr")
		{
			fbVar_finalArray[0] = (*val - feedbackVariables[fbVar_Idx].minVal) /
				(feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);

			musicInfoCompute.convert_FbVar_to_ScaleDeg_to_Freq_MONO(&fbVar_finalArray[0]);
		}

		// CHORD FREQS
		if (feedbackVariables[fbVar_Idx].name == "Chord Fr")
		{
			fbVar_finalArray[0] = (*val - feedbackVariables[fbVar_Idx].minVal) /
				(feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);

			musicInfoCompute.convert_FbVar_to_ChordDeg_to_Freqs_POLY(fbVar_finalArray);
		}
	}
};