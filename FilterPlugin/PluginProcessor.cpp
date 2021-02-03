#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterPluginAudioProcessor::FilterPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    _parameters(*this, nullptr, juce::Identifier("FilterPlugin"),{
        std::make_unique<juce::AudioParameterFloat> ("fc", // parameter ID
                                                     "Cutoff frequency", // parameter name
                                                     20.0, // minimum value
                                                     18000.0, // maximum value
                                                     1000.0
                                                     ),
        std::make_unique<juce::AudioParameterFloat> ("Q", "Q", 0.0, 10.0, 3.0),
        std::make_unique<juce::AudioParameterFloat> ("boost_cut", "Boost/Cut", -96.0, 24.0, 0.0),
        std::make_unique<juce::AudioParameterInt> ("filter_type", "Filter Type", 0, 1, 0)
    })
{
}

FilterPluginAudioProcessor::~FilterPluginAudioProcessor()
{
}

//==============================================================================
const juce::String FilterPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String FilterPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void FilterPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void FilterPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    _left_filter.reset(sampleRate);
    _right_filter.reset(sampleRate);
}

void FilterPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool FilterPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FilterPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto parameters = _left_filter.getParameters();
    parameters.fc = *_parameters.getRawParameterValue("fc");
    parameters.Q = *_parameters.getRawParameterValue("Q");
    parameters.boost_cut_db = *_parameters.getRawParameterValue("boost_cut");
    
     int algorithm_index = _parameters.getParameterAsValue("filter_type").getValue();
    switch (algorithm_index)
    {
        case 0:
        {
            parameters.algorithm = rubdsp::filterAlgorithm::LPF1;
            break;
        }
        case 1:
        {
            parameters.algorithm = rubdsp::filterAlgorithm::LPF2;
            break;
        }
    }


    _left_filter.setParameters(parameters);
    _right_filter.setParameters(parameters);

    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            if (channel == 0)
            {
                buffer.setSample(channel, sample, _left_filter.processAudioSample(buffer.getSample(channel, sample)));
            }
            else if (channel == 1)
            {
                buffer.setSample(channel, sample, _right_filter.processAudioSample(buffer.getSample(channel, sample)));
            }
        }
    }
}

//==============================================================================
bool FilterPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FilterPluginAudioProcessor::createEditor()
{
    return new FilterPluginAudioProcessorEditor (*this);
}

//==============================================================================
void FilterPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void FilterPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterPluginAudioProcessor();
}
