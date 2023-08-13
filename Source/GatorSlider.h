/*
  ==============================================================================

    GatorSlider.h
    Created: 17 May 2023 11:18:06am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include "binaryData.h"
#include <sos_sliders/sos_sliders.h>

class GatorSlider : public SOSSliderHorizontal, public juce::Timer
{
public:
    
    GatorSlider(LaterGatorAudioProcessor& p, const juce::Identifier& paramID ,int _paramIndex)
    : SOSSliderHorizontal(p, paramID, _paramIndex), processor(p), paramIndex(_paramIndex)
    {
        trackColor = juce::Colours::lightgrey;
        trackFillColor = juce::Colours::hotpink;
        startTimer(30);
    }

    ~GatorSlider()
    {
        stopTimer();
    }

    void paint(juce::Graphics& g) override
    {
        auto halfImageHandleWidth = gatorImage.getWidth() / 2.0f;
        juce::Point<float> trackTopLeft {halfImageHandleWidth, getHeight() - trackWidth + 2};
        juce::Point<float> trackBottomRight {static_cast<float>(getWidth() - halfImageHandleWidth), static_cast<float>(getHeight() + 2)};
        //track
        juce::Rectangle<float> trackRectangle {trackTopLeft, trackBottomRight};
        g.setColour(trackColor);
        g.fillRoundedRectangle(trackRectangle, amountRounded);
        
        //track fill
        auto handleX = GetTrackFillPos();
        auto trackFillRectangle = trackRectangle.withRight(handleX + halfImageHandleWidth);
        g.setColour(trackFillColor);

        g.fillRoundedRectangle(trackFillRectangle, amountRounded);
        
        //handle
        //TODO: make image its own class if its the only thing that needs to be redrawn under modulation
        g.drawImageAt(gatorImage, GetModdedHandlePos(), 0);
    }

    void timerCallback() override
    {
        repaint();
    }
    
    float GetTrackFillPos()
    {
        auto multiplier = (static_cast<float>(getWidth()) - gatorImage.getWidth())  / static_cast<float>(getWidth());
        auto normalizedValue = static_cast<float>(getValue()) / getRange().getEnd();
        return normalizedValue * getWidth() * multiplier;
    }

    float GetModdedHandlePos()
    {
        float moddedParamVal;
        if (paramIndex == 0)
        {
            moddedParamVal = processor.gator.lCurve.inputX.value.load();
        }
        else if (paramIndex == 1)
        {
            moddedParamVal = processor.gator.rCurve.inputX.value.load();
        }
        auto multiplier = (static_cast<float>(getWidth()) - gatorImage.getWidth())  / static_cast<float>(getWidth());
        auto normalizedValue = static_cast<float>(moddedParamVal) / getRange().getEnd();
        return normalizedValue * getWidth() * multiplier;
    }

    juce::Image gatorImage {juce::ImageCache::getFromMemory(BinaryData::crocodile_png, 4354)};
    LaterGatorAudioProcessor& processor;
    int paramIndex;
};
