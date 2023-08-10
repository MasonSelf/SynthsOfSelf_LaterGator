/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LaterGatorAudioProcessor::LaterGatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
, gator(apvts,
        "modulation",
        rateFreeParamID,
        rateSyncParamID,
        rateBlendParamID,
        resetParamID,
        freezeParamID,
        modAmpParamID,
        smoothingParamID),
apvts(*this, nullptr, juce::Identifier("audioProcesserValueTree"),
      {
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"later_left", 1}, "Later Left (link)", juce::NormalisableRange<float>(0.0f, 1.0f, 0.00001f), 0.00000f), //version hint in parameterID
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"later_right", 1}, "Later Right", juce::NormalisableRange<float>(0.0f, 1.0f, 0.00001f), 0.00000f), //version hint in parameterID
    std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"link", 1}, "Link", true),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{smoothingParamID.toString(), 1}, "Smoothing", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f), 
    std::make_unique<juce::AudioParameterBool>(juce::ParameterID{resetParamID.toString(), 1},
                                               "Reset",
                                               false),
    std::make_unique<juce::AudioParameterBool>(juce::ParameterID{freezeParamID.toString(), 1},
                                               "Freeze",
                                               false),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{rateFreeParamID.toString(), 1}, //version int in param id
                                                "Free Rate",                       //visible name
                                                juce::NormalisableRange<float>(0.0f, 40.0f, 0.00001f, 0.3f),
                                                1.0f), //skew factor
    std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{rateSyncParamID.toString(), 1},
                                                 "Sync Rate",
                                                 TempoSync::SyncStrings,
                                                 6),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{rateBlendParamID.toString(), 1}, //version int in param id
                                                "Rate Blend",                       //visible name
                                                juce::NormalisableRange<float>(0.0f, 1.0f, 0.00001f),
                                                0.9f),
    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{modAmpParamID.toString(), 1}, //version int in param id
                                                "Modulation Amplitude",                       //visible name
                                                juce::NormalisableRange<float>(0.0f, 1.0f, 0.00001f),
                                                1.0f),
}
      )

{
}

LaterGatorAudioProcessor::~LaterGatorAudioProcessor()
{
}

juce::AudioProcessorValueTreeState& LaterGatorAudioProcessor::GetAPVTS()
{
    return apvts;
}
const juce::Array<juce::AudioProcessorParameter*> & LaterGatorAudioProcessor::GetParams()
{
    return getParameters();
}

//==============================================================================
const juce::String LaterGatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LaterGatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LaterGatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LaterGatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LaterGatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LaterGatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LaterGatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LaterGatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LaterGatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void LaterGatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LaterGatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gator.PrepareToPlay(sampleRate, getTotalNumInputChannels(), samplesPerBlock);
}

void LaterGatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}


bool LaterGatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.

    // {1,2}, {2,2}
    if (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
    {
        if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono() ||
            layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo())
        {
            return true;
        }
    }
    return false;
}


void LaterGatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    //DBG(totalNumInputChannels);
    //auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto playHeadData = getPlayHead()->getPosition();
    if (playHeadData.hasValue())
    {
        gator.modProcessor.tempoData.ProcessTempoData(playHeadData);
    }
    
    gator.Process(buffer, midiMessages);
}

//==============================================================================
bool LaterGatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LaterGatorAudioProcessor::createEditor()
{
    return new LaterGatorAudioProcessorEditor (*this);
}

//==============================================================================
void LaterGatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //create output stream
    juce::MemoryOutputStream stream(destData, false);

    //save adjusters
    gator.SaveState(apvts);
    
    //write to stream
    apvts.state.writeToStream(stream);
}

void LaterGatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //LOAD STATE
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        gator.LoadState(tree);
        //restore rest of apvts (params)
        apvts.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LaterGatorAudioProcessor();
}
