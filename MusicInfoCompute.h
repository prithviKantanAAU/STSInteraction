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

	short idx_scale_Present = 0;
	short idx_tonic_Present = 0;
	short idx_chordTypes_Present = 0;
	short idx_chord_degSeq_Present = 0;

	short chord_degSequences[7][8] =
	{
		{1, 2, 3, 4, 5, 6, 7, 8},
		{1, 4, 5, 1, 1, 4, 5, 1},
		{1, 6, 4, 5, 1, 6, 4, 5},
		{1, 3, 2, 1, 5, 4, 3, 1}
	};

	short numChordTypes = 5;
	String chordTypes_Names[10] =
	{
		"Normal",
		"Sevenths",
		"Sixths",
		"Ninths",
		"Sus4"
	};

	int chordTypes_DegreeIntervals[10][4] =
	{
		{0, 2, 4, 7},
		{0, 2, 4, 6},
		{0, 2, 4, 5},
		{0, 2, 4, 8},
		{0, 2, 3, 4}
	};

	// COMPLETE REMAINING SCLES
	short scales[10][17] =
	{
		{-1,0,2,4,5,7,9,11,12,14, 16, 17, 19, 21, 23, 24, 26},	//Major
		{-2,0,2,3,5,7,8,10,12,14, 15, 17, 19, 20, 22, 24, 26},	//Minor
		{-2,0,1,3,5,7,8,10,12,13, 15, 17, 19, 20, 22, 24, 25},	//Phrygian
		{-2,0,2,3,5,7,9,10,12,14, 15, 17, 19, 21, 22, 24, 26},	//Dorian
		{-2,0,2,4,5,7,9,10,12,14, 16, 17, 19, 21, 22, 24, 26},	//Mixolydian
		{-1,0,2,4,6,7,9,11,12,14, 16, 18, 19, 21, 23, 24, 26},	//Lydian
		{-2,0,3,5,7,10,12,15,17,19, 22, 24, 27, 29}				//Pentatonic
	};

	// FB Var between 0 and 1
	void convert_FbVar_to_ScaleDeg_to_Freq_MONO(double *fbVar)
	{
		int scaleDeg = (int)(*fbVar * 16.0);
		*fbVar = MidiMessage::getMidiNoteInHertz(48 + scales[idx_scale_Present]	[scaleDeg]);
	}

	void convert_FbVar_to_ChordDeg_to_Freqs_POLY(double fbVar[])
	{
		int chordDeg = (int)(fbVar[0] * 8);
		int midiKeys[4] = { 36,36,36,36 };
		for (int i = 0; i < 4; i++)
		{
			midiKeys[i] = 48 + tonics_Offsets[idx_tonic_Present] 
				+ scales[idx_scale_Present][chordDeg + 
				chordTypes_DegreeIntervals[idx_chordTypes_Present][i]];
			fbVar[i] = MidiMessage::getMidiNoteInHertz(midiKeys[i]);
		}
	}
};