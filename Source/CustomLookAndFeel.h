/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 6 Sep 2023 1:52:27am
    Author:  Ali

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground (juce::Graphics& g, juce::Button& button, 
                               const juce::Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;
    
    void drawButtonText (juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, 
                           float sliderPos, float minSliderPos, float maxSliderPos, 
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawWaveform(juce::Graphics& g, const juce::Rectangle<int>& area, const float* data, int dataSize);

private:
    juce::Font getFontFromHeight(int height, const juce::String& text);
};
