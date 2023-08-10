/*
  ==============================================================================

    LaterGatorProcessor.cpp
    Created: 16 May 2023 10:54:57am
    Author:  Mason Self

  ==============================================================================
*/

#include "LaterGatorProcessor.h"

LaterGatorProcessor::LaterGatorProcessor(juce::AudioProcessorValueTreeState& tree,
                                         std::string modulatorName,
                                         const juce::Identifier& rateFreeParamName,
                                         const juce::Identifier& rateSyncParamName,
                                         const juce::Identifier& rateBlendParamName,
                                         const juce::Identifier& resetParamName,
                                         const juce::Identifier& freezeParamName,
                                         const juce::Identifier& scaleParamName,
                                         const juce::Identifier& _smoothParamName)
:   lCurve("lCurve", 0.0f, 0.2, {{{0.0f, 0.0f}, {0.5f, 0.0f}, {1.0f, 1.0f}}} ),
    rCurve("rCurve", 0.0f, 0.2,{{{0.0f, 0.0f}, {0.5f, 0.0f}, {1.0f, 1.0f}}} ),
    modProcessor(modulatorName,
                tree,
                rateFreeParamName,
                rateSyncParamName,
                rateBlendParamName,
                resetParamName,
                freezeParamName,
                scaleParamName,
                {"left delay", "right delay"}),
    apvts(tree),
smoothParamID(_smoothParamName)
{
}

void LaterGatorProcessor::PrepareToPlay(double _sampleRate, int _numInputChans, int samplesPerBlock)
{
    numInputChans = _numInputChans;
    //allocate copyBuffer
    if (numInputChans == 1)
    {
        audioBufferCopyForMonoInput.setSize(1, samplesPerBlock);
    }

    sampleRate = _sampleRate;
    crossfadeGainL.reset(sampleRate, 0.002);
    crossfadeGainR.reset(sampleRate, 0.002);
    lCurve.smoothedVal.Reset(sampleRate);
    rCurve.smoothedVal.Reset(sampleRate);
    
    lDelay.Init(sampleRate);
    rDelay.Init(sampleRate);
    modProcessor.Init(_sampleRate);
}

void LaterGatorProcessor::Process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    for (auto i = numInputChans; i < numOutputChans; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //assign write pointers
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel; //to be assigned immediately!


    if (numInputChans == 1)
    {
        audioBufferCopyForMonoInput.copyFrom(0, 0, buffer.getReadPointer(0), buffer.getNumSamples());
        rightChannel = audioBufferCopyForMonoInput.getWritePointer(0);
    }
    else if (numInputChans == 2)
    {
        rightChannel = buffer.getWritePointer(1);
    }
    else jassertfalse;

    modProcessor.Process(buffer.getNumSamples());

    //translate delay times to samples
    auto translatedLTime = lCurve.GetTranslatedOutput(modProcessor.GetModdedVal(leftDelayID, "left delay"));
    auto translatedRTime = rCurve.GetTranslatedOutput(modProcessor.GetModdedVal(rightDelayID, "right delay"));
    Translate_0_1_ToSamples(translatedLTime);
    Translate_0_1_ToSamples(translatedRTime);

    //iterate through buffer
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        //push new samples
        lDelay.PushSample(leftChannel[i]);
        rDelay.PushSample(rightChannel[i]);
        
        //link r to l if should link
        if (*apvts.getRawParameterValue(linkID))
        {
            translatedRTime = translatedLTime;
        }
        
        HandlePossibleCrossfade(leftChannel[i], rightChannel[i], translatedLTime, translatedRTime);
    }
    //copy the AudioBufferCopyForMonoInput to the right output if necessary
    if (numInputChans == 1)
    {
        buffer.copyFrom(1, 0, audioBufferCopyForMonoInput.getReadPointer(0), buffer.getNumSamples());
    }
}

void LaterGatorProcessor::SaveState(juce::AudioProcessorValueTreeState& apvts)
{
    //save adjusters
    lCurve.SaveState(apvts);
    rCurve.SaveState(apvts);

    //save modulator state
    modProcessor.SaveState(apvts);
}

void LaterGatorProcessor::LoadState(juce::ValueTree& tree)
{
    lCurve.LoadAndRemoveStateFromAPTVS(tree);
    rCurve.LoadAndRemoveStateFromAPTVS(tree);

    modProcessor.LoadAndRemoveStateFromAPTVS(tree);
}


void LaterGatorProcessor::Translate_0_1_ToSamples(float& input)
{
    input *= sampleRate * 1.5f;
}

void LaterGatorProcessor::HandlePossibleCrossfade(float& l, float& r, float lDelayTime, float rDelayTime)
{
    float lDelayed = l;
    float rDelayed = r;
    lDelay.Process(lDelayed, lDelayTime, *apvts.getRawParameterValue(smoothParamID));
    rDelay.Process(rDelayed, rDelayTime, *apvts.getRawParameterValue(smoothParamID));
    
    //left
    if (lDelayTime == 0.0f && crossfadeGainL.getTargetValue() != 0.0f)
    {
        crossfadeGainL.setTargetValue(0.0f);
    }
    else if (lDelayTime != 0.0f && crossfadeGainL.getTargetValue() != 1.0f)
    {
        crossfadeGainL.setTargetValue(1.0f);
    }
    auto gainL = crossfadeGainL.getNextValue();
    l = l * (1.0f - gainL) + lDelayed * gainL;

    //right
    if (rDelayTime == 0.0f && crossfadeGainR.getTargetValue() != 0.0f)
    {
        crossfadeGainR.setTargetValue(0.0f);
    }
    else if (rDelayTime != 0.0f && crossfadeGainR.getTargetValue() != 1.0f)
    {
        crossfadeGainR.setTargetValue(1.0f);
    }
    auto gainR = crossfadeGainR.getNextValue();
    r = r * (1.0f - gainR) + rDelayed * gainR;
}
