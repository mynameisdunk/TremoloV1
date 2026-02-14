/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
 
 This is a test bit of text to check I've opened the correct file ( you have )
 
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DelayLine.h"
#include "BBD.h"
#include "Compander.h"

//==============================================================================
TremoloV1AudioProcessor::TremoloV1AudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                      ), params(apvts)
{
   
}

TremoloV1AudioProcessor::~TremoloV1AudioProcessor()
{
    
    
}

//==============================================================================
const juce::String TremoloV1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TremoloV1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TremoloV1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TremoloV1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TremoloV1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TremoloV1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TremoloV1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void TremoloV1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TremoloV1AudioProcessor::getProgramName (int index)
{
    return {};
}

void TremoloV1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

juce::AudioProcessorParameter* TremoloV1AudioProcessor::getBypassParameter() const
{
return params.bypassParam;
}

//==============================================================================
void TremoloV1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 2;
    
    sinLFO.prepare(sampleRate);
    sinLFO.reset();
    
    triLFO.prepare(sampleRate);
    sinLFO.reset();
    
// COMPANDER
    mEnvelopeFollower.prepare(sampleRate);
    mEnvelopeFollower.reset();
    mCompressor.prepare(sampleRate);
    mCompressor.reset();
    mExpander.prepare(sampleRate);
    mExpander.reset();
    
    rateParamInHz = 1.0f;
// FILTERS
    
    
    
}

void TremoloV1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TremoloV1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void TremoloV1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    rateParamInHz = params.rate;
    sinLFO.setFrequency(rateParamInHz);
    triLFO.setFrequency(rateParamInHz);
    
    float sampleRate = float(getSampleRate());
    
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    
    scaledDepth = params.depth/ (1 + params.rate * k);
    
    params.update();
    params.smoothen();
    
    
    for (int samp = 0; samp < buffer.getNumSamples(); ++samp )
        {
            //   ============  Sine LFO - Don't delete will adapt for use later ====
            lfoValue = sinLFO.process(); // LFO position -1 to 1
            
            lfoValue = triLFO.process(); // LFO position -1 to 1
            
            float gain = lfoValue;
            
            float dryL = channelDataL[samp];
            float dryR = channelDataR[samp];
            
            float compDryL = mCompressor.process(dryL);
            float compDryR = mCompressor.process(dryR);
            
            float wetL = dryL * (params.depth * gain);
            float wetR = dryR * (params.depth * gain);
            
            wetL = mExpander.process(wetL);
            wetR = mExpander.process(wetR);
            
            float mixL = wetL;
            float mixR = wetR;
            
            if (!params.bypassed)
            {
                mixL = dryL;
                mixR = dryR;
            }
            
            channelDataL[samp] =  mixL * params.gain;
            channelDataR[samp] =  mixR * params.gain;
            
            
            
            
        }
}

//==============================================================================
bool TremoloV1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TremoloV1AudioProcessor::createEditor()
{
    return new TremoloV1AudioProcessorEditor (*this);
}

//==============================================================================
void TremoloV1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void TremoloV1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TremoloV1AudioProcessor();
}

