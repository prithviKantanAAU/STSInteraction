#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MovementAnalysis.h"
#include "GaitParam_Single.h"
#include <ctime>
#include "windows.h"
#include "STS_Synth.h"

class dsp;
class MapUI;

class StsinteractionAudioProcessor  : public AudioProcessor, public HighResolutionTimer
{
public:
    StsinteractionAudioProcessor();
    ~StsinteractionAudioProcessor();

	void mapFBVariable(int fbVar_Idx);

	bool isReady = false;
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

	// LOGGING - START RECORD
	void start_Recording_MPLog();
	void stop_Recording_MPLog();
	void writeLine_Recording_MPLog(MovementParameter mpArray[]);
	bool is_Recording_MPLog = false;
	String getCurrentTime()
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M-%S", timeinfo);
		std::string str(buffer);

		return String(str);
	}
	FILE *mpLog;
	std::string mpLog_FormatSpec = "%s,\n";

	bool isDSP_ON = true;
	float musicLevel_dB = 0;

	void startMusicDSP()
	{
		fDSP = new mydsp();
		fDSP->init(getSampleRate());
		fUI = new MapUI();
		fDSP->buildUserInterface(fUI);
		outputs = new float* [2];
		for (int channel = 0; channel < 2; ++channel) {
			outputs[channel] = new float[getBlockSize()];
		}
		set_COMP_EQ();			
		isDSP_ON = true;
		isReady = true;
		set_masterGain(movementAnalysis.musicControl.mixerSettings.masterGain);
		for (int i = 0; i < movementAnalysis.musicControl.mixerSettings.num_Tracks; i++)
			set_trackFader(i, movementAnalysis.musicControl.mixerSettings.gain_Track[i]);

	}

	void stopMusicDSP()
	{
		if (isDSP_ON)
		{
			isReady = false;
			isDSP_ON = false;
			delete fDSP;
			delete fUI;
			for (int channel = 0; channel < 2; ++channel) {
				delete[] outputs[channel];
			}
			delete[] outputs;
		}
	}

	void set_masterGain(float faderVal)
	{
		movementAnalysis.musicControl.mixerSettings.masterGain = faderVal;
		fUI->setParamValue(movementAnalysis.musicControl.faustStrings.masterGain.toStdString().c_str(), faderVal);
	}

	void set_trackFader(short trackIdx, float faderVal)
	{
		movementAnalysis.musicControl.mixerSettings.gain_Track[trackIdx] = faderVal;
		fUI->setParamValue(movementAnalysis.musicControl.faustStrings.getTrackGainAddress(trackIdx).c_str(), faderVal);
	}

	void set_COMP_EQ()
	{
		std::string address = "";
		float value = 0;

		for (int trackIndex = 0; trackIndex < movementAnalysis.musicControl.mixerSettings.num_Tracks; trackIndex++)
		{
			for (int j = 0; j < 4; j++)		//Param ID
			{
				address = movementAnalysis.musicControl.faustStrings.FetchComp_String(trackIndex, j);
				value = movementAnalysis.musicControl.mixerSettings.fetchCompValue(trackIndex, j);
				fUI->setParamValue(address.c_str(), value);
			}
		}

		// EQ
		for (int trackIndex = 0; trackIndex < movementAnalysis.musicControl.mixerSettings.num_Tracks; trackIndex++)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					std::string address = movementAnalysis.musicControl.faustStrings.FetchEQ_String(trackIndex, i, j);
					value = movementAnalysis.musicControl.mixerSettings.fetchEQValue(trackIndex, i, j);
					fUI->setParamValue(address.c_str(), value);
				}
			}
		}
	}

private:
	MapUI* fUI;
	dsp* fDSP;
	float** outputs;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StsinteractionAudioProcessor)
};
