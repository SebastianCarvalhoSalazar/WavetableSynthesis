/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveTableGeneratorAudioProcessor::WaveTableGeneratorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    sampleRate = 44100;
    freq = 1;
    phase = 0;
    wtSize = 512;
    gain = 0;
    increment = 0;
    waveType = 0;

    // Sine Wave
    for (auto i = 0; i < wtSize; i++) { sineWaveTable.insert(i, sin(2.0 * double_Pi * i / wtSize)); }

    // Square Wave
    for (auto i = 0; i < (wtSize / 2); i++) { squareWaveTable.insert(i, 1); }
    for (auto i = (wtSize / 2); i < wtSize; i++) { squareWaveTable.insert(i, -1); }

    // fillsaw wave
    for (auto i = 0; i < wtSize; i++){ fillSawWaveTable.insert(i, 1 - (2 * (float)i / (float)wtSize)); }

    // Triangle Wave
    for (auto i = 0; i < wtSize / 2; i++) { triangleWaveTable.insert(i, 2 * (float)i / (float)(wtSize / 2) - 1 ); }
    for (auto i = wtSize / 2; i < wtSize; i++) { triangleWaveTable.insert(i, 1 - (2 * (float)(i - wtSize / 2) / (float)(wtSize / 2))); }

    waveVisualiser.clear();
}

WaveTableGeneratorAudioProcessor::~WaveTableGeneratorAudioProcessor()
{
}

//==============================================================================


const juce::String WaveTableGeneratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveTableGeneratorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveTableGeneratorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveTableGeneratorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveTableGeneratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveTableGeneratorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveTableGeneratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveTableGeneratorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WaveTableGeneratorAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveTableGeneratorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WaveTableGeneratorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WaveTableGeneratorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveTableGeneratorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WaveTableGeneratorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    increment = freq * (wtSize / sampleRate);

    switch (waveType)
    {
    case 1: 
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            for (auto channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelN = buffer.getWritePointer(channel);
                channelN[sample] = sineWaveTable[(int)phase] * gain;
                phase = fmod((phase + increment/2), wtSize);
            }
        }
        break;
    case 2: 
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            for (auto channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelN = buffer.getWritePointer(channel);
                channelN[sample] = squareWaveTable[(int)phase] * gain;
                phase = fmod((phase + increment/2), wtSize);
            }
        }
        break;
    case 3:
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            for (auto channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelN = buffer.getWritePointer(channel);
                channelN[sample] = fillSawWaveTable[(int)phase] * gain;
                phase = fmod((phase + increment/2), wtSize);
            }
        }
        break;
    case 4: 
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            for (auto channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelN = buffer.getWritePointer(channel);
                channelN[sample] = triangleWaveTable[(int)phase] * gain;
                phase = fmod((phase + increment/2), wtSize);
            }
        }
        break;
    case 5: 
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            for (auto channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelN = buffer.getWritePointer(channel);
                channelN[sample] = channelN[sample] * sineWaveTable[(int)phase] * gain;
                phase = fmod((phase + increment/2), wtSize);
            }
        }
        break;
    default: buffer.clear(); break;
    }
    waveVisualiser.pushBuffer(buffer);
}

//==============================================================================
bool WaveTableGeneratorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WaveTableGeneratorAudioProcessor::createEditor()
{
    return new WaveTableGeneratorAudioProcessorEditor (*this);
}

//==============================================================================
void WaveTableGeneratorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaveTableGeneratorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveTableGeneratorAudioProcessor();
}
