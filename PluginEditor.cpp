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

	ui_mappingMatrix.populatePresets(mapPresetPtr,mpArrayPtr,apArrayPtr);

	// SET INITIAL TAB
	switchTab(0);
}

StsinteractionAudioProcessorEditor::~StsinteractionAudioProcessorEditor()
{
}

void StsinteractionAudioProcessorEditor::comboBoxChanged(ComboBox *box)
{
	for (int i = 0; i < sensInfoPtr->numSensorsMax; i++)
	{
		if (box == &ui_sensorCon.Location[i])
			sensInfoPtr->bodyLocation[i] = ui_sensorCon.Location[i].getSelectedId();
	}

	if (box == &ui_movementAnalysis.operationMode)
	{
		mAnalysisPtr->operationMode_Present = box->getSelectedId();
	}

	if (box == &ui_musicControl.tonic)
		musInfoCompPtr->idx_tonic_Present = box->getSelectedId() - 1;

	if (box == &ui_musicControl.scale)
		musInfoCompPtr->idx_scale_Present = box->getSelectedId() - 1;

	if (box == &ui_musicControl.chord_Type)
		musInfoCompPtr->idx_chordTypes_Present = box->getSelectedId() - 1;

	for (int i = 0; i < 8; i++)
	{
		if (box == &ui_musicControl.chord_Degree[i])
		{
			musInfoCompPtr->chord_degSequence[i] = box->getSelectedId();
			musInfoCompPtr->set_order_MEL(i, box->getSelectedId());
		}
	}

	for (int i = 0; i < musControlPtr->numFbVariables; i++)
	{
		if (box == &ui_mappingMatrix.mapping_Function[i])
		{
			apArrayPtr[i].mapFunc = box->getSelectedId();
		}

		if (box == &ui_mappingMatrix.mapping_Polarity[i])
		{
			apArrayPtr[i].polarity = box->getSelectedId();
		}

		if (box == &ui_mappingMatrix.mapping_QuantLevels[i])
		{
			apArrayPtr[i].quantLevels_2raisedTo
				= box->getSelectedId() - 1;
		}

		if (box == &ui_mappingMatrix.preset_ListLoad)
		{
			ui_mappingMatrix.loadPreset(&mapPresetPtr[box->getSelectedId() - 1]);
			auto preset = &mapPresetPtr[box->getSelectedId() - 1];
			
			// SET VOICE CUE RELATED CONTROLS
			ui_musicControl.triOsc_BPM.setValue(preset->dataHolder_oscBPM * 60);
			ui_musicControl.voiceCue_Enable.setToggleState(preset->dataHolder_voiceCue_ON,sendNotificationSync);
			ui_musicControl.voiceCue_Level.setValue(preset->dataHolder_voiceCue_voldB);
			ui_musicControl.voiceCue_CountLength.setValue(preset->dataHolder_voiceCue_Length);
			ui_musicControl.voiceCue_FineTimeAdjust.setValue(preset->dataHolder_voiceCue_timingFine);

			for (int i = 0; i < 3; i++)
			{
				ui_musicControl.voiceCue_intervalEnable[i].setToggleState(preset->dataHolder_voiceCue_isIntervalEnabled[i],	sendNotificationSync);
				ui_musicControl.voiceCue_isPosCrossing[i].setToggleState(preset->dataHolder_voiceCue_isPos[i], sendNotificationSync);
				ui_musicControl.voiceCue_OscTrig_Location[i].setValue(preset->dataHolder_voiceCue_location[i]);
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (box == &ui_movementAnalysis.IMU_Mount_Side[i])
				mAnalysisPtr->IMU_Mount_Side[i] = box->getSelectedId();

			if (box == &ui_movementAnalysis.IMU_Polarity[i])
				mAnalysisPtr->IMU_Polarity[i] = box->getSelectedId();
		}
	}

	if (box == &ui_mappingMatrix.mp_isVisible)
	{
		if (box->getSelectedId() <= mAnalysisPtr->numMovementParams)
		{
			ui_mappingMatrix.toggleVisible(false, mpArrayPtr, apArrayPtr);
			mpArrayPtr[box->getSelectedId() - 1].isVisible = false;
			mappingMatrix_resetLayout();
		}
	}

	if (box == &ui_mappingMatrix.mp_isHidden)
	{
		if (box->getSelectedId() <= mAnalysisPtr->numMovementParams)
		{
			ui_mappingMatrix.toggleVisible(false, mpArrayPtr, apArrayPtr);
			mpArrayPtr[box->getSelectedId() - 1].isVisible = true;
			mappingMatrix_resetLayout();
		}
	}

	if (box == &ui_mappingMatrix.ap_isVisible)
	{
		if (box->getSelectedId() <= musControlPtr->numFbVariables)
		{
			ui_mappingMatrix.toggleVisible(false, mpArrayPtr, apArrayPtr);
			apArrayPtr[box->getSelectedId() - 1].isVisible = false;
			mappingMatrix_resetLayout();
		}
	}

	if (box == &ui_mappingMatrix.ap_isHidden)
	{
		if (box->getSelectedId() <= musControlPtr->numFbVariables)
		{
			ui_mappingMatrix.toggleVisible(false, mpArrayPtr, apArrayPtr);
			apArrayPtr[box->getSelectedId() - 1].isVisible = true;
			mappingMatrix_resetLayout();
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

	if (mAnalysisPtr->operationMode_Present == 1)
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
		ui_movementAnalysis.toggleVisible(false, mAnalysisPtr->dataInput_Mode);
		ui_musicControl.toggleVisible(false);
		ui_mappingMatrix.toggleVisible(false,mpArrayPtr,apArrayPtr);
		break;
	case 1:												// Movement Tab
		ui_sensorCon.toggleVisible(false);
		ui_movementAnalysis.toggleVisible(true, mAnalysisPtr->dataInput_Mode);
		ui_musicControl.toggleVisible(false);
		ui_mappingMatrix.toggleVisible(false, mpArrayPtr, apArrayPtr);
		break;
	case 2:												// Music Tab
		ui_sensorCon.toggleVisible(false);
		ui_movementAnalysis.toggleVisible(false, mAnalysisPtr->dataInput_Mode);
		ui_musicControl.toggleVisible(true);
		ui_mappingMatrix.toggleVisible(false, mpArrayPtr, apArrayPtr);
		break;
	case 3:												// Mapping Matrix
		ui_sensorCon.toggleVisible(false);
		ui_movementAnalysis.toggleVisible(false, mAnalysisPtr->dataInput_Mode);
		ui_musicControl.toggleVisible(false);
		ui_mappingMatrix.toggleVisible(true, mpArrayPtr, apArrayPtr);
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
		FileChooser logChooser("Please select a valid log folder.",
			File::getSpecialLocation(File::currentApplicationFile), extension);

		switch (mAnalysisPtr->dataInput_Mode)
		{
		case 0:
			if (logChooser.browseForDirectory())
			{
				File logFile(logChooser.getResult());
				filePath = logFile.getFullPathName();
				//if (mAnalysisPtr->open_mpLogFile_forStream(filePath))
				if (mAnalysisPtr->open_mpLogDir_IMU_Raw(filePath))
				{
					mAnalysisPtr->dataInput_Mode = 1;
					ui_movementAnalysis.mpLog_File_Play_Pause.setVisible(true);
					ui_movementAnalysis.mpLog_File_Stop.setVisible(false);
					ui_movementAnalysis.mpLog_File_Progress.setVisible(true);
					ui_movementAnalysis.mpLog_File_Load_Unload.setButtonText("Unload");
				}
			}
			
			break;

		case 1:
			// UNLOAD
			mAnalysisPtr->dataInput_Mode = 0;
			ui_movementAnalysis.mpLog_File_Play_Pause.setVisible(false);
			ui_movementAnalysis.mpLog_File_Stop.setVisible(false);
			ui_movementAnalysis.mpLog_File_FWD.setVisible(false);
			ui_movementAnalysis.mpLog_File_RWD.setVisible(false);
			ui_movementAnalysis.mpLog_File_Progress.setVisible(false);
			ui_movementAnalysis.mpLog_File_Load_Unload.setButtonText("Load Log");
			break;
		case 2:
			// UNLOAD
			mAnalysisPtr->dataInput_Mode = 0;
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
		switch (mAnalysisPtr->dataInput_Mode)
		{
		case 1:
			// START PLAYBACK
			mAnalysisPtr->dataInput_Mode = 2;
			ui_movementAnalysis.mpLog_File_Stop.setVisible(true);
			ui_movementAnalysis.mpLog_File_FWD.setVisible(true);
			ui_movementAnalysis.mpLog_File_RWD.setVisible(true);
			ui_movementAnalysis.mpLog_File_Play_Pause.setButtonText("Pause");
			break;
		case 2:
			// PAUSE PLAYBACK
			mAnalysisPtr->dataInput_Mode = 1;
			ui_movementAnalysis.mpLog_File_Play_Pause.setButtonText("Loop Play Data");
			break;
		}
	};

	ui_movementAnalysis.mpLog_File_Stop.onClick = [this]
	{
		mAnalysisPtr->dataInput_Mode = 1;
		mAnalysisPtr->mpFile_Streaming_Line_Current = 0;
		ui_movementAnalysis.mpLog_File_FWD.setVisible(false);
		ui_movementAnalysis.mpLog_File_RWD.setVisible(false);
		mAnalysisPtr->mpFile_Streaming_Progress = 0;
		ui_movementAnalysis.mpLog_File_Play_Pause.setButtonText("Loop Play Data");
	};

}

void StsinteractionAudioProcessorEditor::resized()
{
	ui_sensorCon.setLayout();
	ui_movementAnalysis.setLayout();
	ui_musicControl.setLayout(musControlPtr->mixerSettings.num_Tracks);
	ui_mappingMatrix.setLayout
	(
		interface_Width,
		interface_Height,
		mAnalysisPtr->numMovementParams,
		musControlPtr->numFbVariables,
		mpArrayPtr,
		apArrayPtr
	);
}
