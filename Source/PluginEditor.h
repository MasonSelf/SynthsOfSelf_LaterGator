/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "GatorSlider.h"
#include "LaterGatorAdjustmentWindow.h"
#include "PluginProcessor.h"
#include <sos_drop_down_window/sos_drop_down_window.h>


//==============================================================================
/**
*/
class LaterGatorAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Button::Listener
{
public:
    LaterGatorAudioProcessorEditor (LaterGatorAudioProcessor&);
    ~LaterGatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button*) override;
    void buttonStateChanged (juce::Button* b) override;

private:
    LaterGatorAudioProcessor& audioProcessor;

    juce::Colour gatorGreen  {92, 158, 49};
    juce::Colour gatorLightGreen {166, 198, 47};
    juce::Colour opaquePink {255,180,217};
    juce::Colour brown {134, 97, 11};
    GatorSlider lDelaySlider;
    GatorSlider rDelaySlider;
    GatorSlider linked_rDelaySlider; //replaces regular rDelaySlider when linked
    SOSToggleButton linkToggle;

    const int defaultWindowHeight {300};

    const int dropDownWindowHeight {200};
    const int tabWidth {60};
    const int tabHeight {30};
    LaterGatorAdjustmentWindow adjustWindow;
    LFO_modEditor modulationWindow;
    DropDownTab modDropTab;
    DropDownTab adjustDropTab;


    enum PopUpWindowOrder {adjustOnTop, modulationOnTop};
    PopUpWindowOrder popUpWindowOrder;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaterGatorAudioProcessorEditor)
};
