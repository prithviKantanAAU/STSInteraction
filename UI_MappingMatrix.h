#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

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
    
    void configure()
    {
    }
    
    void toggleVisible(bool on)
    {
		for (int i = 0; i < 20; i++)		// Columns
		{
			labels_movementParams[i].setVisible(on);
			labels_audioParams[i].setVisible(on);
			mapping_Function[i].setVisible(on);
			mapping_Polarity[i].setVisible(on);
			for (int j = 0; j < 20; j++)	// Rows
			{
				mapping_Matrix[i][j].setVisible(on);
			}
		}
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
				matrix_startPointX - 150, 
				matrix_startPointY + gap_interRow * i,
				150,25);

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

			labels_audioParams[k].setBounds(
				matrix_startPointX + gap_interCol * k - gap_interCol * 0.3,
				matrix_startPointY + numMP * gap_interRow + 100,
				gap_interCol * 0.95,
				25
			);
		}

    }
};