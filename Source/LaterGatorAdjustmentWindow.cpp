/*
  ==============================================================================

    LaterGatorAdjustmentWindow.cpp
    Created: 17 May 2023 9:16:53pm
    Author:  Mason Self

  ==============================================================================
*/

#include "LaterGatorAdjustmentWindow.h"

LaterGatorAdjustmentWindow::LaterGatorAdjustmentWindow(LaterGatorAudioProcessor& p, juce::Colour _backgroundColor = juce::Colours::lightgrey)
: backgroundColor(_backgroundColor),
    leftDelayCurveAdjuster(0,
                           "later_left",
                           true,
                           false,
                           true,
                           adjusterComponentWidth,
                           adjusterComponentWidth,
                           adjusterWidth,
                           adjusterWidth,
                           p,
                           p.gator.lCurve,
                           true,
                           "left delay (link)",
                           "0s",
                           "1.5s"),
    rightDelayCurveAdjuster(1,
                            "later_right",
                            true,
                            false,
                            true,
                            adjusterComponentWidth,
                            adjusterComponentWidth,
                            adjusterWidth,
                            adjusterWidth,
                            p,
                            p.gator.rCurve,
                            true,
                            "right delay",
                            "0s",
                            "1.5s")
{
    setInterceptsMouseClicks(false, true); //no clicks on this, but children are ok
    addAndMakeVisible(leftDelayCurveAdjuster);
    addAndMakeVisible(rightDelayCurveAdjuster);
}

void LaterGatorAdjustmentWindow::paint(juce::Graphics& g)
{
    g.setColour(backgroundColor);
    g.setOpacity(0.5f);
    g.fillAll();
}

void LaterGatorAdjustmentWindow::resized()
{
    auto spaceBetweenAdjusters = 15;
    leftDelayCurveAdjuster.setBounds(30, spaceBetweenAdjusters, adjusterComponentWidth, adjusterComponentWidth);
    rightDelayCurveAdjuster.setBounds(leftDelayCurveAdjuster.getRight() + spaceBetweenAdjusters, leftDelayCurveAdjuster.getY(), adjusterComponentWidth, adjusterComponentWidth);
}


