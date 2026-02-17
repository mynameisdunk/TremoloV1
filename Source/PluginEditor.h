/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"
//==============================================================================
/**
*/
class TremoloV1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TremoloV1AudioProcessorEditor (TremoloV1AudioProcessor&);
    ~TremoloV1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TremoloV1AudioProcessor& audioProcessor;

    RotaryKnob mixKnob {"MIX", audioProcessor.apvts, mixParamID, false, false};
    RotaryKnob rateKnob {"RATE", audioProcessor.apvts, rateParamID, false, false};
    RotaryKnob depthKnob {"DEPTH", audioProcessor.apvts, depthParamID, false, false};
    RotaryKnob outputGainKnob {"OUTPUT", audioProcessor.apvts, outputGainParamID, true, false};
    RotaryKnob waveKnob {"WAVE", audioProcessor.apvts, waveParamID, false, false};
    RotaryKnob pulseWidthKnob {"PW", audioProcessor.apvts, pulseWidthParamID, false, false};
    
    juce::GroupComponent paramGroup;
    
    juce::DropShadow edgeBlur {juce::Colours::black, 6, {0, 0}};
    
    juce::TextButton bypassButton;
    
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment {audioProcessor.apvts, bypassParamID.getParamID(), bypassButton};
    
    PedalButtonLookAndFeel pedalButtonLookAndFeel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoloV1AudioProcessorEditor)
};
