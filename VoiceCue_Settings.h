#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class VoiceCues
{
    public:
    VoiceCues()
    {
    }
    
    ~VoiceCues()
    {
    }
    
    bool isEnabled = false;
    int count_Length = 4;
    int count_Present = 0;
    bool interval_IsEnabled[8] = {false, false, false, false, false, false, false, false};
    bool isPosCrossing[8] = {true, true, true, true, true, true, true, true};
    float interval_crossThresh[8] = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    float oscSig_z1 = 0;
    float voiceGain_dB = 0;
    
    bool getVoiceTriggerSignal(float oscSig)
    {
        bool isTrigger = false;
        bool validCrossingCondition[8] = { false,false, false, false, false, false, false, false };

        if (isEnabled)
        {
            for (int i = 0; i < 8; i++)
            {
                if (interval_IsEnabled[i])
                {
                    if (isPosCrossing[i])
                    {
                        validCrossingCondition[i] = (oscSig >= interval_crossThresh[i]) && (oscSig_z1 < interval_crossThresh[i]);
                    }

                    else
                    {
                        validCrossingCondition[i] = (oscSig < interval_crossThresh[i]) && (oscSig_z1 >= interval_crossThresh[i]);
                    }
                }
            }
        }
        
        for (int j = 0; j < 8; j++)
        {
            if (validCrossingCondition[j]) isTrigger = true;
        }

        if (isTrigger)
        {
            count_Present = (count_Present + 1) % count_Length;
        }

        oscSig_z1 = oscSig;

        return isTrigger;
    }
};