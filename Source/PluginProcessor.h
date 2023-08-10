/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include "LaterGatorProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
/**
*/
class LaterGatorAudioProcessor  : public juce::AudioProcessor, public IAudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    LaterGatorAudioProcessor();
    ~LaterGatorAudioProcessor() override;

    juce::AudioProcessorValueTreeState& GetAPVTS() override;
    const juce::Array<juce::AudioProcessorParameter*> & GetParams() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    const juce::Identifier leftDelayID {"later_left"};
    const juce::Identifier rightDelayID {"later_right"};
    const juce::Identifier linkID {"link"};
    const juce::Identifier smoothingParamID {"smoothing"};

    //identifiers pertaining to modulator
    const juce::Identifier resetParamID {"resetOrTriggerParamID"};
    const juce::Identifier freezeParamID {"freezeParamID"};
    const juce::Identifier rateFreeParamID {"rateFreeParamID"};
    const juce::Identifier rateSyncParamID {"rateSyncParamID"};
    const juce::Identifier rateBlendParamID {"rateBlendParamID"};
    const juce::Identifier modAmpParamID {"modAmpParamID"};

    LaterGatorProcessor gator;
    juce::AudioProcessorValueTreeState apvts;
private:
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaterGatorAudioProcessor)
};
