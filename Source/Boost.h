#pragma once
#include <JuceHeader.h>
#include "Compander.h"


class jfetBoost
{
public:
    jfetBoost()
    {
        mBias = 0.3f;
        mDriveLevel = 1.0f;
    }
    
    void prepare(double sampleRate)
    {
        mCompressor.prepare(sampleRate);
        mExpander.prepare(sampleRate);
    }
    
    void setDriveLevel( float driveLevel)
    {
        mDriveLevel = driveLevel * mDriveScaler;
    }
    
    
    float process(float input)
    {
        float compressed = mCompressor.process(input);
        float driven = compressed * mDriveLevel;
        float processed = std::tanh(driven + mBias) - std::tanh(mBias);
        float output = mExpander.process(processed);
        
        return output;
    };
   
    void reset()
    {
        mCompressor.reset();
        mExpander.reset();
    }
private:
    
    Compressor mCompressor;
    Expander mExpander;
    
    float mBias;
    float mDriveLevel;
    float mDriveScaler = 3.0f;
    
};
