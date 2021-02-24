#include "PluginProcessor.h"
#include "PluginEditor.h"

StsinteractionAudioProcessorEditor::StsinteractionAudioProcessorEditor (StsinteractionAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	// CONFIGURE INTERFACE AND TABS, ADD CONTROLS
    setSize (interface_Width, interface_Height);
	tabs = std::make_unique<TabbedComponent>(TabbedButtonBar::TabsAtTop);
	addAndMakeVisible(*tabs);
	tabs->addTab("Sensors", Colours::black, Component::getParentComponent(), 1);
	tabs->addTab("Movement", Colours::black, Component::getParentComponent(), 1);
	tabs->addTab("Music", Colours::black, Component::getParentComponent(), 1);
	tabs->addTab("Mapping Matrix", Colours::black, Component::getParentComponent(), 1);
	tabs->setBounds(0, 0, getWidth(), getHeight());
	addControls_ALL();
	startTimerHz(UIRefreshFreq);								// START UI TIMER CALLBACK

	// INITIALIZE CONTROLS
	sensorConfig_initializeControls();
	movementAnalysis_initializeControls();
	musicControl_initializeControls();
	mappingMatrix_initializeControls();

	ui_mappingMatrix.populatePresets(processor.movementAnalysis.musicControl.mappingPresets);

	// SET INITIAL TAB
	switchTab(0);
}

StsinteractionAudioProcessorEditor::~StsinteractionAudioProcessorEditor()
{
}

void StsinteractionAudioProcessorEditor::comboBoxChanged(ComboBox *box)
{
	for (int i = 0; i < processor.movementAnalysis.sensorInfo.numSensorsMax; i++)
	{
		if (box == &ui_sensorCon.Location[i])
		{
			processor.movementAnalysis.sensorInfo.bodyLocation[i] = ui_sensorCon.Location[i].getSelectedId();
		}
	}

	if (box == &ui_movementAnalysis.operationMode)
	{
		processor.movementAnalysis.operationMode_Present = box->getSelectedId();
	}

	if (box == &ui_movementAnalysis.orientationAlgo)
	{
		processor.movementAnalysis.orientAlgo_Present = box->getSelectedId();
	}

	if (box == &ui_musicControl.tonic)
	{
		processor.movementAnalysis.musicControl.musicInfoCompute.idx_tonic_Present = 
			box->getSelectedId() - 1;
	}

	if (box == &ui_musicControl.scale)
	{
		processor.movementAnalysis.musicControl.musicInfoCompute.idx_scale_Present = 
			box->getSelectedId() - 1;
	}

	if (box == &ui_musicControl.chord_Type)
	{
		processor.movementAnalysis.musicControl.musicInfoCompute.idx_chordTypes_Present =
			box->getSelectedId() - 1;
	}

	for (int i = 0; i < 8; i++)
	{
		if (box == &ui_musicControl.chord_Degree[i])
		{
			processor.movementAnalysis.musicControl.musicInfoCompute.chord_degSequence[i] =
				box->getSelectedId();
		}
	}

	for (int i = 0; i < processor.movementAnalysis.musicControl.numFbVariables; i++)
	{
		if (box == &ui_mappingMatrix.mapping_Function[i])
		{
			processor.movementAnalysis.musicControl.feedbackVariables[i].mapFunc = box->getSelectedId();
		}

		if (box == &ui_mappingMatrix.mapping_Polarity[i])
		{
			processor.movementAnalysis.musicControl.feedbackVariables[i].polarity = box->getSelectedId();
		}

		if (box == &ui_mappingMatrix.mapping_QuantLevels[i])
		{
			processor.movementAnalysis.musicControl.feedbackVariables[i].quantLevels_2raisedTo 
				= box->getSelectedId() - 1;
		}

		if (box == &ui_mappingMatrix.preset_ListLoad)
		{
			ui_mappingMatrix.loadPreset(&processor.movementAnalysis.musicControl.mappingPresets
				[box->getSelectedId() - 1]);
		}

		for (int i = 0; i < 3; i++)
		{
			if (box == &ui_movementAnalysis.IMU_Mount_Side[i])
				processor.movementAnalysis.IMU_Mount_Side[i] = box->getSelectedId();

			if (box == &ui_movementAnalysis.IMU_Polarity[i])
				processor.movementAnalysis.IMU_Polarity[i] = box->getSelectedId();
		}
	}
}

void StsinteractionAudioProcessorEditor::timerCallback()
{
	// CHECK FOR TAB CHANGES
	presentTab = tabs->getCurrentTabIndex();
	if (previousTab != presentTab)
		switchTab(presentTab);
	previousTab = presentTab;

	if (processor.movementAnalysis.operationMode_Present == 1)
		simulation_HandleKeyPresses();

	// UPDATE LABELS ON PRESENT TAB
	switch (presentTab)
	{
	case 0:
		sensorConfig_updateLabels();
		break;
	case 1:
		movementAnalysis_updateLabels();
		break;
	case 2:
		musicControls_updateLabels();
		break;
	case 3:
		mappingMatrix_updateLabels();
		break;
	}
}

void StsinteractionAudioProcessorEditor::switchTab(int currentTab)
{
	// SET FLAGS BASED ON TAB INDEX
	switch (currentTab)
	{
	case 0:												// Sensors Tab
		ui_sensorCon.toggleVisible(true);
		ui_movementAnalysis.toggleVisible(false, processor.movementAnalysis.dataInput_Mode);
		ui_musicControl.toggleVisible(false);
		ui_mappingMatrix.toggleVisible(false);
		break;
	case 1:												// Movement Tab
		ui_sensorCon.toggleVisible(false);
		ui_movementAnalysis.toggleVisible(true, processor.movementAnalysis.dataInput_Mode);
		ui_musicControl.toggleVisible(false);
		ui_mappingMatrix.toggleVisible(false);
		break;
	case 2:												// Music Tab
		ui_sensorCon.toggleVisible(false);
		ui_movementAnalysis.toggleVisible(false, processor.movementAnalysis.dataInput_Mode);
		ui_musicControl.toggleVisible(true);
		ui_mappingMatrix.toggleVisible(false);
		break;
	case 3:												// Mapping Matrix
		ui_sensorCon.toggleVisible(false);
		ui_movementAnalysis.toggleVisible(false, processor.movementAnalysis.dataInput_Mode);
		ui_musicControl.toggleVisible(false);
		ui_mappingMatrix.toggleVisible(true);
		break;
	}
}

void StsinteractionAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void StsinteractionAudioProcessorEditor::mpLogStream_Configure_Button_Behaviors()
{
	ui_movementAnalysis.mpLog_File_Load_Unload.onClick = [this]
	{
		String filePath = "";
		String extension = "MP Log.csv";
		FileChooser logChooser("Please select a valid log file:",
			File::getSpecialLocation(File::userHomeDirectory), extension);

		switch (processor.movementAnalysis.dataInput_Mode)
		{
		case 0:
			if (logChooser.browseForFileToOpen())
			{
				File logFile(logChooser.getResult());
				filePath = logFile.getFullPathName();
				if (processor.movementAnalysis.open_mpLogFile_forStream(filePath))
				{
					processor.movementAnalysis.dataInput_Mode = 1;
					ui_movementAnalysis.mpLog_File_Play_Pause.setVisible(true);
					ui_movementAnalysis.mpLog_File_Stop.setVisible(false);
					ui_movementAnalysis.mpLog_File_Progress.setVisible(true);
					ui_movementAnalysis.mpLog_File_Load_Unload.setButtonText("Unload");
				}
			}
			
			break;

		case 1:
			// UNLOAD
			processor.movementAnalysis.dataInput_Mode = 0;
			ui_movementAnalysis.mpLog_File_Play_Pause.setVisible(false);
			ui_movementAnalysis.mpLog_File_Stop.setVisible(false);
			ui_movementAnalysis.mpLog_File_FWD.setVisible(false);
			ui_movementAnalysis.mpLog_File_RWD.setVisible(false);
			ui_movementAnalysis.mpLog_File_Progress.setVisible(false);
			ui_movementAnalysis.mpLog_File_Load_Unload.setButtonText("Load Log");
			break;
		case 2:
			// UNLOAD
			processor.movementAnalysis.dataInput_Mode = 0;
			ui_movementAnalysis.mpLog_File_Play_Pause.setVisible(false);
			ui_movementAnalysis.mpLog_File_Stop.setVisible(false);
			ui_movementAnalysis.mpLog_File_FWD.setVisible(false);
			ui_movementAnalysis.mpLog_File_RWD.setVisible(false);
			ui_movementAnalysis.mpLog_File_Progress.setVisible(false);
			ui_movementAnalysis.mpLog_File_Load_Unload.setButtonText("Load Log");
			break;
		}
	};

	ui_movementAnalysis.mpLog_File_Play_Pause.onClick = [this]
	{
		switch (processor.movementAnalysis.dataInput_Mode)
		{
		case 1:
			// START PLAYBACK
			processor.movementAnalysis.dataInput_Mode = 2;
			ui_movementAnalysis.mpLog_File_Stop.setVisible(true);
			ui_movementAnalysis.mpLog_File_FWD.setVisible(true);
			ui_movementAnalysis.mpLog_File_RWD.setVisible(true);
			ui_movementAnalysis.mpLog_File_Play_Pause.setButtonText("Pause");
			break;
		case 2:
			// PAUSE PLAYBACK
			processor.movementAnalysis.dataInput_Mode = 1;
			ui_movementAnalysis.mpLog_File_Play_Pause.setButtonText("Loop Play Data");
			break;
		}
	};

	ui_movementAnalysis.mpLog_File_Stop.onClick = [this]
	{
		processor.movementAnalysis.dataInput_Mode = 1;
		processor.movementAnalysis.mpFile_Streaming_Line_Current = 0;
		ui_movementAnalysis.mpLog_File_FWD.setVisible(false);
		ui_movementAnalysis.mpLog_File_RWD.setVisible(false);
		processor.movementAnalysis.mpFile_Streaming_Progress = 0;
		ui_movementAnalysis.mpLog_File_Play_Pause.setButtonText("Loop Play Data");
	};

}

void StsinteractionAudioProcessorEditor::resized()
{
	ui_sensorCon.setLayout();
	ui_movementAnalysis.setLayout();
	ui_musicControl.setLayout(processor.movementAnalysis.musicControl.mixerSettings.num_Tracks);
	ui_mappingMatrix.setLayout
	(
		interface_Width,
		interface_Height,
		processor.movementAnalysis.numMovementParams,
		processor.movementAnalysis.musicControl.numFbVariables,
		processor.movementAnalysis.movementParams,
		processor.movementAnalysis.musicControl.feedbackVariables
	);
}
