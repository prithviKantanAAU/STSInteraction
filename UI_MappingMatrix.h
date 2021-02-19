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

	Label labels_movementParams[20];
	Label labels_audioParams[20];
	ToggleButton mapping_Matrix[20][20];
	Slider mapping_Strength[20][20];
	Slider mp_minThresh[20];
	ComboBox mapping_Function[20];
	ComboBox mapping_Polarity[20];
	ComboBox mapping_QuantLevels[20];
	Label movementParams_Value[20][2];		// 0 = Filled // 1 = Unfilled
	Label audioParams_Value[20][2];
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
    
    void configure()
    {
		for (int i = 0; i < 20; i++)
		{
			movementParams_Value[i][0].setColour(movementParams_Value[i][0].backgroundColourId, Colours::yellow);
			movementParams_Value[i][1].setColour(movementParams_Value[i][1].backgroundColourId, Colours::blue);
			audioParams_Value[i][0].setColour(audioParams_Value[i][0].backgroundColourId, Colours::yellow);
			audioParams_Value[i][1].setColour(audioParams_Value[i][1].backgroundColourId, Colours::blue);
			labels_movementParams[i].setJustificationType(Justification::centred);
			labels_audioParams[i].setJustificationType(Justification::centred);

			mp_minThresh[i].setRange(0, 1);
			mp_minThresh[i].setValue(0);
			mp_minThresh[i].setColour(mp_minThresh[i].trackColourId, Colours::yellow);
			mp_minThresh[i].setColour(mp_minThresh[i].backgroundColourId, Colours::blue);
			mp_minThresh[i].setTextBoxStyle(Slider::NoTextBox,false,10,10);
		}

		preset_Name.setJustification(Justification::centred);
		preset_Save.setButtonText("Save as Preset");
    }

	void populatePresets(int numPresets, MappingPreset presetArray[])
	{

	}

	void saveAsPreset()
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
			//String formatSpecifier = String::repeatedString("%s,", num_AP + 1) + "\n";
			String formatSpecifier = "%s,\n";

			int numLines = num_MP + 3 + num_MP;
			String lineString = "";
			String lineHeader = "";

			for (int l = 0; l < numLines; l++)
			{
				lineString = "";
				for (int a = 0; a < num_AP; a++)
				{
					if (l < num_MP)
					{
						lineHeader = "MP Row " + String(l + 1);
						lineString += mapping_Matrix[l][a].getToggleState() ? "1," : "0,";
					}

					if (l == num_MP)
					{
						lineHeader = "Map Func Idx";
						lineString += String(mapping_Function[a].getSelectedId()) + ",";
					}

					if (l == num_MP + 1)
					{
						lineHeader = "Polarity";
						lineString += String(mapping_Polarity[a].getSelectedId()) + ",";
					}

					if (l == num_MP + 2)
					{
						lineHeader = "Quant Bits";
						lineString += String(mapping_QuantLevels[a].getSelectedId() - 1) + ",";
					}

					if (l > num_MP + 2)
					{
						lineHeader = "MP Str Row " + String(l - num_MP - 2 - 1 + 1);
						lineString += String(mapping_Strength[l - num_MP - 2 - 1][a].getValue(),2) + ",";
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

	void populatePresets(MappingPreset presetArray[])
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
			loadPreset_SINGLE(&presetArray[i + 1], currentFile);
			preset_ListLoad.addItem(presetArray[i].name, i + 2);
		}
	}

	void loadPreset_SINGLE(MappingPreset *presetContainer, File currentFile)
	{
		juce::FileInputStream inputStream(currentFile); // [2]

		if (!inputStream.openedOk())
			return;  // failed to open

		while (!inputStream.isExhausted())
		{
			auto line = inputStream.readNextLine();
			auto line_Rem = line;

			String line_header = line.upToFirstOccurrenceOf(",", false, true);
			line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);

			for (int i = 0; i < num_MP; i++)
			{
				if (line_header == "MP Row " + String(i + 1))
				{
					for (int j = 0; j < num_AP; j++)
					{
						presetContainer->mappingMatrix[i][j] =
							line_Rem.upToFirstOccurrenceOf(",", false, true) == "1" ? true : false;
						line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
					}
				}

				if (line_header == "MP Str Row " + String(i + 1))
				{
					for (int j = 0; j < num_AP; j++)
					{
						presetContainer->mappingStrength[i][j] =
							line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
						line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
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
			}
			mapping_Function[i].setSelectedId(loadedPreset->mapFunc_Idx[i]);
			mapping_Polarity[i].setSelectedId(loadedPreset->polarity[i]);
			mapping_QuantLevels[i].setSelectedId(loadedPreset->num_quantBits[i] + 1);
		}
	}
    
    void toggleVisible(bool on)
    {
		for (int i = 0; i < 20; i++)		// Columns
		{
			labels_movementParams[i].setVisible(on);
			labels_audioParams[i].setVisible(on);
			mapping_Function[i].setVisible(on);
			mapping_Polarity[i].setVisible(on);
			mapping_QuantLevels[i].setVisible(on);
			movementParams_Value[i][0].setVisible(on);
			movementParams_Value[i][1].setVisible(on);
			audioParams_Value[i][0].setVisible(on);
			audioParams_Value[i][1].setVisible(on);
			for (int j = 0; j < 20; j++)	// Rows
			{
				mapping_Matrix[i][j].setVisible(on);
				mapping_Strength[i][j].setVisible(on && mapping_Matrix[i][j].getToggleState());
				mp_minThresh[i].setVisible(on);
			}
		}

		preset_Save.setVisible(on);
		preset_ListLoad.setVisible(on);
		preset_Name.setVisible(on);
    }
    
    void setLayout(int interfaceWidth, int interfaceHeight, int numMP, int numAP,
	MovementParameter mpArray[], FeedbackVariable fbVars[])
    {
		num_MP = numMP;
		num_AP = numAP;

		int num_MP_Visible = 0;
		int num_AP_Visible = 0;

		for (int i = 0; i < numMP; i++) { if (mpArray[i].isVisible) num_MP_Visible += 1; }
		for (int i = 0; i < numAP; i++) { if (fbVars[i].isVisible) num_AP_Visible += 1; }

		int matrix_Width = 0.75 * interfaceWidth;
		int matrix_Height = 0.5 * interfaceHeight;

		int matrix_startPointX = (interfaceWidth - matrix_Width) / 2 + 100;
		int matrix_startPointY = (interfaceHeight - matrix_Height) / 2 - 70;
		float mp_startPointX = matrix_startPointX - 110 - width_Value;

		int gap_interRow = matrix_Height / num_MP_Visible;
		int gap_interCol = matrix_Width / num_AP_Visible;

		int num_MP_populated = 0;
		int num_AP_populated = 0;

		for (int i = 0; i < numMP; i++)
		{
			if (mpArray[i].isVisible)
			{
				labels_movementParams[i].setBounds(
					matrix_startPointX - 110,
					matrix_startPointY + gap_interRow * num_MP_populated,
					110, 25);

				mp_minThresh[i].setBounds(
					mp_startPointX - 3,
					matrix_startPointY + gap_interRow * num_MP_populated + 20,
					width_Value + 7,
					10
				);

				for (int j = 0; j < numAP; j++)
				{
					if (fbVars[j].isVisible)
					{
						mapping_Matrix[i][j].setBounds(
							matrix_startPointX + gap_interCol * num_AP_populated,
							matrix_startPointY + gap_interRow * num_MP_populated,
							25,
							25
						);

						mapping_Strength[i][j].setBounds(
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

		for (int k = 0; k < numAP; k++)
		{
			if (fbVars[k].isVisible)
			{
				mapping_Function[k].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 20,
					gap_interCol * 0.95,
					25
				);

				mapping_Polarity[k].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 60,
					gap_interCol * 0.95,
					25
				);

				mapping_QuantLevels[k].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 100,
					gap_interCol * 0.95,
					25
				);

				labels_audioParams[k].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3,
					matrix_startPointY + num_MP_Visible * gap_interRow + 125,
					gap_interCol * 0.95,
					25
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
		int matrix_Height = 0.5 * interfaceHeight;

		int matrix_startPointX = (interfaceWidth - matrix_Width) / 2 + 100;
		int matrix_startPointY = (interfaceHeight - matrix_Height) / 2 - 70;

		int gap_interRow = matrix_Height / num_MP_Visible;
		int gap_interCol = matrix_Width / num_AP_Visible;

		float width_Lab1 = 0;
		float width_Lab2 = 0;
		float mp_startPointX = matrix_startPointX - 110 - width_Value;
		float ap_startPointY = matrix_startPointY + numMP * gap_interRow + 100 + 30;

		for (int i = 0; i < numMP; i++)
		{
			if (mpArray[i].isVisible)
			{
				width_Lab1 = (mpArray[i].value - mpArray[i].minVal) / (mpArray[i].maxVal - mpArray[i].minVal) * width_Value;
				width_Lab2 = width_Value - width_Lab1;

				Colour colour_mpVal;
				colour_mpVal = (mpArray[i].value > mpArray[i].thresh_min_NORM) ? Colours::yellow : Colours::orange;
				movementParams_Value[i][0].setColour(movementParams_Value[i][0].backgroundColourId, colour_mpVal);

				movementParams_Value[i][0].setBounds(
					mp_startPointX,
					matrix_startPointY + gap_interRow * num_MP_populated,
					width_Lab1,
					20
				);

				movementParams_Value[i][1].setBounds(
					mp_startPointX + width_Lab1,
					matrix_startPointY + gap_interRow * num_MP_populated,
					width_Lab2,
					20
				);

				num_MP_populated = (num_MP_populated + 1) % num_MP_Visible;
			}
		}

		for (int i = 0; i < numAP; i++)
		{
			if (fbVars[i].isVisible)
			{
				width_Lab1 = (fbVars[i].value - fbVars[i].minVal) / (fbVars[i].maxVal - fbVars[i].minVal) * width_Value_AP;
				width_Lab2 = width_Value_AP - width_Lab1;

				audioParams_Value[i][0].setBounds(
					matrix_startPointX + gap_interCol * num_AP_populated - gap_interCol * 0.3 + 10,
					matrix_startPointY + num_MP_Visible * gap_interRow + 150,
					width_Lab1,
					20
				);

				audioParams_Value[i][1].setBounds(
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