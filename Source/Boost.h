#pragma once
#include <JuceHeader.h>
//#include "Compander.h"


class jfetBoost
{
public:
    jfetBoost()
    {
        mBias = 0.3f;
        mDriveLevel = 1.0f;
    }
    
//    void prepare(double sampleRate)
//    {
//        mCompressor.prepare(sampleRate);
//        mExpander.prepare(sampleRate);
//    }
    
    void setDriveLevel( float driveLevel)
    {
//        float driveLevelScaled = driveLevel / mDriveScaler;
//        mDriveLevel = juce::jmax(1.0, driveLevelScaled);
        
        mDriveLevel = driveLevel;
    }
    
    
    float process(float input)
    {
//        float compressed = mCompressor.process(input);
//        float driven = compressed * mDriveLevel;
//        float processed = std::tanh(driven + mBias) - std::tanh(mBias);
//        float output = mExpander.process(processed);
        
        input *= mDriveLevel;
        float output = std::tanh(input + mBias) - std::tanh(mBias);
        return output;
    };
   
//    void reset()
//    {
//        mCompressor.reset();
//        mExpander.reset();
//    }
private:
    
//    Compressor mCompressor;
//    Expander mExpander;
    
    float mBias;
    float mDriveLevel;
    float mDriveScaler = 3.0f;
    
};
