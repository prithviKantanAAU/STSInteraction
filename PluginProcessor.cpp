#include "PluginProcessor.h"
#include "PluginEditor.h"

StsinteractionAudioProcessor::StsinteractionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	startTimer(1);
}

StsinteractionAudioProcessor::~StsinteractionAudioProcessor()
{
}

void StsinteractionAudioProcessor::mapFBVariable(int fbVar_Idx)
{
    // FOR EACH SYNTH CONTROL
    for (int i = 0; i < movementAnalysis.musicControl.feedbackVariables[fbVar_Idx].numSynthControls; i++)
    {
        // MAP ARRAY VALUES TO DSPFAUST CONTROLS
        String address = movementAnalysis.musicControl.faustStrings.getFBVar_FAUSTAddress_Full(fbVar_Idx, i);
        fUI->setParamValue(address.toStdString().c_str(),
            movementAnalysis.musicControl.fbVar_FinalVals[fbVar_Idx][i]);
    }
}

void StsinteractionAudioProcessor::hiResTimerCallback()
{
	pulsesElapsed = (pulsesElapsed + 1) % 1000000;				// INCREMENT AND AVOID OVERFLOW
	
	if (pulsesElapsed % 10 == 0)								// Once every 10 ms
	{
		// UPDATE IMU BUFFERS FOR ALL ACTIVE SENSORS
		for (int i = 0; i < movementAnalysis.sensorInfo.numSensorsMax; i++)
			if (movementAnalysis.sensorInfo.isOnline[i])
				movementAnalysis.sensors_OSCReceivers[i].updateBuffers();

		// MOVEMENT ANALYSIS, FB COMPUTATION, MAPPING CALLBACK
		movementAnalysis.callback();
        if (isReady)
        {
            for (int i = 0; i < movementAnalysis.musicControl.numFbVariables; i++)
            {
                mapFBVariable(i);
            }
        }
		ipVerify_AssignedSensors();

		// WRITE LINE TO LOG IF RECORDING
		if (is_Recording_MPLog)
			writeLine_Recording_MPLog(movementAnalysis.movementParams);
	}

	// UPDATE EVERY 2 SEC
	if (pulsesElapsed % 2000 == 0)
	{
		movementAnalysis.updateSensorStatus();
	}

	if (pulsesElapsed % 25000 == 0)
	{
		if (movementAnalysis.orientAlgo_Present == 2)
		{
			for (int i = 0; i < 3; i++)
				movementAnalysis.compFilters[i].trunk_CalibrateRest(
					movementAnalysis.sensors_OSCReceivers[movementAnalysis.locationsOnline[i]].acc_Buf
				);
		}
	}
}

void StsinteractionAudioProcessor::start_Recording_MPLog()
{
    // File for Movement Parameters
	String filepath_Rec = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
	filepath_Rec = filepath_Rec.upToLastOccurrenceOf("\\", true, false);
	filepath_Rec += "MP Log - " + getCurrentTime();
	String filepath_Rec_FULL = filepath_Rec + "\\MP Log.csv";
	CreateDirectory(filepath_Rec.toStdString().c_str(), NULL);
	mpLog = fopen(filepath_Rec_FULL.toStdString().c_str(), "w");

    // RAW LOGS FOR ALL SENSORS
    short bodyPartIndex = 0;
    String sensorFileNameRaw;
    for (int i = 0; i < movementAnalysis.sensorInfo.numSensorsMax; i++)
    {
        if (movementAnalysis.sensorInfo.isOnline[i])
        {
            bodyPartIndex = movementAnalysis.sensorInfo.bodyLocation[i];
            sensorFileNameRaw = filepath_Rec + "\\Raw IMU Data - " + String(bodyPartIndex) + ".csv";
            imuRaw_Log[i] = fopen(sensorFileNameRaw.toStdString().c_str(), "w");
        }
    }
	
	//WRITE FIRST LINE TO LOG - MP NAMES
	String line = "";
	for (int i = 0; i < movementAnalysis.numMovementParams; i++)
		line += movementAnalysis.movementParams[i].name + ",";
	fprintf(mpLog, mpLog_FormatSpec.c_str(), line);

	is_Recording_MPLog = true;
}

void StsinteractionAudioProcessor::stop_Recording_MPLog()
{
	is_Recording_MPLog = false;
	
    fclose(mpLog);
    for (int i = 0; i < movementAnalysis.sensorInfo.numSensorsMax; i++)
    {
        if (movementAnalysis.sensorInfo.isOnline[i])
            fclose(imuRaw_Log[i]);
    }
}

void StsinteractionAudioProcessor::writeLine_Recording_MPLog(MovementParameter mpArray[])
{
	String line = "";
	for (int i = 0; i < movementAnalysis.numMovementParams; i++)
		line += String(movementAnalysis.movementParams[i].value,5) + ",";
	fprintf(mpLog, mpLog_FormatSpec.c_str(), line);

    for (int i = 0; i < movementAnalysis.sensorInfo.numSensorsMax; i++)
    {
        line = "";
        
        if (movementAnalysis.sensorInfo.isOnline[i])
        {
            line += String(movementAnalysis.sensors_OSCReceivers[i].acc[0]) + ",";
            line += String(movementAnalysis.sensors_OSCReceivers[i].acc[1]) + ",";
            line += String(movementAnalysis.sensors_OSCReceivers[i].acc[2]) + ",";
            line += String(movementAnalysis.sensors_OSCReceivers[i].gyr[0]) + ",";
            line += String(movementAnalysis.sensors_OSCReceivers[i].gyr[1]) + ",";
            line += String(movementAnalysis.sensors_OSCReceivers[i].gyr[2]) + ",";
            line += String(movementAnalysis.sensors_OSCReceivers[i].mag[0]) + ",";
            line += String(movementAnalysis.sensors_OSCReceivers[i].mag[1]) + ",";
            line += String(movementAnalysis.sensors_OSCReceivers[i].mag[2]);
            fprintf(imuRaw_Log[i], mpLog_FormatSpec.c_str(), line);
        }
    }
}

const String StsinteractionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StsinteractionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StsinteractionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StsinteractionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StsinteractionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StsinteractionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StsinteractionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StsinteractionAudioProcessor::setCurrentProgram (int index)
{
}

const String StsinteractionAudioProcessor::getProgramName (int index)
{
    return {};
}

void StsinteractionAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void StsinteractionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fDSP = new mydsp();
    fDSP->init(sampleRate);
    fUI = new MapUI();
    fDSP->buildUserInterface(fUI);
    outputs = new float* [2];
    for (int channel = 0; channel < 2; ++channel) {
        outputs[channel] = new float[samplesPerBlock];
    }
    isReady = true;
    set_COMP_EQ();
}

void StsinteractionAudioProcessor::releaseResources()
{
    if (isDSP_ON)
        stopMusicDSP();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StsinteractionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StsinteractionAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    float outputVoice = 0.0;

    if (isReady)
    {
        fDSP->compute(buffer.getNumSamples(), NULL, outputs);                           // GET MUSIC FROM FAUST

        for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
            for (int i = 0; i < buffer.getNumSamples(); i++) {
                
                outputVoice = audio_preLoaded.playVoiceSample(                          // GET VOICE AUDIO DATA
                    movementAnalysis.voice_isTrigger, 
                    movementAnalysis.voiceCue.count_Present,
                    channel, i, buffer.getNumSamples())
                    * Decibels::decibelsToGain(movementAnalysis.voiceCue.voiceGain_dB);

                *buffer.getWritePointer(channel, i) = outputs[channel][i] + outputVoice; // SUM DATA
            }

            // GET DB LEVEL FOR METERING
            musicLevel_dB = fmax(-60,Decibels::gainToDecibels(buffer.getMagnitude(0,0,buffer.getNumSamples())));
        }
    }
}

//==============================================================================
bool StsinteractionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* StsinteractionAudioProcessor::createEditor()
{
    return new StsinteractionAudioProcessorEditor (*this);
}

//==============================================================================
void StsinteractionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StsinteractionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StsinteractionAudioProcessor();
}
