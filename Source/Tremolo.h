#pragma once
#include <JuceHeader.h>
#include "LFO.h"  // Your existing LFO class
#include "WaveSelector.h"

class Tremolo
{
public:
    Tremolo()
    {
        mDepth = 0.5f;
        mMinVolume = 0.0f;
        mMaxVolume = 1.0f;
        mRate = 5.0f;
    }
    
    void prepare(float sampleRate)
    {
        mWaveSelector.prepare(sampleRate);
        mWaveSelector.setRate(5.0f);  // Default 5 Hz rate
        mWaveSelector.setPulseWidth(0.5f);
    }
    
    // User controls
    void setRate(float rateHz)
    {
        mWaveSelector.setRate(rateHz);
    }
    
    void setDepth(float depth)  // 0.0 to 1.0
    {
        mDepth = juce::jlimit(0.0f, 1.0f, depth);
    }
    
    
    // Optional: customize the volume range
    void setVolumeRange(float minVolume, float maxVolume)
    {
        mMinVolume = juce::jlimit(0.0f, 1.0f, minVolume);
        mMaxVolume = juce::jlimit(0.0f, 1.0f, maxVolume);
    }
    
    void setBlend (float newBlend)
    {
        mWaveSelector.setBlend(newBlend);
    }
    
    void setPulseWidth(float pulseWidth)
    {
        mWaveSelector.setPulseWidth(pulseWidth);
    }
    
    // Process a single sample
    float process(float inputSample)
    {
        
        // Step 1: Get LFO value (-1 to 1)
        float lfoValue = mWaveSelector.process();
        
        // Step 2: Convert to unipolar (0 to 1)
        float lfoUnipolar = (lfoValue + 1.0f) * 0.5f;
        
        // Step 3: Calculate volume multiplier with depth control
        float range = mMaxVolume - mMinVolume;
        float volumeMultiplier = mMaxVolume - (range * lfoUnipolar * mDepth);
        
        // Step 4: Apply to input
        float output = inputSample * volumeMultiplier;
        
        return output;
    }
    
    void reset()
    {
        mWaveSelector.reset();
    }
    
private:
    
    waveSelector mWaveSelector;
    float mDepth;
    float mMinVolume;
    float mMaxVolume;
    float mRate;
};
