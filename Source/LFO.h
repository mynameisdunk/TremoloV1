#pragma once
#include <JuceHeader.h>

class sineLFO
{
public:
    sineLFO();
    
    static constexpr float twoPi = juce::MathConstants<float>::twoPi;
    
    void prepare(float sampleRate);
    void setFrequency (float frequencyHz);
    float process(); // Literally the LFO code
    void reset();
    
private:
    
    float mPhase;
    float mIncrement;
    float mFrequency;
    float mSampleRate;
    
    void calculateIncrement()
    {
        mIncrement = mFrequency / mSampleRate;
    }

};

//=====================================================================================================================================================================================

class triangleLFO
{
public:
    triangleLFO();
    
    static constexpr float twoPi = juce::MathConstants<float>::twoPi;
    
    void prepare(float sampleRate);
    void setFrequency (float frequencyHz);
    float process(); // Literally the LFO code
    void reset();
  
    
private:
    
    float mPhase;
    float mIncrement;
    float mFrequency;
    float mSampleRate;
    float skewAmount = 1.4f;
    
    void calculateIncrement()
    {
        mIncrement = mFrequency / mSampleRate;
    }

};


//================================================================================================================================================================================

class squareLFO
{
    public:
    squareLFO();
    
    static constexpr float twoPi = juce::MathConstants<float>::twoPi;
    
    void prepare(float sampleRate);
    void setFrequency(float frequencyHz);
    void setPulseWidth(float pulseWidth);
    float softClip (float waveValue);
    float process();
    void reset();
    
    private:
    
    float mPhase;
    float mIncrement;
    float mFrequency;
    float mSampleRate;
    float mPulseWidth;
    
    void calculateIncrement()
    {
        mIncrement = mFrequency / mSampleRate;
    }
    
    // In your LFO class:
    private:
    
    float mLastOutput = 0.0f;


};

//================================================================================================================================================================================



class clippedSineLFO
{
public:
    clippedSineLFO();
    
    static constexpr float twoPi = juce::MathConstants<float>::twoPi;
    
    void prepare(float sampleRate);
    void setFrequency (float frequencyHz);
    float process(); // Literally the LFO code
    void reset();
    
private:
    
    float mPhase;
    float mIncrement;
    float mFrequency;
    float mSampleRate;
    float mThreshold;

    
    void calculateIncrement()
    {
        mIncrement = mFrequency / mSampleRate;
    }

};
