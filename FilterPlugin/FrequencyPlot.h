#pragma once

#include <array>
#include <cmath>

#include "PluginProcessor.h"
#include "utils.h"

namespace
{
    constexpr int NUM_GRAPH_POINTS = 512;
    constexpr float FREQ_PLOT_MAX = 20000.0f;
    constexpr float FREQ_PLOT_MIN = 10.0f;
    const float PLOT_STEP = (std::log10(FREQ_PLOT_MAX) - std::log10(FREQ_PLOT_MIN)) / (NUM_GRAPH_POINTS); 
}
//==============================================================================
class FrequencyPlot  : public juce::Component, public juce::Timer
{
public:
    FrequencyPlot (FilterPluginAudioProcessor& p) : processorRef(p)
    {
        _x_points[0] = FREQ_PLOT_MIN;
        for (int i = 1; i < NUM_GRAPH_POINTS; ++i)
        {
            _x_points[i] = std::pow(10.0f, i * PLOT_STEP + std::log10(FREQ_PLOT_MIN));
        }

        startTimerHz(30);
    }
    ~FrequencyPlot() override
    {
    }

    //==============================================================================
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black);

        for (int i = 0; i < NUM_GRAPH_POINTS; ++i)
        {
            _y_points[i] = processorRef.getMagnitudedB(_x_points[i]);
        }  

        float height = getHeight();
        float width = getWidth();

        juce::Path path;

        path.startNewSubPath(0.0f, height + 1.0f);
        for (int i = 0; i < NUM_GRAPH_POINTS; ++i)
        {
            float y_value = rubdsp::map_value(-12.0f, 12.0f, height, 0.0f, _y_points[i], true);
            float x_value = i * width / (NUM_GRAPH_POINTS - 1);//rubdsp::map_value(0.0f, FREQ_PLOT_MAX, 0.0f, width, _x_points[i]);
            if (isnan(y_value))
            {
                y_value = 0.0f;
            }
            path.lineTo(x_value, y_value);
            // g.setColour(juce::Colours::orangered);
            // g.fillEllipse(x_value - 5.0f, y_value - 5.0f, 10.0f, 10.0f);
        }
        path.lineTo(width, height + 1.0f);
        path.closeSubPath();

        g.setColour(juce::Colours::royalblue);
        g.strokePath(path, juce::PathStrokeType(3.0f));
        g.setColour(juce::Colours::royalblue.withAlpha(0.5f));
        g.fillPath(path);     
    }
    void resized() override
    {

    }

    void timerCallback() override
    {
        repaint();
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterPluginAudioProcessor& processorRef;

    std::array<float, NUM_GRAPH_POINTS> _x_points;
    std::array<float, NUM_GRAPH_POINTS> _y_points;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyPlot)
};
