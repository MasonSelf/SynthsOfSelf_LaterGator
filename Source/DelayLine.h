/*
  ==============================================================================

    DelayLine.h
    Created: 16 May 2023 10:48:23am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <sos_maths/sos_maths.h>

/*
 This class adds curvature to otherwise linear ramps
 */

class DoubleSmoothedValue
{
public:
    DoubleSmoothedValue(float manualSmoothing)
    : manualSmoothingDefault(manualSmoothing)
    {}
    void Init(double sampleRate)
    {
        smoothedValue.reset(sampleRate, 0.2);
        smoothedValue.setCurrentAndTargetValue(0.0f);
        manualSmoothingDefault *= 44100.0 / sampleRate; //ensures same smoothing rate regardless of sampleRate

    }
    
    float GetNextValue(float extraSmoothing)
    {
        auto tempTarget = smoothedValue.getNextValue();
        if (juce::approximatelyEqual(manuallySmoothedVal, tempTarget))
        {
            return manuallySmoothedVal;
        }
        
        auto diff = tempTarget - manuallySmoothedVal;
        manuallySmoothedVal += (diff * GetExtraSmoothingMultiplier(extraSmoothing));
        return manuallySmoothedVal;
    }
    
    float GetNextValue()
    {
        auto tempTarget = smoothedValue.getNextValue();
        if (juce::approximatelyEqual(manuallySmoothedVal, tempTarget))
        {
            return manuallySmoothedVal;
        }
        
        auto diff = tempTarget - manuallySmoothedVal;
        manuallySmoothedVal += (diff * manualSmoothingDefault);
        return manuallySmoothedVal;
    }
    
    void SetTargetValue(float target)
    {
        auto currentTarget = smoothedValue.getTargetValue();
        if (currentTarget != target)
        {
            smoothedValue.setTargetValue(target);
        }
    }
private:
    float GetExtraSmoothingMultiplier(float paramInput)
    {
        //TODO: slightly more smooth max
        //TODO: more resolution on smoother values
        //input is 0.0f to 1.0f
        //target range? 0.001f none -> 0.00001f max?
        paramInput = (1.0f - paramInput) * 1.2f + 0.01f; //invert, then scale down, offset to avoid multiplying by 0
        //DBG(paramInput);
        return manualSmoothingDefault * paramInput;
    }
    
    
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedValue;
    float manuallySmoothedVal {0.0f};
    float manualSmoothingDefault;

};

class DelayBuffer
{
public:
    void Init (double sampleRate);
    void PushSample (float& sample);
    
    /*process with typical control over dryWet and feedback*/
    void Process(float& sample, float delayAmount, float dryWet, float feedback, float smoothParam);
    
    /*process with out param as only the delayed sample (100% wet, 0% feedback)*/
    void Process(float& sample, float delayAmount, float smoothParam);
    
private:
    inline int Modulo(int a, int b)
    {
        const int result = a % b;
        return result >= 0 ? result : result + b;
    }
    void GetDelayedSample (float& sample, float delayAmount, float smoothParam);
    float LinInterp(float sample_x, float sample_x1, float inPhase);
    void UpdateSmoothedDelayTime(float delayAmount);
    
    std::vector<float> buffer;
    int writePos{-1};
    int bufferLen{0};
    float previousSample {0.0f};
    
    DoubleSmoothedValue smoothedDelayTime{0.001f};
    
};
