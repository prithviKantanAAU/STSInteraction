#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GaitParam_Single.h"
#include "AudioParam_Single.h"
#include "FaustStrings.h"
#include "MusicInfoCompute.h"
#include "MixerSettings.h"
#include "MappingPreset.h"
#include "BiQuad.h"

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
		
		//								 NAME	   MIN  MAX	 DEF    POL F  Q   #CON TYPE  VIS
		
		//	TRIGGERS

		feedbackVariables[0].initialize("T_Djemb", 50, 100, 50,	 1, 1, 3,	 1, 1			);
		feedbackVariables[2].initialize("T_Voice", 50, 100, 50,		 1, 1, 3,	 1, 1			);
		feedbackVariables[4].initialize("T_Piano", 0, 100, 0,		 1, 1, 1,	 4, 1			);
		feedbackVariables[23].initialize("T_Flute", 50, 100, 50,	 1, 1, 5,	 1, 1			);
		feedbackVariables[11].initialize("T_Gtr", 0, 100, 50,		 1, 1, 3,	 1, 1			);
		feedbackVariables[16].initialize("T_Bell", 50, 100, 50,		 1, 1, 0,	 1, 1			);
		
		//	FUNDAMENTAL FREQUENCIES

		feedbackVariables[8].initialize("F_Djemb", 150, 600, 150,	 1, 1, 0,	 1, 2			);		
		feedbackVariables[1].initialize("F_Voice", 90, 700, 90,		 2, 1, 0,	 1, 2			);
		feedbackVariables[3].initialize("F_Piano", 50, 1000, 50,	 1, 1, 0,	 4, 2			);
		feedbackVariables[7].initialize("F_Flute", 420, 1320, 420,	 2, 1, 0,	 1, 2			);
		feedbackVariables[18].initialize("F_Gtr", 50, 1500, 50,		 2, 1, 0,	 1, 2			);

		// DYNAMICS

		feedbackVariables[14].initialize("D_Djemb", 0.01, 4.5, 1,	 1, 1, 0,	 1, 3			);
		feedbackVariables[13].initialize("D_Voice", 0, 1, 1,		 1, 1, 0,	 1, 3			);
		feedbackVariables[6].initialize("D_Piano", 0, 1, 1,			 1, 1, 3,	 1, 3			);
		feedbackVariables[15].initialize("D_Flute", 0.7, 1, 0.9,	 1, 1, 0,	 1, 3			);
		feedbackVariables[17].initialize("D_Gtr", 20, 1500, 1500,	 1, 1, 0,	 1, 3			);

		// TIMBRE

		feedbackVariables[10].initialize("Vowel", 0, 2, 0,			 1, 1, 0,	 1, 4,	 false	);
		feedbackVariables[12].initialize("Gtr Stf", 0.01, 0.4, 0.1,	 1, 1, 0,	 1, 4,	 false	);

		// NEGATIVE FEEDBACK
		
		feedbackVariables[5].initialize("Detune", 0, 1, 0,			 1, 1, 0,	 1, 5,	 false	);
		feedbackVariables[9].initialize("F_Warp"	, 0.5, 1, 0.5,	 1, 1, 0,	 1, 5,	 false	);
		feedbackVariables[22].initialize("Flute Pos", 0, 0.5, 0.5,	 2, 1, 0,	 1, 5,	 false	);
		
		// SINE WAVES		
		
		feedbackVariables[19].initialize("Sin1 F", 300, 500, 300,	 1, 1, 0,	 1, 6,	 false	);
		feedbackVariables[20].initialize("Sin2 F", 600, 1000, 600,	 1, 1, 0,	 1, 6,   false	);
		feedbackVariables[21].initialize("Sin3 F", 1200, 2000, 1200, 1, 1, 0,	 1, 6,   false	);

		populate_num_AP();
		populateDispIndex_AP();
		apSmoothing_INIT();
	};
	~MusicControl() 
	{
	};

	short numFbVariables = 0;
	void populate_num_AP()
	{
		for (int i = 0; i < 40; i++)
		{
			if (feedbackVariables[i].name != "Placeholder")
				numFbVariables++;
		}
	}

	// Smoothing Filter Definition and Initialization
	BiQuad apSmoothing[40];
	void apSmoothing_INIT()
	{
		for (int i = 0; i < 40; i++)
			apSmoothing[i].calculateLPFCoeffs(feedbackVariables[i].freq_Smoothing, 0.7, 100);
	};

	void apSmoothing_SET_FREQ(int idx, float freq)
	{
		apSmoothing[idx].calculateLPFCoeffs(freq, 0.7, 100);
	}

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
		setDispIndex_AP("T_Djemb", 0);
		setDispIndex_AP("T_Voice", 1);
		setDispIndex_AP("T_Piano", 2);
		setDispIndex_AP("T_Flute", 3);
		setDispIndex_AP("T_Gtr", 4);
		setDispIndex_AP("T_Bell", 5);
		
		setDispIndex_AP("F_Djemb", 6);
		setDispIndex_AP("F_Voice", 7);
		setDispIndex_AP("F_Piano", 8);
		setDispIndex_AP("F_Flute", 9);
		setDispIndex_AP("F_Gtr", 10);
		
		setDispIndex_AP("D_Djemb", 11);
		setDispIndex_AP("D_Voice", 12);
		setDispIndex_AP("D_Piano", 13);
		setDispIndex_AP("D_Flute", 14);
		setDispIndex_AP("D_Gtr", 15);

		setDispIndex_AP("Vowel", 16);
		setDispIndex_AP("Gtr Stf", 17);
		
		setDispIndex_AP("Detune", 18);
		setDispIndex_AP("Pitch Warp", 19);
		setDispIndex_AP("Flute Pos", 20);

		setDispIndex_AP("Sin1 F", 21);
		setDispIndex_AP("Sin2 F", 22);
		setDispIndex_AP("Sin3 F", 23);
	}

	// FAUST OBJECT
	bool isMusicDSP_On = false;

	FeedbackVariable feedbackVariables[40];
	short numMovementParams = 0;

	// HELPER CLASSES
	FaustStrings faustStrings;
	MusicInfoCompute musicInfoCompute;
	MixerSettings mixerSettings;
	MappingPreset mappingPresets[50];

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

						if (mpArray[j].inRange)
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
		float norm_MaxVal = feedbackVariables[fbVar_Idx].rangeNorm_MAX;

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

		// Normalize *val within normalized AP range
		*val = norm_MinVal + *val * (norm_MaxVal - norm_MinVal);

		//*val = fmax(feedbackVariables[fbVar_Idx].rangeNorm_MIN, *val);
		*val = new_Min + *val * (new_Max - new_Min);

		if (feedbackVariables[fbVar_Idx].freq_Smoothing <= 48)
		*val = apSmoothing[fbVar_Idx].doBiQuad(*val, 0);

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
		if (feedbackVariables[fbVar_Idx].name == "F_Voice" || feedbackVariables[fbVar_Idx].name == "F_Gtr")
		{
			fbVar_finalArray[0] = (*val - feedbackVariables[fbVar_Idx].minVal) /
				(feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);

			//musicInfoCompute.convert_FbVar_to_ScaleDeg_to_Freq_MONO(&fbVar_finalArray[0]);
			musicInfoCompute.convert_FbVar_to_ScaleDeg_to_Freq_MONO_GEN(&fbVar_finalArray[0],8,24);
		}

		if (feedbackVariables[fbVar_Idx].name == "F_Flute")
		{
			fbVar_finalArray[0] = (*val - feedbackVariables[fbVar_Idx].minVal) /
				(feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);

			//musicInfoCompute.convert_FbVar_to_FluteFreq(&fbVar_finalArray[0]);
			musicInfoCompute.convert_FbVar_to_ScaleDeg_to_Freq_MONO_GEN(&fbVar_finalArray[0], 8, 60);
		}

		// CHORD FREQS
		if (feedbackVariables[fbVar_Idx].name == "F_Piano")
		{
			fbVar_finalArray[0] = (*val - feedbackVariables[fbVar_Idx].minVal) /
				(feedbackVariables[fbVar_Idx].maxVal - feedbackVariables[fbVar_Idx].minVal);

			musicInfoCompute.convert_FbVar_to_ChordDeg_to_Freqs_POLY(fbVar_finalArray);
		}
	}
};