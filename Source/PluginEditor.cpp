/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveTableGeneratorAudioProcessorEditor::WaveTableGeneratorAudioProcessorEditor (WaveTableGeneratorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)


{

    addAndMakeVisible(textLabel);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(wavesMenu);
    addAndMakeVisible(audioProcessor.waveVisualiser);

    // Titulo
    //textLabel.setFont(Font(16.0f, Font::bold));
    textLabel.setText("S.C.S", dontSendNotification);
    textLabel.setColour(Label::textColourId, Colours::red);
    textLabel.setJustificationType(Justification::centred);

    // Gain Slider
    gainSlider.setRange(-80, 0, 0.01);
    gainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    gainSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::green));
    gainSlider.setValue(-80);
    gainLabel.setText("Gain", dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, true);

    gainSlider.onValueChange = [this]
    {
        float decJ = Decibels::decibelsToGain(gainSlider.getValue());
        audioProcessor.gain = decJ;
    };

    // Frequency Fader
    frequencySlider.setRange(1, 20000, 0.01);
    frequencySlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    frequencySlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    frequencySlider.setTextValueSuffix("Hz");
    frequencySlider.setValue(80);
    freqLabel.setText("Freq", dontSendNotification);
    freqLabel.attachToComponent(&frequencySlider, true);
    

    frequencySlider.onValueChange = [this]
    {
        float freqJ = frequencySlider.getValue();
        audioProcessor.freq = freqJ;
    };

    // WavesMenu
    wavesMenu.addItem("None", 1);
    wavesMenu.addItem("Sinusoid", 2);
    wavesMenu.addItem("Square", 3);
    wavesMenu.addItem("Sawtooth", 4);
    wavesMenu.addItem("Triangle", 5);
    wavesMenu.addItem("Voice Modulation", 6); 
    wavesMenu.setSelectedId(2);
    //wavesMenu.setJustificationType(Justification::centred);
    

    wavesMenu.onChange = [this]
    { 
        audioProcessor.waveType = wavesMenu.getSelectedItemIndex();
    };

    // Dimensiones 
    setSize(400, 300);

    textLabel.setBounds(265, 265, 200, 50);
    wavesMenu.setBounds(20, 10, 360, 20);
    gainSlider.setBounds(50, 50, 340, 20);
    frequencySlider.setBounds(50, 80, 340, 20); 
    audioProcessor.waveVisualiser.setBounds(20, 120, 360, 160); // Grafica
 
}


WaveTableGeneratorAudioProcessorEditor::~WaveTableGeneratorAudioProcessorEditor()
{
}

//==============================================================================
void WaveTableGeneratorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    /*g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);*/
}

void WaveTableGeneratorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
