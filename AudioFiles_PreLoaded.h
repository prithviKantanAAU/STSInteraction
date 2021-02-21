#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class AudioFiles_PreLoaded
{
    public:

        float voiceCues[8][48000];
        int readIndex_voiceCues = 0;
        int sampleLength_voiceCues = 0;
        bool trigger_z1 = false;
        bool isPlaying = false;

        // CURRENT FILE
        int bufferNum_Present = 0;

        AudioFiles_PreLoaded()
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 48000; j++)
                    voiceCues[i][j] = 0;
            }

            AudioFormatManager formatManager;
            formatManager.registerBasicFormats();

            AudioFormat* audioFormat = formatManager.getDefaultFormat();

            String filePath = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
            filePath = filePath.upToLastOccurrenceOf("\\", true, false) + "Voice Cues\\";
            
            for (int i = 0; i < 8; i++)
            {
                MemoryMappedAudioFormatReader* reader = audioFormat->createMemoryMappedReader(
                    filePath + String(i+1) + ".wav");
                reader->mapEntireFile();
                auto sampleRange = reader->getMappedSection();
                sampleLength_voiceCues = sampleRange.getEnd();

                for (int j = 0; j < sampleLength_voiceCues; j++)
                {
                    reader->getSample(j, &voiceCues[i][j]);
                }
            }
        }
    
        float playVoiceSample(bool trigger, int fileIdx, int channel, int smplIdx, int bufferSize)
        {
            float output = 0.0;

            // IF NOT PRESENTLY PLAYING
            if (!isPlaying)
            {
                // IF NOTHING THEN PLAY NOTHING
                if (!trigger_z1 && !trigger)
                {
                    trigger_z1 = trigger;
                    return output;
                }

                // IF TRIGGER THEN START PLAYING
                if (trigger && !trigger_z1)
                    isPlaying = true;
            }
            
            else
            {
                // HANDLE SWIFT FILE CHANGE
                if (trigger && !trigger_z1)
                    bufferNum_Present = 0;

                // FETCH PRESENT OUTPUT SAMPLE
                readIndex_voiceCues = bufferSize * bufferNum_Present + smplIdx;
                output = jlimit(-1.0,1.0,(double)voiceCues[fileIdx][readIndex_voiceCues + 1024]);
                
                if (channel == 1)
                {
                    // INCREMENT BUFFER NUMBER FOR PRESENT FILE    
                    if (smplIdx == bufferSize - 1)
                    bufferNum_Present++;

                    // STOP PLAYBACK IF FILE EXHAUSTED
                    if (readIndex_voiceCues >= sampleLength_voiceCues - 1)
                    {
                        bufferNum_Present = 0;
                        isPlaying = false;
                    }
                }
            }

            return output;
        }

         ~AudioFiles_PreLoaded()
        {
        }


};
