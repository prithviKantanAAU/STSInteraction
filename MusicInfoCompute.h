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

	short scales[10][16] =
	{
		{-1,0,2,4,5,7,9,11,12,14, 16, 17, 19, 21, 23, 24},		//Major
		{-2,0,2,3,5,7,8,10,12,14, 15, 17, 19, 20},				//Minor
		{-2,0,1,3,5,7,8,10,12,13, 15, 17, 19, 20},				//Phrygian
		{-2,0,2,3,5,7,9,10,12,14, 15, 17, 19, 21},				//Dorian
		{-2,0,2,4,5,7,9,10,12,14, 16, 17, 19, 21},				//Mixolydian
		{-1,0,2,4,6,7,9,11,12,14, 16, 18, 19, 21},				//Lydian
		{-2,0,3,5,7,10,12,15,17,19, 22, 24, 27, 29}				//Pentatonic
	};

	void convert_FbVar_to_ScaleDeg_to_Freq_MONO(double *fbVar)
	{

	}

	void convert_FbVar_to_ChordDeg_to_Freqs_POLY(double fbVar[])
	{

	}
};