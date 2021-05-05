#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioParam_Single.h";
#include "GaitParam_Single.h"

class UI_MappingMatrix
{
    public:
		UI_MappingMatrix()
    {
        configure();
    }
    
    ~UI_MappingMatrix()
    {}

	//MEMBER ELEMENTS
	// STEPS TO ADD NEW CONTROLS
	// Declare here
	// Add and make visible in PluginEditor.h (add to existing function)
	// Initialize here
	// Initialize in PluginEditor.h
	// Add SetVisible to ToggleVisible() here
	// Set position in SetLayout() here

	Label labels_movementParams[40];
	Label labels_audioParams[40];
	ToggleButton mapping_Matrix[40][40];
	Slider mapping_Strength[40][40];
	Slider normRange_MP[40];
	Slider normRange_AP[40];
	Slider apSmoothing_Fc[40];
	ComboBox mapping_Function[40];
	ComboBox mapping_Polarity[40];
	ComboBox mapping_QuantLevels[40];
	Label movementParams_Value[40][2];		// 0 = Filled // 1 = Unfilled
	Label audioParams_Value[40][2];
	float width_Value = 90;
	float width_Value_AP = 50;
	int num_MP = 0;
	int num_AP = 0;

	// OSC CONTROL
	Slider triOsc_BPM;
	Label triOsc_BPM_LAB;

	// CHANGE LAYOUT
	ComboBox mp_isVisible;
	ComboBox mp_isHidden;
	ComboBox ap_isVisible;
	ComboBox ap_isHidden;

	// PRESETS
	String lineHeaders_Main[40];
	int num_lineHeaders = 0;
	int num_linesTotal = 0;
	int numRows_inHeader[40];
	int numCols_inRow[40];
	String rowData[1000];
	String rowHeaders[1000];

	TextButton preset_Save;
	ComboBox preset_ListLoad;
	Label  preset_ListLoad_LAB;
	TextEditor preset_Name;
	FILE *presetFile;

	// PRESETS - DATA HOLDERS
	float dataHolder_oscBPM = 1;
	bool dataHolder_voiceCue_ON = false;
	float dataHolder_voiceCue_timingFine = 0;
	float dataHolder_voiceCue_voldB = 0;
	int dataHolder_voiceCue_Length = 1;
	bool dataHolder_voiceCue_isIntervalEnabled[8] = { true, true, false };
	bool dataHolder_voiceCue_isPos[8] = { true, false, true };
	float dataHolder_voiceCue_location[8] = { 0.5, 0.5, 0.5 };
    
    void configure()
    {
		for (int i = 0; i < 40; i++)
		{
			movementParams_Value[i][0].setColour(movementParams_Value[i][0].textColourId, Colours::black);
			movementParams_Value[i][0].setColour(movementParams_Value[i][0].backgroundColourId, Colours::yellow);
			movementParams_Value[i][1].setColour(movementParams_Value[i][1].backgroundColourId, Colours::blue);
			audioParams_Value[i][0].setColour(audioParams_Value[i][0].backgroundColourId, Colours::yellow);
			audioParams_Value[i][1].setColour(audioParams_Value[i][1].backgroundColourId, Colours::blue);
			labels_movementParams[i].setJustificationType(Justification::centred);
			labels_audioParams[i].setJustificationType(Justification::centred);

			normRange_MP[i].setRange(0, 1);
			normRange_MP[i].setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
			normRange_MP[i].setMinValue(0);
			normRange_MP[i].setMaxValue(1);
			normRange_MP[i].setColour(normRange_MP[i].trackColourId, Colours::yellow);
			normRange_MP[i].setColour(normRange_MP[i].backgroundColourId, Colours::blue);
			normRange_MP[i].setTextBoxStyle(Slider::NoTextBox,false,10,10);

			normRange_AP[i].setRange(0, 1);
			normRange_AP[i].setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
			normRange_AP[i].setMinValue(0);
			normRange_AP[i].setMaxValue(1);
			normRange_AP[i].setColour(normRange_AP[i].trackColourId, Colours::yellow);
			normRange_AP[i].setColour(normRange_AP[i].backgroundColourId, Colours::blue);
			normRange_AP[i].setTextBoxStyle(Slider::NoTextBox, false, 10, 10);

			apSmoothing_Fc[i].setRange(1, 49);
			apSmoothing_Fc[i].setColour(apSmoothing_Fc[i].trackColourId, Colours::yellow);
			apSmoothing_Fc[i].setColour(apSmoothing_Fc[i].backgroundColourId, Colours::blue);
			apSmoothing_Fc[i].setTextBoxStyle(Slider::NoTextBox, false, 10, 10);
			apSmoothing_Fc[i].setSkewFactor(0.5);
		}

		preset_Name.setJustification(Justification::centred);
		preset_Save.setButtonText("Save as Preset");

		mp_isVisible.setColour(mp_isVisible.backgroundColourId, Colours::green);
		mp_isHidden.setColour(mp_isHidden.backgroundColourId, Colours::red);
		ap_isVisible.setColour(ap_isVisible.backgroundColourId, Colours::green);
		ap_isHidden.setColour(ap_isHidden.backgroundColourId, Colours::red);

		mp_isVisible.addItem("VISIBLE MP", 100);	mp_isVisible.setSelectedId(100);
		mp_isHidden.addItem("HIDDEN MP", 100);		mp_isHidden.setSelectedId(100);
		ap_isVisible.addItem("VISIBLE AP", 100);	ap_isVisible.setSelectedId(100);
		ap_isHidden.addItem("HIDDEN AP", 100);		ap_isHidden.setSelectedId(100);

		// Tri Osc BPM
		triOsc_BPM.setRange(10, 100);
		triOsc_BPM.setSkewFactor(0.3);
		triOsc_BPM.setSliderStyle(Slider::SliderStyle::Rotary);
		triOsc_BPM.setTextBoxStyle(Slider::NoTextBox, true, 20, 20);
		triOsc_BPM.setValue(60);
		triOsc_BPM.setColour(triOsc_BPM.rotarySliderFillColourId, Colours::yellow);
		triOsc_BPM.setColour(triOsc_BPM.backgroundColourId, Colours::blue);
    }

	void configurePresetFields()
	{
		// INITIALIZE ARRAY CONTENTS
		for (int i = 0; i < 40; i++)
		{
			lineHeaders_Main[i] = "";
			numRows_inHeader[i] = 0;
			numCols_inRow[i] = 0;
		};

		for (int j = 0; j < 1000; j++)
		{
			rowData[j] = "";
		}

		// AP Name Row
		lineHeaders_Main[0] = "AP Names";
		numRows_inHeader[0] = 1;
		numCols_inRow[0] = num_AP;

		// Mapping Matrix Booleans
		lineHeaders_Main[1] = "isMap_";
		numRows_inHeader[1] = num_MP;
		numCols_inRow[1] = num_AP;

		// Mapping Strength
		lineHeaders_Main[2] = "mapStr_";
		numRows_inHeader[2] = num_MP;
		numCols_inRow[2] = num_AP;

		// Mapping Function Index
		lineHeaders_Main[3] = "Map Func Idx";
		numRows_inHeader[3] = 1;
		numCols_inRow[3] = num_AP;

		// Mapping Polarity
		lineHeaders_Main[4] = "Polarity";
		numRows_inHeader[4] = 1;
		numCols_inRow[4] = num_AP;

		// Quantization Bits
		lineHeaders_Main[5] = "Quant Bits";
		numRows_inHeader[5] = 1;
		numCols_inRow[5] = num_AP;

		// Oscillator BPM
		lineHeaders_Main[6] = "OSC_BPM";
		numRows_inHeader[6] = 1;
		numCols_inRow[6] = 1;

		// Voice Cue ON
		lineHeaders_Main[7] = "VoiceCue_ON";
		numRows_inHeader[7] = 1;
		numCols_inRow[7] = 1;

		// Voice Cue - Timing (Fine)
		lineHeaders_Main[8] = "VoiceCue_TimingFine";
		numRows_inHeader[8] = 1;
		numCols_inRow[8] = 1;

		// Voice Cue - Level (dB)
		lineHeaders_Main[9] = "VoiceCue_LeveldB";
		numRows_inHeader[9] = 1;
		numCols_inRow[9] = 1;

		// Voice Cue - Length
		lineHeaders_Main[10] = "VoiceCue_Length";
		numRows_inHeader[10] = 1;
		numCols_inRow[10] = 1;

		// Voice Cue - Is Interval Enabled?
		lineHeaders_Main[11] = "VoiceCue_isIntervalEnabled";
		numRows_inHeader[11] = 1;
		numCols_inRow[11] = 3;

		// Voice Cue - Interval Location
		lineHeaders_Main[12] = "VoiceCue_intervalLocation";
		numRows_inHeader[12] = 1;
		numCols_inRow[12] = 3;

		// Map Threshold - AP - Min
		lineHeaders_Main[13] = "mapThresh_AP_Min";
		numRows_inHeader[13] = 1;
		numCols_inRow[13] = num_AP;

		// Map Threshold - AP - Max
		lineHeaders_Main[14] = "mapThresh_AP_Max";
		numRows_inHeader[14] = 1;
		numCols_inRow[14] = num_AP;

		// Map Threshold - MP
		lineHeaders_Main[15] = "mapThresh_MP_";
		numRows_inHeader[15] = num_MP;
		numCols_inRow[15] = 2;

		// AP Smoothing Fc
		lineHeaders_Main[15] = "apSmoothing_Fc";
		numRows_inHeader[15] = 1;
		numCols_inRow[15] = num_AP;

		// Find Num Line Headers
		for (int i = 0; i < 40; i++)
		{
			if (lineHeaders_Main[i] != "")
			{
				num_lineHeaders += 1;
				num_linesTotal += numRows_inHeader[i];
			}		
		};
	}

	// STEPS TO ADD NEW DATA TO PRESET
	// 1 - Add info in configurePresetFields() function
	// 2 - Add Data Population Specifics
	// 3 - Add Data Reading Specifics

	void saveAsPreset(MovementParameter mpArray[], FeedbackVariable apArray[])
	{
		if (preset_Name.getText() == "")
			return;
		else
		{
			configurePresetFields();

			String presetFile_Path = "";
			// INITIALIZE PATHS, STORE INFO
			presetFile_Path = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
			presetFile_Path = presetFile_Path.upToLastOccurrenceOf("\\", true, false);
			presetFile_Path += "Mapping Presets\\" + preset_Name.getText() + ".csv";
			
			presetFile = fopen(presetFile_Path.toStdString().c_str(), "w");
			
			int lineIterator = 0;

			// POPULATE ROW DATA MATRIX
			for (int i = 0; i < num_lineHeaders; i++)
			{
				if (lineHeaders_Main[i] == "AP Names")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];

					for (int k = 0; k < numCols_inRow[i]; k++)
					{
						rowData[lineIterator] += apArray[k].name + ",";
					}

					lineIterator++;
				}

				if (lineHeaders_Main[i] == "isMap_")
				{
					for (int j = 0; j < numRows_inHeader[i]; j++)
					{
						rowHeaders[lineIterator] = lineHeaders_Main[i] + mpArray[j].name;
						
						for (int k = 0; k < numCols_inRow[i]; k++)
						{
							rowData[lineIterator] += mapping_Matrix[j][k].getToggleState() ? "1," : "0,";
						}

						lineIterator++;
					}
				}

				if (lineHeaders_Main[i] == "mapStr_")
				{
					for (int j = 0; j < numRows_inHeader[i]; j++)
					{
						rowHeaders[lineIterator] = lineHeaders_Main[i] + mpArray[j].name;

						for (int k = 0; k < numCols_inRow[i]; k++)
						{
							rowData[lineIterator] += String(mapping_Strength[j][k].getValue(),2) + ",";
						}

						lineIterator++;
					}
				}

				if (lineHeaders_Main[i] == "Map Func Idx")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					
					for (int k = 0; k < numCols_inRow[i]; k++)
					{
						rowData[lineIterator] += String(mapping_Function[k].getSelectedId()) + ",";
					}

					lineIterator++;
				}

				if (lineHeaders_Main[i] == "Polarity")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];

					for (int k = 0; k < numCols_inRow[i]; k++)
					{
						rowData[lineIterator] += String(mapping_Polarity[k].getSelectedId()) + ",";
					}

					lineIterator++;
				}

				if (lineHeaders_Main[i] == "Quant Bits")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];

					for (int k = 0; k < numCols_inRow[i]; k++)
					{
						rowData[lineIterator] += String(mapping_QuantLevels[k].getSelectedId() - 1) + ",";
					}

					lineIterator++;
				}

				if (lineHeaders_Main[i] == "OSC_BPM")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					rowData[lineIterator] += String(dataHolder_oscBPM,4);
					lineIterator++;
				}

				if (lineHeaders_Main[i] == "VoiceCue_ON")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					rowData[lineIterator] += dataHolder_voiceCue_ON ? "1" : "0";
					lineIterator++;
				}

				if (lineHeaders_Main[i] == "VoiceCue_TimingFine")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					rowData[lineIterator] += String(dataHolder_voiceCue_timingFine, 2);
					lineIterator++;
				}

				if (lineHeaders_Main[i] == "VoiceCue_LeveldB")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					rowData[lineIterator] += String(dataHolder_voiceCue_voldB, 2);
					lineIterator++;
				}

				if (lineHeaders_Main[i] == "VoiceCue_Length")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					rowData[lineIterator] += String(dataHolder_voiceCue_Length);
					lineIterator++;
				}

				if (lineHeaders_Main[i] == "VoiceCue_isIntervalEnabled")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					for (int k = 0; k < numCols_inRow[i]; k++)
					rowData[lineIterator] += String(dataHolder_voiceCue_isIntervalEnabled[k] ? "1," : "0,");
					lineIterator++;
				}

				if (lineHeaders_Main[i] == "VoiceCue_isIntervalPos")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					for (int k = 0; k < numCols_inRow[i]; k++)
					rowData[lineIterator] += String(dataHolder_voiceCue_isPos[k] ? "1," : "0,");
					lineIterator++;
				}

				if (lineHeaders_Main[i] == "VoiceCue_intervalLocation")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];
					for (int k = 0; k < numCols_inRow[i]; k++)
					rowData[lineIterator] += String(dataHolder_voiceCue_location[k], 2) + ",";
					lineIterator++;
				}

				if (lineHeaders_Main[i] == "mapThresh_AP_Min")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];

					for (int k = 0; k < numCols_inRow[i]; k++)
					{
						rowData[lineIterator] += String(apArray[k].rangeNorm_MIN,2) + ",";
					}

					lineIterator++;
				}

				if (lineHeaders_Main[i] == "mapThresh_AP_Max")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];

					for (int k = 0; k < numCols_inRow[i]; k++)
					{
						rowData[lineIterator] += String(apArray[k].rangeNorm_MAX, 2) + ",";
					}

					lineIterator++;
				}

				if (lineHeaders_Main[i] == "mapThresh_MP_")
				{
					for (int j = 0; j < numRows_inHeader[i]; j++)
					{
						rowHeaders[lineIterator] = lineHeaders_Main[i] + mpArray[j].name;
						rowData[lineIterator] = String(mpArray[j].rangeNorm_MIN, 2) + ","
							+ String(mpArray[j].rangeNorm_MAX, 2);
						lineIterator++;
					}
				}

				if (lineHeaders_Main[i] == "apSmoothing_Fc")
				{
					rowHeaders[lineIterator] = lineHeaders_Main[i];

					for (int k = 0; k < numCols_inRow[i]; k++)
					{
						rowData[lineIterator] += String(apArray[k].freq_Smoothing, 2) + ",";
					}

					lineIterator++;
				}
			}
			
			// SAVE CSV FILE
			String formatSpecifier = "%s,\n";

			for (int i = 0; i < lineIterator; i++)
			{
				fprintf(
					presetFile,
					formatSpecifier.toStdString().c_str(),
					rowHeaders[i] + "," + rowData[i]
				);
			}

			fclose(presetFile);
		}
	}

	short num_Presets = 0;

	void populatePresets(MappingPreset presetArray[], MovementParameter mpArray[], FeedbackVariable apArray[])
	{
		preset_ListLoad.clear();
		preset_ListLoad.addItem("No Preset", 1);
		String presetPath = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
		presetPath = presetPath.upToLastOccurrenceOf("\\", true, false) + "Mapping Presets\\";

		auto dir_Base = File(presetPath);
		num_Presets = dir_Base.getNumberOfChildFiles(2, "*.csv");
		auto presetFiles = dir_Base.findChildFiles(2, false, "*.csv");
		presetFiles.sort();
		File currentFile;

		for (int i = 0; i < num_Presets; i++)						// Load Styles from Defined Directory
		{
			currentFile = presetFiles.getUnchecked(i);
			presetArray[i].name = currentFile.getFileNameWithoutExtension();
			loadPreset_SINGLE(&presetArray[i + 1], currentFile, mpArray, apArray);
			preset_ListLoad.addItem(presetArray[i].name, i + 2);
		}
	}

	void loadPreset_SINGLE(MappingPreset *presetContainer, File currentFile, MovementParameter mpArray[],
							FeedbackVariable apArray[])
	{
		juce::FileInputStream inputStream(currentFile); // [2]
		short apOrder_inFile[40] = {-1,-1,-1,-1,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1,
									-1,-1,-1,-1,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1 };
		short apOrder_writeIdx = 0;

		if (!inputStream.openedOk())
			return;  // failed to open

		while (!inputStream.isExhausted())
		{
			auto line = inputStream.readNextLine();
			auto line_Rem = line;

			String line_header = line.upToFirstOccurrenceOf(",", false, true);
			line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);

			if (line_header == "AP Names")				// ESTABLISH ORDER OF COLUMNS
			{
				String presentAP = "";
				while (line_Rem != "")					// UNTIL LINE STRING EXHAUSTED
				{
					presentAP = line_Rem.upToFirstOccurrenceOf(",", false, true);
					
					for (int a = 0; a < 40; a++)
					{
						if (presentAP == apArray[a].name)
						{
							apOrder_inFile[apOrder_writeIdx] = a;
							apOrder_writeIdx++;
						}
					}

					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
			}

			for (int i = 0; i < num_MP; i++)							// FOR EACH MP
			{
				if (line_header == "isMap_" + mpArray[i].name)			// MAPPING MATRIX BOOL ROWS
				{
					for (int j = 0; j < num_AP; j++)
					{
						if (apOrder_inFile[j] != -1)
						{
							presetContainer->mappingMatrix[i][apOrder_inFile[j]] =
								line_Rem.upToFirstOccurrenceOf(",", false, true) == "1" ? true : false;
							line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
						}
					}
				}

				if (line_header == "mapStr_" + mpArray[i].name)			// MAPPING MATRIX STR ROWS
				{
					for (int j = 0; j < num_AP; j++)
					{
						if (apOrder_inFile[j] != -1)
						{
							presetContainer->mappingStrength[i][apOrder_inFile[j]] =
								line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
							line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
						}
					}
				}

				if (line_header == "mapThresh_MP_" + mpArray[i].name)			
				{
					presetContainer->mappingThresh_MP_Min[i] = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
					presetContainer->mappingThresh_MP_Max[i] = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
				}
			}

			if (line_header == "Map Func Idx")
			{
				for (int j = 0; j < num_AP; j++)
				{
					presetContainer->mapFunc_Idx[j] =
						line_Rem.upToFirstOccurrenceOf(",", false, true).getIntValue();
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
			}

			if (line_header == "Polarity")
			{
				for (int j = 0; j < num_AP; j++)
				{
					presetContainer->polarity[j] =
						line_Rem.upToFirstOccurrenceOf(",", false, true).getIntValue();
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
			}

			if (line_header == "Quant Bits")
			{
				for (int j = 0; j < num_AP; j++)
				{
					presetContainer->num_quantBits[j] =
						line_Rem.upToFirstOccurrenceOf(",", false, true).getIntValue();
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
			}

			if (line_header == "OSC_BPM")
			{
				presetContainer->dataHolder_oscBPM = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			if (line_header == "VoiceCue_ON")
			{
				presetContainer->dataHolder_voiceCue_ON = (line_Rem.upToFirstOccurrenceOf(",", false, true).getIntValue() == 1) ? true : false;
			}

			if (line_header == "VoiceCue_TimingFine")
			{
				presetContainer->dataHolder_voiceCue_timingFine = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			if (line_header == "VoiceCue_LeveldB")
			{
				presetContainer->dataHolder_voiceCue_voldB = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			if (line_header == "VoiceCue_Length")
			{
				presetContainer->dataHolder_voiceCue_Length = line_Rem.upToFirstOccurrenceOf(",", false, true).getIntValue();
			}

			short mat_writeIdx = 0;

			if (line_header == "VoiceCue_isIntervalEnabled")
			{
				while (line_Rem != "")
				{
					presetContainer->dataHolder_voiceCue_isIntervalEnabled[mat_writeIdx] =
						line_Rem.upToFirstOccurrenceOf(",", false, true) == "1" ? true : false;
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
					mat_writeIdx++;
				}
				mat_writeIdx = 0;
			}

			if (line_header == "VoiceCue_isIntervalPos")
			{
				while (line_Rem != "")
				{
					presetContainer->dataHolder_voiceCue_isPos[mat_writeIdx] =
						line_Rem.upToFirstOccurrenceOf(",", false, true) == "1" ? true : false;
					mat_writeIdx++;
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
				mat_writeIdx = 0;
			}

			if (line_header == "VoiceCue_intervalLocation")
			{
				while (line_Rem != "")
				{
					presetContainer->dataHolder_voiceCue_location[mat_writeIdx] =
						line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
					mat_writeIdx++;
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
				mat_writeIdx = 0;
			}

			if (line_header == "mapThresh_AP_Min")
			{
				for (int j = 0; j < num_AP; j++)
				{
					if (apOrder_inFile[j] != -1)
					{
						presetContainer->mappingThresh_AP_Min[apOrder_inFile[j]] =
							line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
						line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
					}
				}
			}

			if (line_header == "mapThresh_AP_Max")
			{
				for (int j = 0; j < num_AP; j++)
				{
					if (apOrder_inFile[j] != -1)
					{
						presetContainer->mappingThresh_AP_Max[apOrder_inFile[j]] =
							line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
						line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
					}
				}
			}

			if (line_header == "apSmoothing_Fc")
			{
				for (int j = 0; j < num_AP; j++)
				{
					if (apOrder_inFile[j] != -1)
					{
						presetContainer->apSmoothing_Fc[apOrder_inFile[j]] =
							line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
						line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
					}
				}
			}
		}
	}

	void loadPreset(MappingPreset *loadedPreset)
	{
		for (int i = 0; i < num_AP; i++)
		{
			normRange_AP[i].setMinValue(loadedPreset->mappingThresh_AP_Min[i]);
			normRange_AP[i].setMaxValue(loadedPreset->mappingThresh_AP_Max[i]);
			apSmoothing_Fc[i].setValue(loadedPreset->apSmoothing_Fc[i]);

			for (int j = 0; j < num_MP; j++)
			{
				mapping_Matrix[j][i].setToggleState(loadedPreset->mappingMatrix[j][i],sendNotificationSync);
				mapping_Strength[j][i].setValue(loadedPreset->mappingStrength[j][i]);
			}
			mapping_Function[i].setSelectedId(loadedPreset->mapFunc_Idx[i]);
			mapping_Polarity[i].setSelectedId(loadedPreset->polarity[i]);
			mapping_QuantLevels[i].setSelectedId(loadedPreset->num_quantBits[i] + 1);
		}

		for (int k = 0; k < num_MP; k++)
		{
			normRange_MP[k].setMinValue(loadedPreset->mappingThresh_MP_Min[k]);
			normRange_MP[k].setMaxValue(loadedPreset->mappingThresh_MP_Max[k]);
		}
	}
    
    void toggleVisible(bool on, MovementParameter mpArray[], FeedbackVariable apArray[])
    {
		for (int m = 0; m < 40; m++)
		{
			if (mpArray[m].isVisible)
			{
				labels_movementParams[m].setVisible(on);
				movementParams_Value[m][0].setVisible(on);
				movementParams_Value[m][1].setVisible(on);
				normRange_MP[m].setVisible(on);
			}
		}

		for (int a = 0; a < 40; a++)
		{
			if (apArray[a].isVisible)
			{
				labels_audioParams[a].setVisible(on);
				mapping_Function[a].setVisible(on);
				mapping_Polarity[a].setVisible(on);
				mapping_QuantLevels[a].setVisible(on);
				audioParams_Value[a][0].setVisible(on);
				audioParams_Value[a][1].setVisible(on);
				normRange_AP[a].setVisible(on);
				apSmoothing_Fc[a].setVisible(on);
			}
		}

		for (int m = 0; m < 40; m++)
		{
			for (int a = 0; a < 40; a++)
			{
				if (mpArray[m].isVisible && apArray[a].isVisible)
				{
					mapping_Matrix[m][a].setVisible(on);
					mapping_Strength[m][a].setVisible(on && mapping_Matrix[m][a].getToggleState());
				}
			}
		}

		preset_Save.setVisible(on);
		preset_ListLoad.setVisible(on);
		preset_Name.setVisible(on);

		mp_isVisible.setVisible(on);
		mp_isHidden.setVisible(on);
		ap_isVisible.setVisible(on);
		ap_isHidden.setVisible(on);
		triOsc_BPM.setVisible(on);
    }
    
	int dispSeq_MP[40], num_MP_Visible = 0, dispPos_MP_present = 0;
	int dispSeq_AP[40], num_AP_Visible = 0, dispPos_AP_present = 0;

    void setLayout(int interfaceWidth, int interfaceHeight, int numMP, int numAP,
	MovementParameter mpArray[], FeedbackVariable fbVars[])
    {
		// INITIAL DECLARATION AND ASSIGNMENTS
		num_MP = numMP;		num_AP = numAP;

		num_MP_Visible = 0;
		num_AP_Visible = 0;
		dispPos_MP_present = 0;
		dispPos_AP_present = 0;

		// FIND NUMBER OF VISIBLE PARAMETERS, DISPLAY POSITIONS
		for (int i = 0; i < numMP; i++) 
		{ 
			if (mpArray[i].isVisible)
			{
				mp_isVisible.addItem(mpArray[i].name, i + 1);
				num_MP_Visible += 1;
			}
			else
			{
				mp_isHidden.addItem(mpArray[i].name, i + 1);
			}
		}
		
		for (int i = 0; i < numAP; i++) 
		{ 
			if (fbVars[i].isVisible)
			{
				ap_isVisible.addItem(fbVars[i].name, i + 1);
				num_AP_Visible += 1;
			}
			else
			{
				ap_isHidden.addItem(fbVars[i].name, i + 1);
			}
		}

		int numDispGrp_MP = 15;
		int numDispGrp_AP = 15;

		// CREATE DISPLAY SEQUENCE OF MPS
		// ITERATE OVER DISPLAY POSITION GRP (UNIQUE)
		for (int dg_mp = 0; dg_mp < numDispGrp_MP; dg_mp++)		
		{
			// ITERATE OVER MPs
			for (int m = 0; m < num_MP; m++)					
			{
				// CHECK IF DISPLAY POSITION GRP MATCHES MP DISP POS GRP
				if (mpArray[m].isVisible && mpArray[m].dispIdx == dg_mp)	
				{
					dispSeq_MP[dispPos_MP_present] = m;
					dispPos_MP_present++;
				}
			}
		}

		// CREATE DISPLAY SEQUENCE OF APS
		// ITERATE OVER DISPLAY POSITION GRP (UNIQUE)
		for (int dg_ap = 0; dg_ap < numDispGrp_AP; dg_ap++)
		{
			// ITERATE OVER APs
			for (int a = 0; a < num_MP; a++)
			{
				// CHECK IF DISPLAY POSITION GRP MATCHES MP DISP POS GRP
				if (fbVars[a].isVisible && fbVars[a].dispIdx == dg_ap)
				{
					dispSeq_AP[dispPos_AP_present] = a;
					dispPos_AP_present++;
				}
			}
		}

		mp_isVisible.setBounds(interfaceWidth / 2 - 300, 0, 150, 30);
		mp_isHidden.setBounds(interfaceWidth / 2 - 130, 0, 150, 30);
		ap_isVisible.setBounds(interfaceWidth / 2 + 40, 0, 150, 30);
		ap_isHidden.setBounds(interfaceWidth / 2 + 210, 0, 150, 30);

		int matrix_Width = 0.75 * interfaceWidth;
		int matrix_Height = 0.65 * interfaceHeight;

		int matrix_startPointX = (interfaceWidth - matrix_Width) / 2 + 100;
		int matrix_startPointY = (interfaceHeight - matrix_Height) / 2 - 70;
		float mp_startPointX = matrix_startPointX - 110 - width_Value;

		int gap_interRow = matrix_Height / num_MP_Visible;
		int gap_interCol = matrix_Width / num_AP_Visible;

		width_Value_AP = 50.0 / 63.0 * gap_interCol;

		int num_MP_populated = 0;
		int num_AP_populated = 0;

		for (int i = 0; i < num_MP_Visible; i++)
		{
			if (mpArray[dispSeq_MP[i]].isVisible)
			{
				labels_movementParams[dispSeq_MP[i]].setBounds(
					matrix_startPointX - 110,
					matrix_startPointY + gap_interRow * num_MP_populated,
					110, 25);

				if (mpArray[dispSeq_MP[i]].name == "Tri Osc")
				{
					triOsc_BPM.setBounds(
						matrix_startPointX - 110 - width_Value - gap_interRow - 10,
						matrix_startPointY + gap_interRow * num_MP_populated - 8,
						50, 40);
				}

				normRange_MP[dispSeq_MP[i]].setBounds(
					mp_startPointX - 3,
					matrix_startPointY + gap_interRow * num_MP_populated + 20,
					width_Value + 7,
					10
				);

				for (int j = 0; j < num_AP_Visible; j++)
				{
					if (fbVars[dispSeq_AP[j]].isVisible)
					{
						mapping_Matrix[dispSeq_MP[i]][dispSeq_AP[j]].setBounds(
							matrix_startPointX + gap_interCol * num_AP_populated,
							matrix_startPointY + gap_interRow * num_MP_populated,
							25,
							25
						);

						mapping_Strength[dispSeq_MP[i]][dispSeq_AP[j]].setBounds(
							matrix_startPointX + gap_interCol * num_AP_populated + 20,
							matrix_startPointY + gap_interRow * num_MP_populated - 7,
							50,
							40
						);

						num_AP_populated += 1;
						num_AP_populated %= num_AP_Visible;
					}
				}
				num_MP_populated = (num_MP_populated + 1) % num_MP_Visible;
			}
		}

		num_AP_populated = 0;

		for (int k = 0; k < num_AP_Visible; k++)
		{
			if (fbVars[dispSeq_AP[k]].isVisible)
			{
				mapping_Function[dispSeq_AP[k]].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 20,
					gap_interCol * 0.95,
					25
				);

				mapping_Polarity[dispSeq_AP[k]].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 60,
					gap_interCol * 0.95,
					25
				);

				mapping_QuantLevels[dispSeq_AP[k]].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 100,
					gap_interCol * 0.95,
					25
				);

				labels_audioParams[dispSeq_AP[k]].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 125,
					gap_interCol * 0.95,
					25
				);

				normRange_AP[dispSeq_AP[k]].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 175,
					width_Value_AP * 1.2,
					10
				);

				apSmoothing_Fc[dispSeq_AP[k]].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 5,
					width_Value_AP * 1.2,
					10
				);
				////////////////////////////////////////////////////////////////////////////////////////////////
				// ADD PARAM SMOOTHING SLIDER

				num_AP_populated += 1;
			}
		}

		preset_Save.setBounds(
			matrix_startPointX - 110 - width_Value,
			matrix_startPointY + num_MP_Visible * gap_interRow + 20,
			160,25
		);

		preset_Name.setBounds(
			matrix_startPointX - 110 - width_Value,
			matrix_startPointY + num_MP_Visible * gap_interRow + 60,
			160, 25
		);

		preset_ListLoad.setBounds(
			matrix_startPointX - 110 - width_Value,
			matrix_startPointY + num_MP_Visible * gap_interRow + 100,
			160, 25
		);
    }

	void updateValueVisualizers(int interfaceWidth, int interfaceHeight
		, int numMP, int numAP, MovementParameter mpArray[], FeedbackVariable fbVars[])
	{
		int num_MP_Visible = 0;
		int num_AP_Visible = 0;
		int num_MP_populated = 0;
		int num_AP_populated = 0;

		for (int i = 0; i < numMP; i++) { 
			if (mpArray[i].isVisible)
			{
				num_MP_Visible += 1;
				movementParams_Value[i][0].setText(String(mpArray[i].value, 1), dontSendNotification);
			}
		}
		for (int i = 0; i < numAP; i++) { if (fbVars[i].isVisible) num_AP_Visible += 1; }

		int matrix_Width = 0.75 * interfaceWidth;
		int matrix_Height = 0.65 * interfaceHeight;

		int matrix_startPointX = (interfaceWidth - matrix_Width) / 2 + 100;
		int matrix_startPointY = (interfaceHeight - matrix_Height) / 2 - 70;

		int gap_interRow = matrix_Height / num_MP_Visible;
		int gap_interCol = matrix_Width / num_AP_Visible;

		float width_Lab1 = 0;
		float width_Lab2 = 0;
		float mp_startPointX = matrix_startPointX - 110 - width_Value;
		float ap_startPointY = matrix_startPointY + numMP * gap_interRow + 100 + 30;

		for (int i = 0; i < num_MP_Visible; i++)
		{
			if (mpArray[dispSeq_MP[i]].isVisible)
			{
				width_Lab1 = (mpArray[dispSeq_MP[i]].value - mpArray[dispSeq_MP[i]].minVal) 
							/ (mpArray[dispSeq_MP[i]].maxVal - mpArray[dispSeq_MP[i]].minVal) * width_Value;
				width_Lab2 = width_Value - width_Lab1;

				Colour colour_mpVal;
				bool colorCondition = mpArray[dispSeq_MP[i]].inRange;
				colour_mpVal = colorCondition ? Colours::yellow : Colours::orange;
				movementParams_Value[dispSeq_MP[i]][0].setColour(movementParams_Value[dispSeq_MP[i]][0].backgroundColourId, colour_mpVal);

				movementParams_Value[dispSeq_MP[i]][0].setBounds(
					mp_startPointX,
					matrix_startPointY + gap_interRow * num_MP_populated,
					width_Lab1,
					20
				);

				movementParams_Value[dispSeq_MP[i]][1].setBounds(
					mp_startPointX + width_Lab1,
					matrix_startPointY + gap_interRow * num_MP_populated,
					width_Lab2,
					20
				);

				num_MP_populated = (num_MP_populated + 1) % num_MP_Visible;
			}
		}

		for (int i = 0; i < num_AP_Visible; i++)
		{
			if (fbVars[dispSeq_AP[i]].isVisible)
			{
				width_Lab1 = (fbVars[dispSeq_AP[i]].value - fbVars[dispSeq_AP[i]].minVal) 
					/ (fbVars[dispSeq_AP[i]].maxVal - fbVars[dispSeq_AP[i]].minVal) * width_Value_AP;
				width_Lab2 = width_Value_AP - width_Lab1;
				width_Lab2 += (int)(width_Value_AP - width_Lab1 - width_Lab2);

				audioParams_Value[dispSeq_AP[i]][0].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3 + 5,
					matrix_startPointY + num_MP_Visible * gap_interRow + 150,
					width_Lab1,
					20
				);

				audioParams_Value[dispSeq_AP[i]][1].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3 + width_Lab1 + 5,
					matrix_startPointY + num_MP_Visible * gap_interRow + 150,
					width_Lab2,
					20
				);

				num_AP_populated = (num_AP_populated + 1) % num_AP_Visible;
			}
		}
	}
};