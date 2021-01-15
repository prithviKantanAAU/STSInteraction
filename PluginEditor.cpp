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
	tabs->setBounds(0, 0, getWidth(), getHeight());
	addControls_ALL();
	startTimerHz(UIRefreshFreq);								// START UI TIMER CALLBACK

	// INITIALIZE CONTROLS
	sensorConfig_initializeControls();
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
}

void StsinteractionAudioProcessorEditor::timerCallback()
{
	// CHECK FOR TAB CHANGES
	presentTab = tabs->getCurrentTabIndex();
	if (presentTab == 0)							sensorConfig_updateLabels();
	if (previousTab != presentTab)					switchTab(presentTab);
	previousTab = presentTab;
}

void StsinteractionAudioProcessorEditor::switchTab(int currentTab)
{
	// SET FLAGS BASED ON TAB INDEX
	switch (currentTab)
	{
	case 0:												// Sensors Tab
		ui_sensorCon.toggleVisible(true);
		break;
	case 1:												// Movement Tab
		ui_sensorCon.toggleVisible(false);
		break;
	case 2:												// Music Tab
		ui_sensorCon.toggleVisible(false);
		break;
	}
	
}

void StsinteractionAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void StsinteractionAudioProcessorEditor::resized()
{
	ui_sensorCon.setLayout();
}
