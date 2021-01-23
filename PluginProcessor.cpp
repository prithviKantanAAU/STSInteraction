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
		ipVerify_AssignedSensors();
	}

	if (pulsesElapsed % 2000 == 0)
	{
		if (pulsesElapsed % 2000 == 0)
			movementAnalysis.updateSensorStatus();
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StsinteractionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
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
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
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
