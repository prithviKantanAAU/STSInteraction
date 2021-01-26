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
	ComboBox mapping_Function[20];
	ComboBox mapping_Polarity[20];
	ComboBox mapping_QuantLevels[20];
	Label movementParams_Value[20][2];		// 0 = Filled // 1 = Unfilled
	Label audioParams_Value[20][2];
	float width_Value = 90;
	float width_Value_AP = 50;

	// PRESETS
	TextButton preset_Save;
	ComboBox preset_ListLoad;
	Label  preset_ListLoad_LAB;
	TextEditor preset_Name;
    
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
		}

		preset_Name.setJustification(Justification::centred);
		preset_Save.setButtonText("Save as Preset");
		preset_Save.onClick = [this]
		{
			saveAsPreset();
		};

		preset_ListLoad.addItem("No Preset", 1);
		preset_ListLoad.setSelectedId(1);
    }

	void saveAsPreset()
	{
		if (preset_Name.getText() == "")
			return;
		else
		{
			// SAVE CSV FILE
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
			}
		}

		preset_Save.setVisible(on);
		preset_ListLoad.setVisible(on);
		preset_Name.setVisible(on);
    }
    
    void setLayout(int interfaceWidth, int interfaceHeight, int numMP, int numAP)
    {
		int matrix_Width = 0.75 * interfaceWidth;
		int matrix_Height = 0.5 * interfaceHeight;

		int matrix_startPointX = (interfaceWidth - matrix_Width) / 2 + 100;
		int matrix_startPointY = (interfaceHeight - matrix_Height) / 2 - 70;

		int gap_interRow = matrix_Height / numMP;
		int gap_interCol = matrix_Width / numAP;

		for (int i = 0; i < numMP; i++)
		{
			labels_movementParams[i].setBounds(
				matrix_startPointX - 110, 
				matrix_startPointY + gap_interRow * i,
				110,25);

			for (int j = 0; j < numAP; j++)
			{
				mapping_Matrix[i][j].setBounds(
					matrix_startPointX + gap_interCol * j,
					matrix_startPointY + gap_interRow * i,
					25,
					25
				);
			}
		}

		for (int k = 0; k < numAP; k++)
		{
			mapping_Function[k].setBounds(
				matrix_startPointX + gap_interCol * k - gap_interCol * 0.3,
				matrix_startPointY + numMP * gap_interRow + 20,
				gap_interCol * 0.95,
				25
			);

			mapping_Polarity[k].setBounds(
				matrix_startPointX + gap_interCol * k - gap_interCol * 0.3,
				matrix_startPointY + numMP * gap_interRow + 60,
				gap_interCol * 0.95,
				25
			);

			mapping_QuantLevels[k].setBounds(
				matrix_startPointX + gap_interCol * k - gap_interCol * 0.3,
				matrix_startPointY + numMP * gap_interRow + 100,
				gap_interCol * 0.95,
				25
			);

			labels_audioParams[k].setBounds(
				matrix_startPointX + gap_interCol * k - gap_interCol * 0.3,
				matrix_startPointY + numMP * gap_interRow + 125,
				gap_interCol * 0.95,
				25
			);
		}

		preset_Save.setBounds(
			matrix_startPointX - 110 - width_Value,
			matrix_startPointY + numMP * gap_interRow + 20,
			160,25
		);

		preset_Name.setBounds(
			matrix_startPointX - 110 - width_Value,
			matrix_startPointY + numMP * gap_interRow + 60,
			160, 25
		);

		preset_ListLoad.setBounds(
			matrix_startPointX - 110 - width_Value,
			matrix_startPointY + numMP * gap_interRow + 100,
			160, 25
		);
    }

	void updateValueVisualizers(int interfaceWidth, int interfaceHeight
		, int numMP, int numAP, MovementParameter mpArray[], FeedbackVariable fbVars[])
	{
		int matrix_Width = 0.75 * interfaceWidth;
		int matrix_Height = 0.5 * interfaceHeight;

		int matrix_startPointX = (interfaceWidth - matrix_Width) / 2 + 100;
		int matrix_startPointY = (interfaceHeight - matrix_Height) / 2 - 70;

		int gap_interRow = matrix_Height / numMP;
		int gap_interCol = matrix_Width / numAP;

		float width_Lab1 = 0;
		float width_Lab2 = 0;
		float mp_startPointX = matrix_startPointX - 110 - width_Value;
		float ap_startPointY = matrix_startPointY + numMP * gap_interRow + 100 + 30;

		for (int i = 0; i < numMP; i++)
		{
			width_Lab1 = (mpArray[i].value - mpArray[i].minVal) / (mpArray[i].maxVal - mpArray[i].minVal) * width_Value;
			width_Lab2 = width_Value - width_Lab1;

			movementParams_Value[i][0].setBounds(
				mp_startPointX,
				matrix_startPointY + gap_interRow * i,
				width_Lab1,
				20
			);

			movementParams_Value[i][1].setBounds(
				mp_startPointX + width_Lab1,
				matrix_startPointY + gap_interRow * i,
				width_Lab2,
				20
			);
		}

		for (int i = 0; i < numAP; i++)
		{
			width_Lab1 = (fbVars[i].value - fbVars[i].minVal) / (fbVars[i].maxVal - fbVars[i].minVal) * width_Value_AP;
			width_Lab2 = width_Value_AP - width_Lab1;

			audioParams_Value[i][0].setBounds(
				matrix_startPointX + gap_interCol * i - gap_interCol * 0.3 + 8,
				matrix_startPointY + numMP * gap_interRow + 150,
				width_Lab1,
				20
			);

			audioParams_Value[i][1].setBounds(
				matrix_startPointX + gap_interCol * i - gap_interCol * 0.3 + width_Lab1 + 8,
				matrix_startPointY + numMP * gap_interRow + 150,
				width_Lab2,
				20
			);
		}
	}
};