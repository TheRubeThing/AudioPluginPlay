#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterPluginAudioProcessorEditor::FilterPluginAudioProcessorEditor (FilterPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      _cutoff_slider_attachment(*p.getParameterState(), "fc", _cutoff_slider),
      _Q_slider_attachment(*p.getParameterState(), "Q", _Q_slider),
      _boost_cut_slider_attachment(*p.getParameterState(), "boost_cut", _boost_cut_slider),
      _filter_type_combo_box_attachment(*p.getParameterState(), "filter_type", _filter_type_combo_box)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    double ratio = 4.0/3.0;
    setResizeLimits(400, 400/ratio, 1200, 1200/ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(400.0,400.0/ratio);

    // Cutoff slider
    setupSlider(_cutoff_slider, 20.0, 20000.0, " Hz");
    _cutoff_slider.setSkewFactor(0.25);

    addAndMakeVisible(_cutoff_slider_label);
    _cutoff_slider_label.setText("Cutoff frequency", juce::dontSendNotification);
    _cutoff_slider_label.attachToComponent(&_cutoff_slider, false);

    // Q slider
    setupSlider(_Q_slider, 0.0, 10.0, "");

    addAndMakeVisible(_Q_slider_label);
    _Q_slider_label.setText("Q", juce::dontSendNotification);
    _Q_slider_label.attachToComponent(&_Q_slider, false);

    // boost cut
    setupSlider(_boost_cut_slider, -96.0, 24.0, " dB");

    addAndMakeVisible(_boost_cut_slider_label);
    _boost_cut_slider_label.setText("Boost/Cut", juce::dontSendNotification);
    _boost_cut_slider_label.attachToComponent(&_boost_cut_slider, false);

    // type combo box
    addAndMakeVisible(_filter_type_combo_box);
    _filter_type_combo_box.addItem("LPF1", 1);
    _filter_type_combo_box.addItem("LPF2", 2);
    _filter_type_combo_box.setSelectedId(1);

    addAndMakeVisible(_filter_type_combo_box_label);
    _filter_type_combo_box_label.setText("Filter type", juce::dontSendNotification);
    _filter_type_combo_box_label.attachToComponent(&_filter_type_combo_box, false);
}

FilterPluginAudioProcessorEditor::~FilterPluginAudioProcessorEditor()
{
}

//==============================================================================
void FilterPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void FilterPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    float margin = 0.01f;
    int component_box = (getWidth() - 5.0f * margin * getHeight()) / 4.0f;
    auto bounds = getBounds();
    bounds.reduce(margin*getHeight(), margin*getHeight());
    bounds.removeFromTop(component_box * 2); // space for filter graph
    _cutoff_slider.setBounds(bounds.removeFromLeft(component_box).reduced(margin * getHeight(), margin * getHeight()));
    bounds.removeFromLeft(margin * getHeight());
    _Q_slider.setBounds(bounds.removeFromLeft(component_box).reduced(margin * getHeight(), margin * getHeight()));
    bounds.removeFromLeft(margin * getHeight());
    _boost_cut_slider.setBounds(bounds.removeFromLeft(component_box).reduced(margin * getHeight(), margin * getHeight()));
    bounds.removeFromLeft(margin * getHeight());
    // combo box space
    bounds.removeFromBottom(component_box / 3);
    bounds.removeFromTop(component_box / 3);
    _filter_type_combo_box.setBounds(bounds.reduced(margin * getHeight(), margin * getHeight()));
}

void FilterPluginAudioProcessorEditor::setupSlider(juce::Slider& slider, 
                                                   double min_value, 
                                                   double max_value, 
                                                   const std::string& suffix)
{
    addAndMakeVisible(slider);
    slider.setRange(min_value, max_value);
    slider.setTextValueSuffix(suffix);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
}

