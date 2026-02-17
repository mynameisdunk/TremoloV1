#include "LFO.h"




sineLFO::sineLFO()
    {
        mPhase = 0.0f;
        mIncrement = 0.0f;
        mFrequency = 1000.0f; // LFO frequency also known as RATE - set by setFreq function. Parameter value passed as argument
        mSampleRate = 44100.0f;
    }
    
void sineLFO::prepare(float sampleRate)
    {
        mSampleRate = sampleRate;
        
        calculateIncrement();
    }
    
void sineLFO::setFrequency (float frequencyHz)
    {
        mFrequency = frequencyHz;
        calculateIncrement();
    }
    
    float sineLFO::process() // Literally the LFO code
    {
        float sine = std::sin(mPhase * twoPi);
        mPhase += mIncrement;
        
        if (mPhase >= 1.0f)
        {
            mPhase -= 1.0f;
        }
        
        return sine;
    }
    
    void sineLFO::reset()
    {
        mPhase = 0.0f;
    }
    

//=====================================================================================================================================================================================


triangleLFO::triangleLFO()
    {
        mPhase = 0.0f;
        mIncrement = 0.0f;
        mFrequency = 1000.0f; // LFO frequency also known as RATE - set by setFreq function. Parameter value passed as argument
        mSampleRate = 44100.0f;
    }
    
    void triangleLFO::prepare(float sampleRate)
    {
        mSampleRate = sampleRate;
        
        calculateIncrement();
    }
    
    void triangleLFO::setFrequency (float frequencyHz)
    {
        mFrequency = frequencyHz;
        calculateIncrement();
    }
    
    float triangleLFO::process() // Literally the LFO code
    {
        float triangle = 2.0f * std::abs(2.0f * mPhase - 1.0f) - 1.0f;

        mPhase += mIncrement;
        
        float skew = skewAmount;
        
        float skewedTri = std::copysign(std::pow(std::abs(triangle), skew), triangle);
        skewedTri = juce::jlimit(-1.0f, 1.0f, skewedTri);
        
        if (mPhase >= 1.0f)
        {
            mPhase -= 1.0f;
        }
        
        return skewedTri;
    }
    
    void triangleLFO::reset()
    {
        mPhase = 0.0f;
    }

//===============================================================================================================================================================================

squareLFO::squareLFO()
    {
        mPhase = 0.0f;
        mIncrement = 0.0f;
        mFrequency = 1000.0f; // LFO frequency also known as RATE - set by setFreq function. Parameter value passed as argument
        mSampleRate = 44100.0f;
        mPulseWidth = 0.5f;
    }
    
void squareLFO::prepare(float sampleRate)
    {
        mSampleRate = sampleRate;
        calculateIncrement();
        
        
    }
    
void squareLFO::setFrequency (float frequencyHz)
    {
        mFrequency = frequencyHz;
        calculateIncrement();
        setPulseWidth(mPulseWidth);
    }


void squareLFO::setPulseWidth (float pulseWidth)
    {
        mPulseWidth = pulseWidth;
    }


float squareLFO::softClip (float waveValue)
{
    return std::tanh(waveValue * 10.0f);
}



    
float squareLFO::process() // Literally the LFO code
    {
        float square = (mPhase < mPulseWidth) ? 1.0f : -1.0f;
        
//        square = softClip(square);
        
        if (mPhase >= 1.0f)
        {
            mPhase -= 1.0f;
        }
        
        mPhase += mIncrement;
        
        // One-pole smoothing filter
        float smoothingCoeff = 0.9f;  // 0.9-0.99 range
        mLastOutput = smoothingCoeff * mLastOutput + (1.0f - smoothingCoeff) * square;
        
        return mLastOutput;
    }
    
    void squareLFO::reset()
    {
        mPhase = 0.0f;
    }
