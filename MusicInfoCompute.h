#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MusicInfoCompute
{
public:
	MusicInfoCompute()
	{

	}

	~MusicInfoCompute() {};

	short numScales = 7;
	short numTonics = 12;
	String scales_Names[10] =
	{
		"Major",
		"Minor",
		"Phrygian",
		"Dorian",
		"Mixolydian",
		"Lydian",
		"Pentatonic"
	};
	String tonics_Names[12] =
	{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	short tonics_Offsets[12] =
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

	bool isRandom_MEL = false;
	short order_MEL_OCT_2[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	short order_MEL_OCT_1[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	void reset_order_MEL()	{	
		for (int i = 0; i < 16; i++) order_MEL_OCT_2[i] = i;	
		for (int i = 0; i < 8; i++) order_MEL_OCT_1[i] = i;
	}
	void set_order_MEL(int arrayPos, int scaleIdx)
	{
		order_MEL_OCT_1[arrayPos] = scaleIdx;
		order_MEL_OCT_2[arrayPos] = scaleIdx;
		order_MEL_OCT_1[arrayPos + 8] = scaleIdx + 8;
	}
	void randomize_order_MEL()
	{
		if (isRandom_MEL)
		{
			for (int i = 0; i < 16; i++) order_MEL_OCT_2[i] = -1;
			for (int i = 0; i < 8; i++) order_MEL_OCT_1[i] = -1;
			order_MEL_OCT_1[0] = 0;	order_MEL_OCT_2[0] = 0;

			for (int i = 0; i < 16; i++)
				order_MEL_OCT_2[i] = getNewRandomIndex(16, i, order_MEL_OCT_2);

			for (int i = 0; i < 8; i++)
				order_MEL_OCT_2[i] = getNewRandomIndex(9, i, order_MEL_OCT_1);
		}
	}

	Random randGen;
	int getNewRandomIndex(int total, int totalElapsed, short* elapsedIndices)
	{
		int randomIndex = randGen.nextInt(total);
		bool alreadyDone = false;

		for (int i = 0; i < total; i++)
		{
			if (randomIndex == elapsedIndices[i])
				alreadyDone = true;
		} //Check if already done

		while (alreadyDone)
		{
			randomIndex = randGen.nextInt(total);
			alreadyDone = false;
			for (int i = 0; i < total; i++)
			{
				if (randomIndex == elapsedIndices[i])
					alreadyDone = true;
			}
		}

		elapsedIndices[totalElapsed] = randomIndex;
		return randomIndex;
	}

	short idx_scale_Present = 0;
	short idx_tonic_Present = 0;
	short idx_chordTypes_Present = 0;
	short idx_chord_degSeq_Present = 0;

	short chord_degSequence[8] =
	{
		1, 2, 3, 4, 5, 6, 7, 8
	};

	short numChordTypes = 6;
	String chordTypes_Names[10] =
	{
		"Normal",
		"Sevenths",
		"Sixths",
		"Ninths",
		"Sus4",
		"Unison"
	};

	int chordTypes_DegreeIntervals[10][4] =
	{
		{0, 2, 4, 7},
		{0, 2, 4, 6},
		{0, 2, 4, 5},
		{0, 2, 4, 8},
		{0, 2, 3, 4},
		{0, 0, 7, 7}
	};

	// COMPLETE REMAINING SCLES
	short scales[10][17] =
	{
		{0,0,2,4,5,7,9,11,12,14, 16, 17, 19, 21, 23, 24, 26},	//Major
		{0,0,2,3,5,7,8,10,12,14, 15, 17, 19, 20, 22, 24, 26},	//Minor
		{0,0,1,3,5,7,8,10,12,13, 15, 17, 19, 20, 22, 24, 25},	//Phrygian
		{0,0,2,3,5,7,9,10,12,14, 15, 17, 19, 21, 22, 24, 26},	//Dorian
		{0,0,2,4,5,7,9,10,12,14, 16, 17, 19, 21, 22, 24, 26},	//Mixolydian
		{0,0,2,4,6,7,9,11,12,14, 16, 18, 19, 21, 23, 24, 26},	//Lydian
		{0,0,3,5,7,10,12,15,17,19, 22, 24, 27, 29}				//Pentatonic
	};

	void convert_FbVar_to_ScaleDeg_to_Freq_MONO_GEN(double* fbVar, float numDegs, int baseKey)
	{
		short* orderVector;
		orderVector = (numDegs > 9) ? order_MEL_OCT_2 : order_MEL_OCT_1;

		if (!isnan(*fbVar))
		{
			int scaleDeg = (int)(*fbVar * numDegs);
			*fbVar = MidiMessage::getMidiNoteInHertz(baseKey + tonics_Offsets[idx_tonic_Present] +
				scales[idx_scale_Present][min((int)numDegs, (int)orderVector[scaleDeg])]);
		}
	}

	// FB Var between 0 and 1
	void convert_FbVar_to_ScaleDeg_to_Freq_MONO(double *fbVar)
	{
		if (!isnan(*fbVar))
		{
			int scaleDeg = (int)(*fbVar * 16.0);
			*fbVar = MidiMessage::getMidiNoteInHertz(48 + tonics_Offsets[idx_tonic_Present] +
				scales[idx_scale_Present][min(16, scaleDeg)]) / 4.0;
		}
	}

	void convert_FbVar_to_ChordDeg_to_Freqs_POLY(double fbVar[])
	{
		if (!isnan(fbVar[0]))
		{
			int chordDeg = chord_degSequence[(int)(fbVar[0] * 9)];
			int midiKeys[4] = { 36,36,36,36 };
			for (int i = 0; i < 4; i++)
			{
				midiKeys[i] = 36 + tonics_Offsets[idx_tonic_Present]
					+ scales[idx_scale_Present][chordDeg +
					chordTypes_DegreeIntervals[idx_chordTypes_Present][i]];
				fbVar[i] = MidiMessage::getMidiNoteInHertz(midiKeys[i]);
			}
		}
	}

	void convert_FbVar_to_FluteFreq(double fbVar[])
	{
		if (!isnan(*fbVar))
		{
			int scaleDeg = (int)(*fbVar * 8.1);
			*fbVar = MidiMessage::getMidiNoteInHertz(60 + tonics_Offsets[idx_tonic_Present] +
				scales[idx_scale_Present][min(8, scaleDeg)]);
		}
	}
};