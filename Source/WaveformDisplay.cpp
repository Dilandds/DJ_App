/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 24 Jul 2023 10:55:44am
    Author:  Ali

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"


#include "CustomLookAndFeel.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(int _id,
                                 juce::AudioFormatManager& formatManager,
                                 juce::AudioThumbnailCache& thumbCache
                                ) : audioThumb(1000, formatManager, thumbCache),
                                    fileLoaded(false),
                                    position(0),
                                    id(_id)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setLookAndFeel(&customLookAndFeel);
    audioThumb.addChangeListener(this);
}
// Destructor which resets the look and feel
WaveformDisplay::~WaveformDisplay()
{
    setLookAndFeel(nullptr);
}
// Personal code: This function sets the waveform data and marks the component to be repainted.
void WaveformDisplay::setWaveformData(const std::vector<float>& data)
    {
        waveformData = data;
        dataSize = data.size();
        repaint();
    }

// The paint method which draws the component and the waveform
void WaveformDisplay::paint(juce::Graphics& g)
{
    // Existing setup code
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
    g.setColour(juce::Colours::hotpink);

    g.setColour(juce::Colours::orangered);
    g.setFont(18.0f);
    g.drawText("Deck: " + std::to_string(id), getLocalBounds(),
        juce::Justification::centredTop,true);

    if (fileLoaded)
    {
        // Existing drawing code
        g.setFont(15.0f);
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
        );
        g.setColour(juce::Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
        g.setColour(juce::Colours::white);
        g.drawText(fileName, getLocalBounds(),
            juce::Justification::bottomLeft, true);

        // Drawing the waveform data from waveformData vector using CustomLookAndFeel
        if (!waveformData.empty())
        {
            customLookAndFeel.drawWaveform(g, getLocalBounds(), waveformData.data(), dataSize);
        }
    }
    else
    {
        // Existing file not loaded case
        g.setFont(20.0f);
        g.setColour(juce::Colours::green);

        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}


void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
// Change listener callback that gets called when the audio thumbnail changes
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}
// Method to load an audio URL and create a thumbnail for it
void WaveformDisplay::loadURL(juce::URL audioURL)
{
    DBG("WaveformDisplay::loadURL called");
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        DBG("WaveformDisplay::loadURL file loaded");
        fileName = audioURL.getFileName();// Setting the filename to be displayed on the component (Personal code)
        repaint();// Repainting the component to show the loaded file
    }
    else
    {
        DBG("WaveformDisplay::loadURL file NOT loaded");
    }
}
// Method to set the relative position of a marker on the waveform (Personal code)
void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();// Repainting the component to update the position marker
    }
}
// Mouse down event handler for interacting with the waveform (Personal code)
void WaveformDisplay::mouseDown(const juce::MouseEvent& event)
{
    if (fileLoaded)
    {
        position = static_cast<double>(event.x) / getWidth();
        if (onPositionChanged)
        {
            onPositionChanged(position);
        }
        repaint();
    }
}


// Mouse drag event handler for dragging the position marker on the waveform (Personal code)
void WaveformDisplay::mouseDrag(const juce::MouseEvent& event)
{
    if (fileLoaded)
    {
        position = static_cast<double>(event.x) / getWidth();
        if (onPositionChanged)
        {
            onPositionChanged(position);
        }
        repaint();
    }
}

