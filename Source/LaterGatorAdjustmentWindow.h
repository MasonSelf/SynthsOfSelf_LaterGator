/*
  ==============================================================================

    LaterGatorAdjustmentWindow.h
    Created: 17 May 2023 9:16:53pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"
#include <sos_curve_adjuster/sos_curve_adjuster.h>

class LaterGatorAdjustmentWindow : public juce::Component
{
public:
    LaterGatorAdjustmentWindow(LaterGatorAudioProcessor& p, juce::Colour _backgroundColor);

    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    const float adjusterWidth{140.0f};
    const float adjusterComponentWidth{180.0f};
    juce::Colour backgroundColor;

public:
    CurveAdjusterComponent leftDelayCurveAdjuster;
    CurveAdjusterComponent rightDelayCurveAdjuster;

};
