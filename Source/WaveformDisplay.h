/*
  ==============================================================================

    WaveformDisplay.h
    Created: 24 Jul 2023 10:55:44am
    Author:  Ali

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(int _id,
                    juce::AudioFormatManager& formatManager,
                    juce::AudioThumbnailCache& thumbCache);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void loadURL(juce::URL audioURL);
    /**set the relative position of the playhead*/
    void setPositionRelative(double pos);

    // Adding new members to handle mouse interaction and setting playback position
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    void  setWaveformData(const std::vector<float>& data);

    std::function<void(double)> onPositionChanged;

private:
    int id;
    bool fileLoaded;
    double position;
    juce::String fileName;
    juce::AudioThumbnail audioThumb;
    int dataSize = 0;
    std::vector<float> waveformData;
    CustomLookAndFeel customLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
