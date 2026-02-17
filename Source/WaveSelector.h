#pragma once
#include <JuceHeader.h>
#include "LFO.h"


class waveSelector
{
public:
    
    void prepare(float sampleRate)
    {
        mSine.prepare(sampleRate);
        mTri.prepare(sampleRate);
        mSquare.prepare(sampleRate);
        
        mSine.setFrequency(0.5f);
        mTri.setFrequency(0.5f);
        mSquare.setFrequency(0.5f);
        
    }
    
    void setRate(float rateInHz)
    {
        mSine.setFrequency(rateInHz);
        mTri.setFrequency(rateInHz);
        mSquare.setFrequency(rateInHz);
    }
    
    void setBlend(float newBlend)
    {
        blendPosition = juce::jlimit(0.0f, 1.0f, newBlend);
    }
    
    void setPulseWidth(float pulseWidth)
    {
        mSquare.setPulseWidth(pulseWidth);
    }
    
    
    float process()
    {
        float sineValue = mSine.process();
        float triValue = mTri.process();
        float squareValue = mSquare.process();
        
        if (blendPosition <= 0.5)
        {
            float blend = blendPosition / 0.5f;
            output = sineValue * (1.0f - blend) + triValue * blend;
        }
        
        else
        {
            float blend = (blendPosition - 0.5) / 0.5f;
            output = triValue * (1.0f - blend) + squareValue * blend;
        }
        
       
        return output;
    }
    
    void reset()
    {
        mSine.reset();
        mTri.reset();
        mSquare.reset();
    }
    
private:
    
    sineLFO mSine;
    triangleLFO mTri;
    squareLFO mSquare;
   
    float blendPosition = 0.5;
    
    float output;
    
    
};
