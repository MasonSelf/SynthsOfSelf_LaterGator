/*
  ==============================================================================

    DelayLine.h
    Created: 16 May 2023 10:48:23am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>


//adaptation from stereo delay buffer from Tlacael Esparza

class DelayBuffer
{
public:
    void Init (double sampleRate);
    void PushSample (float sample);
    
    /*process with typical control over dryWet and feedback*/
    void Process(float& sample, float delayAmount, float dryWet, float feedback);
    
    /*process with out param as only the delayed sample (100% wet, 0% feedback)*/
    void Process(float& sample, float delayAmount);
    
private:
    inline int Modulo(int a, int b)
    {
        const int result = a % b;
        return result >= 0 ? result : result + b;
    }
    void GetDelayedSample (float& sample, float delayAmount);
    float LinInterp(float sample_x, float sample_x1, float inPhase);
    void UpdateSmoothedDelayTime(float delayAmount);
    
    std::vector<float> buffer;
    int writePos{-1};
    int bufferLen{0};
    float previousSample {0.0f};
    
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDelayTime;
};
