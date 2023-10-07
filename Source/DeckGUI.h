/*
  ==============================================================================

    DeckGUI.h
    Created: 23 Jul 2023 12:16:53pm
    Author:  Ali

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "CoordinatePlot.h"
#include "CustomLookAndFeel.h"
#include "AudioProcessorClass.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer,
                 public CoordinatePlot::Listener
{
public:
    DeckGUI(int _id,
            DJAudioPlayer* player, 
            juce::AudioFormatManager& formatManager, 
            juce::AudioThumbnailCache& thumbCache, AudioProcessorClass& audioProcessor);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /**Implement Button::Listener*/
    void buttonClicked(juce::Button* button) override;
    /**Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;
    /**Implement CoordinatePlot::Listener */
    void coordPlotValueChanged(CoordinatePlot* coordinatePlot) override;
    /**Detects if file is being dragged over deck*/
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    /**Detects if file is dropped onto deck*/
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    /**Listen for changes to the waveform*/
    void timerCallback() override;

    bool isLowPassEnabled() const { return lowPassEnabled; }
    bool isBandPassEnabled() const { return bandPassEnabled; }
    bool isHighPassEnabled() const { return highPassEnabled; }

    void toggleLowPassFilter();
    void toggleBandPassFilter();
    void toggleHighPassFilter();


    void setDJAudioPlayer(DJAudioPlayer* playerInstance);

private:
    int id;
    
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };
    juce::Slider volSlider;
    juce::Label volLabel;
    juce::Slider speedSlider;
    juce::Label speedLabel;
    juce::Slider posSlider;
    juce::Label posLabel;
    juce::Slider reverbSlider;
    juce::Slider slider;
    CoordinatePlot reverbPlot1;
    CoordinatePlot reverbPlot2;

    juce::Slider lowPassSlider;
    juce::Slider bandPassSlider;
    juce::Slider highPassSlider;

    juce::ToggleButton lowPassButton;
    juce::ToggleButton bandPassButton;
    juce::ToggleButton highPassButton;


    void loadFile(juce::URL audioURL);

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    juce::SharedResourcePointer< juce::TooltipWindow > sharedTooltip;

    friend class PlaylistComponent;

    CustomLookAndFeel customLookAndFeel;

    AudioProcessorClass& audioProcessorClass;
    bool lowPassEnabled = true;
    bool bandPassEnabled = false;
    bool highPassEnabled = false;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)


        int margin = 10;  // The space between sliders
    int lineThickness = 2;  // The thickness of separator lines

};
