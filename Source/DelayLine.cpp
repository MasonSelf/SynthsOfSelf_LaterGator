/*
  ==============================================================================

    DelayLine.cpp
    Created: 16 May 2023 10:48:23am
    Author:  Mason Self

  ==============================================================================
*/

#include "DelayLine.h"

void DelayBuffer::Init (double sampleRate)
{
    bufferLen = sampleRate * 2.0;
    buffer.resize (bufferLen);
    smoothedDelayTime.reset(sampleRate, 0.2);
}

void DelayBuffer::PushSample (float sample)
{
    writePos++;
    writePos %= bufferLen;
    buffer[writePos] = sample;
}

/*process with typical control over dryWet and feedback*/
void DelayBuffer::Process(float& sample, float delayAmount, float dryWet, float feedback)
{
    //get delayed sample
    auto delayedSample = sample;
    GetDelayedSample(delayedSample, delayAmount);
    
    //            dry sample                  wet sample                  feedback
    sample = sample * (1.0f - dryWet) + delayedSample * dryWet + previousSample * feedback;
    
    //update previous sample
    previousSample = delayedSample;
}

/*process with out param as ONLY the delayed sample (100% wet, 0% feedback)*/
void DelayBuffer::Process(float& sample, float delayAmount)
{
    GetDelayedSample(sample, delayAmount);
}

void DelayBuffer::GetDelayedSample (float& sample, float delayAmount)
{
    jassert (delayAmount <= bufferLen);
    
    UpdateSmoothedDelayTime(delayAmount);
    auto newSmoothedDelayTime = smoothedDelayTime.getNextValue();
    
    // add buffer len and mod to wrap correctly on circular buffer
    int readIndexA = Modulo (writePos - static_cast<int>(newSmoothedDelayTime),  bufferLen);
    // need to make sure we wrap around if adding 1 goes over the boundary
    int readIndexB = (readIndexA + 1) % bufferLen;
    
    float frac = 1.0f - (newSmoothedDelayTime - static_cast<int>(newSmoothedDelayTime));
    
    sample = LinInterp(buffer[readIndexA], buffer[readIndexB], frac);
}


float DelayBuffer::LinInterp(float sample_x, float sample_x1, float inPhase)
{
    return (1.0f - inPhase) * sample_x + inPhase * sample_x1;
}

void DelayBuffer::UpdateSmoothedDelayTime(float delayAmount)
{
    if (delayAmount != smoothedDelayTime.getTargetValue())
    {
        smoothedDelayTime.setTargetValue(delayAmount);
    }
}
