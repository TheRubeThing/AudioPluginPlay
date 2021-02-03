#pragma once

#include "PluginProcessor.h"

//==============================================================================
class FilterPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit FilterPluginAudioProcessorEditor (FilterPluginAudioProcessor&);
    ~FilterPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void setupSlider(juce::Slider& slider, double min_value, double max_value, const std::string& suffix);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterPluginAudioProcessor& processorRef;

    juce::Slider _cutoff_slider;
    juce::Label _cutoff_slider_label;
    juce::AudioProcessorValueTreeState::SliderAttachment _cutoff_slider_attachment;

    juce::Slider _Q_slider;
    juce::Label _Q_slider_label;
    juce::AudioProcessorValueTreeState::SliderAttachment _Q_slider_attachment;

    juce::Slider _boost_cut_slider;
    juce::Label _boost_cut_slider_label;
    juce::AudioProcessorValueTreeState::SliderAttachment _boost_cut_slider_attachment;

    juce::ComboBox _filter_type_combo_box;
    juce::Label _filter_type_combo_box_label;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment _filter_type_combo_box_attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterPluginAudioProcessorEditor)
};
