/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 10 Aug 2023 5:23:41pm
    Author:  Ali

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

// Function to draw button background with custom colours for different states
// Inputs: Graphics context (g), Reference to the button, Background color, MouseOver and ButtonDown states
// Output: None (modifies the button appearance in the GUI)
void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown)
{
    // (Self-written code) Gets the bounds of the button area
    juce::Rectangle<int> buttonArea = button.getLocalBounds();
    juce::Colour buttonColor;

    // (Self-written code) Modifies the button color based on its state (clicked, hovered, or normal)
    if (isButtonDown)
    {
        buttonColor = juce::Colours::red;
    }
    else if (isMouseOverButton)
    {
        buttonColor = juce::Colours::midnightblue;
    }
    else
    {
        buttonColor = backgroundColour;
    }

    // Drawing the button with the designated color and a rounded rectangle shape
    g.setColour(buttonColor);
    g.fillRoundedRectangle(buttonArea.toFloat(), 10);

    // Drawing the border of the button
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(buttonArea.toFloat(), 10, 1);
}

// Function to draw linear slider with a custom appearance
// Inputs: Graphics context (g), Slider properties (position, size, style) and reference to the slider object
// Output: None (modifies the slider appearance in the GUI)
void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // (Self-written code) Setting the background color of the slider
    g.fillAll(juce::Colours::black);

    // Drawing the slider bar with a custom color and line thickness
    g.setColour(juce::Colour::fromRGB(0, 127, 255));
    g.drawLine(x, y + height / 2, x + width, y + height / 2, 2.0f);

    // (Self-written code) Drawing the thumb of the slider with a custom shape and color
    if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearVertical)
    {
        float thumbWidth = getSliderThumbRadius(slider);
        juce::Rectangle<float> thumbArea;

        // Setting the area of the thumb based on the slider style
        if (style == juce::Slider::LinearVertical)
            thumbArea = juce::Rectangle<float>(x + width * 0.5f - thumbWidth * 0.5f, sliderPos - thumbWidth, thumbWidth, thumbWidth * 2.0f);
        else
            thumbArea = juce::Rectangle<float>(sliderPos - thumbWidth, y + height * 0.5f - thumbWidth * 0.5f, thumbWidth * 2.0f, thumbWidth);

        // Drawing the thumb as a rounded rectangle with a default shape but custom color
        g.setColour(slider.findColour(juce::Slider::thumbColourId));
        g.fillRoundedRectangle(thumbArea.reduced(1.0f), thumbWidth * 0.5f);
    }
}

// Function to draw button text with custom settings
// Inputs: Graphics context (g), Reference to the text button, MouseOver and ButtonDown states
// Output: None (modifies the button text appearance in the GUI)
void CustomLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown)
{
    // (Self-written code) Getting the font based on the button height and text
    juce::Font font = getFontFromHeight(button.getHeight(), button.getButtonText());

    // Setting the colour and drawing the text in the button
    g.setColour(button.findColour(juce::TextButton::textColourOffId).withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));
    g.setFont(font);
    g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);
}

// Function to derive font settings based on the button height and text
// Inputs: Height of the button and the text to be displayed on the button
// Output: Customized font object
juce::Font CustomLookAndFeel::getFontFromHeight(int height, const juce::String& text)
{
    // (Self-written code) Setting the font size based on the height and making it bold
    juce::Font font(height * 0.6f);
    font.setBold(true);
    return font;
}

// Function to draw waveform with custom aesthetics
// Inputs: Graphics context (g), Area to draw the waveform, Data array of the waveform, Size of the data array
// Output: None (draws the waveform in the GUI)
void CustomLookAndFeel::drawWaveform(juce::Graphics& g, const juce::Rectangle<int>& area, const float* data, int dataSize)
{
    if (dataSize <= 0) return;

    // (Self-written code) Setting up custom color and starting the path for waveform drawing
    g.setColour(juce::Colour::fromRGB(0, 128, 255));
    juce::Path waveform;
    waveform.startNewSubPath(area.getX(), juce::jmap(data[0], 0.0f, 1.0f, float(area.getBottom()), float(area.getY())));

    // Drawing the waveform with custom gradient and glow effect
    for (int i = 1; i < dataSize; ++i)
    {
        waveform.lineTo(area.getX() + i, juce::jmap(data[i], 0.0f, 1.0f, float(area.getBottom()), float(area.getY())));
    }

    // Applying a gradient fill and drawing the glow effect at the peaks of the waveform
    juce::ColourGradient gradient(juce::Colour::fromRGB(0, 128, 255), 0.0f, area.getY(),
        juce::Colour::fromRGB(0, 64, 128), 0.0f, area.getBottom(), false);
    g.setGradientFill(gradient);
    g.strokePath(waveform, juce::PathStrokeType(2.0f));
    g.setColour(juce::Colour::fromRGB(0, 128, 255).withAlpha(0.5f));
    g.fillPath(waveform);
}
