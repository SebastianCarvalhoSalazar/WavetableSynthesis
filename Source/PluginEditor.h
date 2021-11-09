/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
using namespace juce;

//==============================================================================
/**
*/
class WaveTableGeneratorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WaveTableGeneratorAudioProcessorEditor (WaveTableGeneratorAudioProcessor&);
    ~WaveTableGeneratorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    ComboBox wavesMenu;

    Slider gainSlider, frequencySlider;

    Label freqLabel, gainLabel, textLabel;

    WaveTableGeneratorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveTableGeneratorAudioProcessorEditor)
};


