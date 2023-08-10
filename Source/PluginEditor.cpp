/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LaterGatorAudioProcessorEditor::LaterGatorAudioProcessorEditor (LaterGatorAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      lDelaySlider(p, p.leftDelayID, 0),
      rDelaySlider(p, p.rightDelayID , 1),
      linked_rDelaySlider(p, p.leftDelayID , 0), //note this is attached to the same param as lDelay
      linkToggle(p, p.linkID, 2, "Link"),
      smoothingBoxFillSlider(p, p.smoothingParamID, 3, "Smooth", juce::Colours::lightgrey, juce::Colours::yellow, juce::Colours::black, juce::Colours::slategrey, 15.0f, buttonWidth),
      adjustWindow(p, juce::Colours::hotpink),
      modulationWindow( p,
                        p.gator.modProcessor,
                        120.0f,
                        120.0f,
                        4,
                        5,
                        6,
                        7,
                        8,
                        9,
                        {"mod output"},
                        p.gator.modProcessor.modMatrix.destinationNames,
                        juce::Colours::black,
                        brown,
                        gatorGreen,
                        juce::Colours::lightgrey,
                        opaquePink,
                        gatorLightGreen),
      modDropTab(gatorLightGreen, juce::Colours::black, "Modulation", 2.0f, 12.0f, 0.5f),
      adjustDropTab(juce::Colours::hotpink, juce::Colours::black, "Adjust", 2.0f, 12.0f, 0.5f)

{
    setSize (400, defaultWindowHeight);
    addAndMakeVisible(lDelaySlider);
    addAndMakeVisible(linkToggle);
    addAndMakeVisible(smoothingBoxFillSlider);

    linkToggle.addListener(this);
    if (linkToggle.getToggleState())
    {
        addAndMakeVisible(linked_rDelaySlider);
    }
    else
    {
        addAndMakeVisible(rDelaySlider);
    }

    
    //addAndMakeVisible(adjustWindow);
    //addAndMakeVisible(modulationWindow);
//    modulationWindow.tab.addListener(this);
//    modulationWindow.popUpWindow.closeBox.addListener(this);

    addAndMakeVisible(modDropTab);
    modDropTab.addListener(this);

    addAndMakeVisible(adjustDropTab);
    adjustDropTab.addListener(this);
}

LaterGatorAudioProcessorEditor::~LaterGatorAudioProcessorEditor()
{
    linkToggle.removeListener(this);
//    adjustWindow.tab.removeListener(this);
//    adjustWindow.popUpWindow.closeBox.removeListener(this);
    modDropTab.removeListener(this);
    adjustDropTab.removeListener(this);
    //modulationWindow.tab.removeListener(this);
    //modulationWindow.popUpWindow.closeBox.removeListener(this);
}

//==============================================================================
void LaterGatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::white);
    g.setOpacity(1.0f);
    g.setColour (juce::Colours::black);
    g.setFont (35.0f);
    g.drawFittedText ("Synths of Self", getLocalBounds().withBottom(30).withX(10), juce::Justification::centredLeft, 1);
    g.setColour(gatorGreen);
    g.drawFittedText("Later Gator", getLocalBounds().withBottom(30).withX(105), juce::Justification::centred, 1);
    g.setFont(12.0f);
    g.setColour(juce::Colours::black);
    g.drawMultiLineText("version 1.0.2 \nwww.synthsofself.com", getWidth() / 2 + 80, defaultWindowHeight - 16,120, juce::Justification::centredLeft);
}

void LaterGatorAudioProcessorEditor::resized()
{
    const auto sliderHeight = 28;
    const auto sideMargin = 10;
    const auto sliderWidth = getWidth() - sideMargin * 2;
    lDelaySlider.setBounds(sideMargin, defaultWindowHeight/ 4, sliderWidth, sliderHeight);
    rDelaySlider.setBounds(sideMargin, defaultWindowHeight / 4 * 3 - 30, sliderWidth, sliderHeight);
    linked_rDelaySlider.setBounds(rDelaySlider.getBounds());
    
    const auto buttonHeight = 24;
    linkToggle.setBounds(getWidth() / 3 - buttonWidth / 2, defaultWindowHeight / 2 - buttonHeight / 2, buttonWidth, buttonHeight);
    smoothingBoxFillSlider.setBounds(getWidth() / 3 * 2 - buttonWidth / 2, defaultWindowHeight / 2 - buttonHeight / 2, buttonWidth, buttonHeight);

    adjustDropTab.setBounds(sideMargin, defaultWindowHeight - tabHeight, tabWidth, tabHeight);
    modDropTab.setBounds(adjustDropTab.getRight() + sideMargin, defaultWindowHeight - tabHeight, tabWidth * 1.4f, tabHeight);

    adjustWindow.setBounds(0, defaultWindowHeight, getWidth(), dropDownWindowHeight);
    modulationWindow.setBounds(0, defaultWindowHeight, getWidth(), dropDownWindowHeight);
}

void LaterGatorAudioProcessorEditor::buttonClicked (juce::Button* b)
{
    if (b == &modDropTab)
    {
        if (modDropTab.getToggleState())
        {
            if (adjustDropTab.getToggleState())
            {
                removeChildComponent(&adjustWindow);
                adjustDropTab.setToggleState(false, juce::dontSendNotification);
            }
            else
            {
                setSize(getWidth(), defaultWindowHeight + dropDownWindowHeight);
            }
            addAndMakeVisible(modulationWindow);
        }
        else
        {
            removeChildComponent(&modulationWindow);
            setSize(getWidth(), defaultWindowHeight);
        }
    }
    else if (b == &adjustDropTab)
    {
        if (adjustDropTab.getToggleState())
        {
            if (modDropTab.getToggleState())
            {
                removeChildComponent(&modulationWindow);
                modDropTab.setToggleState(false, juce::dontSendNotification);
            }
            else
            {
                setSize(getWidth(), defaultWindowHeight + dropDownWindowHeight);
            }
            addAndMakeVisible(adjustWindow);
        }
        else
        {
            removeChildComponent(&adjustWindow);
            setSize(getWidth(), defaultWindowHeight);
        }
    }
}

void LaterGatorAudioProcessorEditor::buttonStateChanged (juce::Button* b)
{
    if (b == &linkToggle && b->getToggleState() != linkToggle.cachedToggleState)
    {
        linkToggle.cachedToggleState = b->getToggleState();
        if (linkToggle.cachedToggleState) {
            removeChildComponent(&rDelaySlider);
            addAndMakeVisible(linked_rDelaySlider);
        }
        else
        {
            removeChildComponent(&linked_rDelaySlider);
            addAndMakeVisible(rDelaySlider);
        }
    }
}

