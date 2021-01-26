#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MovementAnalysis.h"

class StsinteractionAudioProcessor  : public AudioProcessor, public HighResolutionTimer
{
public:
    StsinteractionAudioProcessor();
    ~StsinteractionAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	
	MovementAnalysis movementAnalysis;

	// CALLBACK RELATED
	void hiResTimerCallback();
	long pulsesElapsed = 0;

	// SENSOR RELATED
	void ipVerify_AssignedSensors()
	{
		int remotePort = 0;
		String remoteIP = "";

		for (int i = 0; i < movementAnalysis.sensorInfo.numSensorsMax; i++)		// CHECK FOR ALL SENSORS
			if (movementAnalysis.sensorInfo.bodyLocation[i] != 4)				// ONLY IF ASSIGNED
			{
				if (!movementAnalysis.sensorInfo.isOnline[i])					// ONLY IF OFFLINE
				{
					remotePort = movementAnalysis.sensorInfo.UDP_Ports_REMOTE[i];
					remoteIP = movementAnalysis.sensorInfo.remoteIP[i];
					if (movementAnalysis.sensorInfo.connectionVerify_IP[i].connect(remoteIP, remotePort))
					{
						OSCMessage message("/CONNECT");
						message.addString("Please Connect");
						movementAnalysis.sensorInfo.connectionVerify_IP[i].send(message);
					}
				}
			}
	}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StsinteractionAudioProcessor)
};
