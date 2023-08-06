/*
  ==============================================================================

    LaterGatorProcessor.h
    Created: 16 May 2023 10:54:57am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once

#include "DelayLine.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <sos_curve_adjuster/sos_curve_adjuster.h>
#include <sos_modulator/sos_modulator.h>

class LaterGatorProcessor
{
public:

    LaterGatorProcessor(juce::AudioProcessorValueTreeState& tree,
                        std::string modulatorName,
                        const juce::Identifier& rateFreeParamName,
                        const juce::Identifier& rateSyncParamName,
                        const juce::Identifier& rateBlendParamName,
                        const juce::Identifier& resetParamName,
                        const juce::Identifier& freezeParamName,
                        const juce::Identifier& scaleParamName);
    void PrepareToPlay(double _sampleRate, int _numInputChans, int samplesPerBlock);
    void Process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void SaveState(juce::AudioProcessorValueTreeState& apvts);
    void LoadState(juce::ValueTree& tree);


    CurveAdjuster::CurveAdjusterProcessor lCurve, rCurve;
    DelayBuffer lDelay, rDelay;

    LFO_modProcessor modProcessor;

    juce::AudioProcessorValueTreeState& apvts;
private:
    void Translate_0_1_ToSamples(float& input);
    void HandlePossibleCrossfade(float& sample, float& r, float lDelayTime, float rDelayTime);
    
    double sampleRate{44100.0};
    const int numOutputChans{2}; //marked const because Later Gator requires stereo output
    int numInputChans {1}; //this is set in PrepareToPlay. Accept 1 - 2 input channels
    juce::AudioBuffer<float> audioBufferCopyForMonoInput;

    
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> crossfadeGainL, crossfadeGainR;
    
    const juce::Identifier leftDelayID {"later_left"};
    const juce::Identifier rightDelayID {"later_right"};
    const juce::Identifier linkID {"link"};


    
};
