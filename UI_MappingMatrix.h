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
	ComboBox mapping_Function[40];
	ComboBox mapping_Polarity[40];
	ComboBox mapping_QuantLevels[40];
	Label movementParams_Value[40][2];		// 0 = Filled // 1 = Unfilled
	Label audioParams_Value[40][2];
	float width_Value = 90;
	float width_Value_AP = 50;
	int num_MP = 0;
	int num_AP = 0;

	// PRESETS
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
		}

		preset_Name.setJustification(Justification::centred);
		preset_Save.setButtonText("Save as Preset");
    }

	void saveAsPreset(MovementParameter mpArray[], FeedbackVariable apArray[])
	{
		if (preset_Name.getText() == "")
			return;
		else
		{
			String presetFile_Path = "";
			// INITIALIZE PATHS, STORE INFO
			presetFile_Path = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
			presetFile_Path = presetFile_Path.upToLastOccurrenceOf("\\", true, false);
			presetFile_Path += "Mapping Presets\\" + preset_Name.getText() + ".csv";
			
			presetFile = fopen(presetFile_Path.toStdString().c_str(), "w");

			// SAVE CSV FILE
			String formatSpecifier = "%s,\n";

			// SPECIFY TOTAL LINES
			int numLines = 1 + num_MP + num_MP + num_MP + 3 + 8;

			String lineString = "";
			String lineHeader = "";

			for (int l = 0; l < numLines; l++)											// FULL FILE
			{
				lineString = "";
				for (int a = 0; a < num_AP; a++)										// EACH LINE
				{
					if (l == 0)															// FIRST LINE - AP NAMES
					{
						lineHeader = "AP Names";
						lineString += apArray[a].name + ",";
					}

					if (l >= 1 && l < num_MP + 1)										// MAPPING MATRIX BOOLEANS
					{
						lineHeader = "isMap_" + mpArray[l - 1].name;
						lineString += mapping_Matrix[l - 1][a].getToggleState() ? "1," : "0,";
					}

					if (l >= num_MP + 1 && l < num_MP + 1 + num_MP)						// MAPPING MATRIX STRENGTH
					{
						lineHeader = "mapStr_" + mpArray[l - num_MP - 1].name;
						lineString += String(mapping_Strength[l - num_MP - 1][a].getValue(), 2) + ",";
					}

					if (l >= num_MP + 1 + num_MP && l < num_MP + num_MP + num_MP + 1)	// MAPPING THRESH
					{
						if (a == 0)
						{
							lineHeader = "mapThr_" + mpArray[l - 2 * num_MP - 1].name;
							lineString += String(normRange_MP[l - 2 * num_MP - 1].getValue(), 2) + ",";
						}
					}

					if (l == 3 * num_MP + 1)											// MAPPING FUNCTION IDX
					{
						lineHeader = "Map Func Idx";
						lineString += String(mapping_Function[a].getSelectedId()) + ",";
					}

					if (l == 3 * num_MP + 2)											// MAPPING POLARITY
					{
						lineHeader = "Polarity";
						lineString += String(mapping_Polarity[a].getSelectedId()) + ",";
					}

					if (l == 3 * num_MP + 3)											// MAPPING QUANT BITS
					{
						lineHeader = "Quant Bits";
						lineString += String(mapping_QuantLevels[a].getSelectedId() - 1) + ",";
					}

					if (a == 0)
					{
						if (l == 3 * num_MP + 4)										// OSC BPM
						{
							lineHeader = "OSC_BPM";
							lineString = String(dataHolder_oscBPM, 4);
						}

						if (l == 3 * num_MP + 5)										// VOICE CUE ON
						{
							lineHeader = "VoiceCue_ON";
							lineString = dataHolder_voiceCue_ON ? "1" : "0";
						}

						if (l == 3 * num_MP + 6)										// TIMING FINE
						{
							lineHeader = "VoiceCue_TimingFine";
							lineString = String(dataHolder_voiceCue_timingFine,2);
						}

						if (l == 3 * num_MP + 7)										// VOICE CUE LEVEL
						{
							lineHeader = "VoiceCue_LeveldB";
							lineString = String(dataHolder_voiceCue_voldB,2);
						}

						if (l == 3 * num_MP + 8)										// VOICE CUE LENGTH
						{
							lineHeader = "VoiceCue_Length";
							lineString = String(dataHolder_voiceCue_Length);
						}
					}

					if (a < 3)
					{
						if (l == 3 * num_MP + 9)										// IS INTERVAL ENABLED
						{
							lineHeader = "VoiceCue_isIntervalEnabled";
							lineString += dataHolder_voiceCue_isIntervalEnabled[a] ? "1," : "0,";
						}

						if (l == 3 * num_MP + 10)										// IS INTERVAL POSITIVE CROSS
						{
							lineHeader = "VoiceCue_isIntervalPos";
							lineString += dataHolder_voiceCue_isPos[a] ? "1," : "0,";
						}

						if (l == 3 * num_MP + 11)										// CUE LOCATION
						{
							lineHeader = "VoiceCue_intervalLocation";
							lineString += String(dataHolder_voiceCue_location[a],2) + ",";
						}
					}
				}

				fprintf(
					presetFile,
					formatSpecifier.toStdString().c_str(),
					lineHeader + "," + lineString
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
		short apOrder_inFile[20] = {-1,-1,-1,-1,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1};
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
					
					for (int a = 0; a < 20; a++)
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

				if (line_header == "mapThr_" + mpArray[i].name)			// MAPPING MATRIX THR ROWS
				{
					for (int j = 0; j < num_AP; j++)
					{
							presetContainer->mappingThresh[i] =
								line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
					}
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
		}
	}

	void loadPreset(MappingPreset *loadedPreset)
	{
		for (int i = 0; i < num_AP; i++)
		{
			for (int j = 0; j < num_MP; j++)
			{
				mapping_Matrix[j][i].setToggleState(loadedPreset->mappingMatrix[j][i],sendNotificationSync);
				mapping_Strength[j][i].setValue(loadedPreset->mappingStrength[j][i]);
				//normRange_MP[j].setValue(loadedPreset->mappingThresh[j]);
			}
			mapping_Function[i].setSelectedId(loadedPreset->mapFunc_Idx[i]);
			mapping_Polarity[i].setSelectedId(loadedPreset->polarity[i]);
			mapping_QuantLevels[i].setSelectedId(loadedPreset->num_quantBits[i] + 1);
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
    }
    
	int dispSeq_MP[40], num_MP_Visible = 0, dispPos_MP_present = 0;
	int dispSeq_AP[40], num_AP_Visible = 0, dispPos_AP_present = 0;

    void setLayout(int interfaceWidth, int interfaceHeight, int numMP, int numAP,
	MovementParameter mpArray[], FeedbackVariable fbVars[])
    {
		// INITIAL DECLARATION AND ASSIGNMENTS
		num_MP = numMP;		num_AP = numAP;

		// FIND NUMBER OF VISIBLE PARAMETERS, DISPLAY POSITIONS
		for (int i = 0; i < numMP; i++) { if (mpArray[i].isVisible) num_MP_Visible += 1; }
		for (int i = 0; i < numAP; i++) { if (fbVars[i].isVisible)  num_AP_Visible += 1; }

		int numDispGrp_MP = 10;
		int numDispGrp_AP = 10;

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

		int matrix_Width = 0.75 * interfaceWidth;
		int matrix_Height = 0.65 * interfaceHeight;

		int matrix_startPointX = (interfaceWidth - matrix_Width) / 2 + 100;
		int matrix_startPointY = (interfaceHeight - matrix_Height) / 2 - 70;
		float mp_startPointX = matrix_startPointX - 110 - width_Value;

		int gap_interRow = matrix_Height / num_MP_Visible;
		int gap_interCol = matrix_Width / num_AP_Visible;

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
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3 + 5,
					matrix_startPointY + num_MP_Visible * gap_interRow + 175,
					width_Value_AP * 1.2,
					10
				);

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

		for (int i = 0; i < numMP; i++) { if (mpArray[i].isVisible) num_MP_Visible += 1; }
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
				bool colorCondition = mpArray[dispSeq_MP[i]].value > (mpArray[dispSeq_MP[i]].minVal
					+ mpArray[dispSeq_MP[i]].thresh_min_NORM * (mpArray[dispSeq_MP[i]].maxVal - mpArray[dispSeq_MP[i]].minVal));
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

				audioParams_Value[dispSeq_AP[i]][0].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3 + 10,
					matrix_startPointY + num_MP_Visible * gap_interRow + 150,
					width_Lab1,
					20
				);

				audioParams_Value[dispSeq_AP[i]][1].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3 + width_Lab1 + 10,
					matrix_startPointY + num_MP_Visible * gap_interRow + 150,
					width_Lab2,
					20
				);

				num_AP_populated = (num_AP_populated + 1) % num_AP_Visible;
			}
		}
	}
};