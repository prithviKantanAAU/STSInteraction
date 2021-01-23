#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GaitParam_Single.h"
#include "AudioParam_Single.h"
#include "FaustStrings.h"
#include "MusicInfoCompute.h"
#include "MixerSettings.h"
#include "DspFaust.h"

class MusicControl
{
public:
	MusicControl()
	{
		// Initialize Audio Params !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// initialize(String apName, float mini, float maxi, float defaultVal,
		//            short pol, short mapF, short numSynthControls)
		feedbackVariables[0].initialize("Perc Tr", 0, 5, 0, 0, 1, 4, 1);
		feedbackVariables[1].initialize("Mel Fr", 100, 700, 100, 1, 1, 4, 1);
		feedbackVariables[2].initialize("Mel Tr", 0, 1, 0, 0, 1, 4, 1);
		feedbackVariables[3].initialize("Chord Fr", 50, 1000, 50, 1, 2, 3, 4);
		feedbackVariables[4].initialize("Chord Tr", 0, 1, 0, 1, 2, 3, 4);
		feedbackVariables[5].initialize("Detune", 0, 1, 0, 1, 6, 0, 1);
		feedbackVariables[6].initialize("Pan", 0, 1, 0.5, 1, 5, 0, 1);
		feedbackVariables[7].initialize("Perc2 Tr", 0, 1, 0, 1, 4, 4, 1);
		feedbackVariables[8].initialize("Dynamics", 7, 10, 7, 1, 2, 0, 1);
		feedbackVariables[9].initialize("Pitch Warp", 0, 1, 0.5, 1, 2, 0, 1);
		feedbackVariables[10].initialize("Vowel", 0, 3, 0, 1, 2, 0, 1);
	};
	~MusicControl() 
	{
	};

	// FAUST OBJECT
	DspFaust dspFaust;
	bool isMusicDSP_On = false;

	FeedbackVariable feedbackVariables[20];
	short numFbVariables = 11;

	// HELPER CLASSES
	FaustStrings faustStrings;
	MusicInfoCompute musicInfoCompute;
	MixerSettings mixerSettings;

	// MAPPING FUNCTIONS
	short numMapFunc = 6;
	String mapFunc_Names[20] =
	{
		"Ln",
		"Ex1",
		"Ex2",
		"Lg1",
		"Lg2",
		"Sgm"
	};

	// MAPPING MATRIX
	bool mappingMatrix[20][20] = { false };
	void updateMappingMatrix(short row, short col, bool onOff)
	{
		mappingMatrix[row][col] = onOff;
	}

	// MAIN CALLBACK FUNCTION IN CLASS
	void updateFBVariables(MovementParameter mpArray[], int numMP)
	{
		double fbVar_Value_Temp = 0;
		double fbVar_FinalVals[4] = { 0,0,0,0 };

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
				applySpecialMappingProcessing(&fbVar_Value_Temp, i, fbVar_FinalVals);
			}
			else // SET FB VAR TO 0 IF UNMAPPED
			{
				feedbackVariables[i].value = feedbackVariables[i].defaultVal;
				for (int k = 0; k < 4; k++) fbVar_FinalVals[k] = feedbackVariables[i].defaultVal;
			}

			// MAP TO FAUST
			mapFBVariable(i, fbVar_FinalVals);
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

		*val += feedbackVariables[fbVar_Idx].minVal;

		feedbackVariables[fbVar_Idx].value = *val;
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
			if (stepFound) quantizedParam = currentParamValue - diff;
			return quantizedParam;
		}
	};

	void applySpecialMappingProcessing(double *val, short fbVar_Idx, double fbVar_finalArray[])
	{
		// DEFAULT
		fbVar_finalArray[0] = *val;

		// MEL FREQ
		if (feedbackVariables[fbVar_Idx].name == "Mel Fr")
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

	void mapFBVariable(int fbVar_Idx, double fbVar_finalValues[])
	{
		// FOR EACH SYNTH CONTROL
		for (int i = 0; i < feedbackVariables[fbVar_Idx].numSynthControls; i++)
		{
			// MAP ARRAY VALUES TO DSPFAUST CONTROLS
			dspFaust.setParamValue(faustStrings.getFBVar_FAUSTAddress_Full(fbVar_Idx, i).c_str(),
			fbVar_finalValues[i]);
		}
	}

	// WHEN MUSIC PLAYBACK IS ENABLED - SAME AS PREPARE TO PLAY
	void startMusicDSP()
	{
		isMusicDSP_On = true;
		dspFaust.start();

		// CONFIGURE MIXER FADERS, MUTES, EQ, COMP, MASTER GAIN
		set_masterGain(mixerSettings.masterGain);
		for (int i = 0; i < mixerSettings.num_Tracks; i++)
			set_trackFader(i, mixerSettings.gain_Track[i]);
	}

	// WHEN MUSIC PLAYBACK IS DISABLED
	void stopMusicDSP()
	{
		isMusicDSP_On = false;
		dspFaust.stop();
	}

	void set_masterGain(float faderVal)
	{
		mixerSettings.masterGain = faderVal;
		dspFaust.setParamValue(faustStrings.masterGain.toStdString().c_str(), faderVal);
	}

	void set_trackFader(short trackIdx, float faderVal)
	{
		mixerSettings.gain_Track[trackIdx] = faderVal;
		dspFaust.setParamValue(faustStrings.getTrackGainAddress(trackIdx).c_str(), faderVal);
	}
};