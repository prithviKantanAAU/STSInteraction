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
		int matrix_Width = 0.85 * interfaceWidth;
		int matrix_Height = 0.57 * interfaceHeight;


    }
};