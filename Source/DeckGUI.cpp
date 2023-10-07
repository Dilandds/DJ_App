/*
  ==============================================================================

    DeckGUI.cpp
    Created: 23 Jul 2023 12:16:53pm
    Author:  Ali

  ==============================================================================
*/
#include "AudioProcessorClass.h"
#include <JuceHeader.h>
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(int _id,
                 DJAudioPlayer* _player, 
                 juce::AudioFormatManager& formatManager,
                 juce::AudioThumbnailCache& thumbCache,AudioProcessorClass& audioProcessor
                ) : player(_player),
                    id(_id),
                    waveformDisplay(id, formatManager, thumbCache),
    audioProcessorClass(audioProcessor)
{
    // add all components and make visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(reverbPlot1);
    addAndMakeVisible(reverbPlot2);
    addAndMakeVisible(waveformDisplay);
    
    //filter sliders
    lowPassSlider.setSliderStyle(juce::Slider::Rotary);
    lowPassSlider.setRange(20.0, 20000.0, 0.1);
    lowPassSlider.setValue(20000.0); // Initial value
    lowPassSlider.addListener(this);

    bandPassSlider.setSliderStyle(juce::Slider::Rotary);
    bandPassSlider.setRange(20.0, 20000.0, 0.1);
    bandPassSlider.setValue(1000.0); // Initial value
    bandPassSlider.addListener(this);

    highPassSlider.setSliderStyle(juce::Slider::Rotary);
    highPassSlider.setRange(20.0, 20000.0, 0.1);
    highPassSlider.setValue(20.0); // Initial value
    highPassSlider.addListener(this);

  

    addAndMakeVisible(&lowPassSlider);
    addAndMakeVisible(&bandPassSlider);
    addAndMakeVisible(&highPassSlider);


    // add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    reverbSlider.addListener(this);
    reverbPlot1.addListener(this);
    reverbPlot2.addListener(this);

    //custom looks on components
    playButton.setLookAndFeel(&customLookAndFeel);
    stopButton.setLookAndFeel(&customLookAndFeel);
    loadButton.setLookAndFeel(&customLookAndFeel);
    volSlider.setLookAndFeel(&customLookAndFeel);
    speedSlider.setLookAndFeel(&customLookAndFeel);
    posSlider.setLookAndFeel(&customLookAndFeel);
    reverbSlider.setLookAndFeel(&customLookAndFeel);
    reverbPlot1.setLookAndFeel(&customLookAndFeel);
    reverbPlot2.setLookAndFeel(&customLookAndFeel);
    waveformDisplay.setLookAndFeel(&customLookAndFeel);

    //configure volume slider and label
    double volDefaultValue = 0.5;
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, 
                              false, 
                              50, 
                              volSlider.getTextBoxHeight()
                             );
    volSlider.setValue(volDefaultValue);
    volSlider.setSkewFactorFromMidPoint(volDefaultValue);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);

    //configure speed slider and label
    double speedDefaultValue = 1.0;
    speedSlider.setRange(0.25, 4.0); //reaches breakpoint if sliderValue == 0
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
                              false,
                              50,
                              speedSlider.getTextBoxHeight()
                             );
    speedSlider.setValue(speedDefaultValue);
    speedSlider.setSkewFactorFromMidPoint(speedDefaultValue);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);

    //configure position slider and label
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
                              false,
                              50,
                              posSlider.getTextBoxHeight()
                             );
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);

    //configure reverb slider
    reverbSlider.setRange(0.0, 1.0);
    reverbSlider.setNumDecimalPlacesToDisplay(2);

    //configure reverb plots
    
    reverbPlot1.setTooltip("Set reverbe");
    reverbPlot2.setTooltip("Set reverbe");
   
    reverbPlot1.setLabelText("", "x: damping\ny: room size");
    reverbPlot2.setLabelText("", "x: dry level\ny: wet level");


    waveformDisplay.onPositionChanged = [this](double position) {
        player->setPositionRelative(position);
    };

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
    playButton.setLookAndFeel(nullptr);
    stopButton.setLookAndFeel(nullptr);
    loadButton.setLookAndFeel(nullptr);
    volSlider.setLookAndFeel(nullptr);
    speedSlider.setLookAndFeel(nullptr);
    posSlider.setLookAndFeel(nullptr);
    reverbSlider.setLookAndFeel(nullptr);
    reverbPlot1.setLookAndFeel(nullptr);
    reverbPlot2.setLookAndFeel(nullptr);
    waveformDisplay.setLookAndFeel(nullptr);

}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    

    // Calculate the x-coordinates for drawing lines
    int startX = volSlider.getX();
    int endX = volSlider.getX() + volSlider.getWidth();

    // Draw separator lines
    g.setColour(juce::Colours::black);  // Set color for the separator line
    

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::transparentBlack);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{
    /*This method is where you should set the bounds of any child
    components that your component contains..*/

    auto sliderLeft = getWidth() / 9;
    auto mainRight = getWidth() - getHeight() / 2;
    auto plotRight = getWidth() - mainRight;

    int buttonHeight = getHeight() / 8;

    //                   x start, y start, width, height
    playButton.setBounds(0, 0, mainRight / 3, buttonHeight);
    stopButton.setBounds(mainRight / 3, 0, mainRight / 3, buttonHeight);
    loadButton.setBounds(2 * mainRight / 3, 0, mainRight / 3, buttonHeight);

    lowPassSlider.setBounds(0, buttonHeight, mainRight / 3, buttonHeight);
    bandPassSlider.setBounds(mainRight / 3, buttonHeight, mainRight / 3, buttonHeight);
    highPassSlider.setBounds(2 * mainRight / 3, buttonHeight, mainRight / 3, buttonHeight);

    // Increasing the height of the sliders below to use up the space left by removed toggle buttons
    volSlider.setBounds(sliderLeft, 2 * buttonHeight, mainRight - sliderLeft, buttonHeight * 1.5);
    speedSlider.setBounds(sliderLeft, 3.5 * buttonHeight, mainRight - sliderLeft, buttonHeight * 1.5);
    posSlider.setBounds(sliderLeft, 5 * buttonHeight, mainRight - sliderLeft, buttonHeight * 1.5);

    reverbPlot1.setBounds(mainRight, 0, plotRight, getHeight() / 2);
    reverbPlot2.setBounds(mainRight, getHeight() / 2, plotRight, getHeight() / 2);

    // Expanding waveform display to use up the remaining space
    waveformDisplay.setBounds(0, 6.5 * buttonHeight, mainRight, 1.5 * buttonHeight);
}


//to handle button clicks
void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked ");
        player->play();
    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked ");
        player->stop();
    }
    if (button == &loadButton)
    {
        DBG("Load button was clicked ");
        juce::FileChooser chooser{"Select a file"};
        if (chooser.browseForFileToOpen())
        {
            loadFile(juce::URL{ chooser.getResult() });
        }
    }
}

//to handle the slider value changes
void DeckGUI::sliderValueChanged(juce::Slider* sliderP)
{
    if (sliderP == &volSlider)
    {
        DBG("Volume slider moved " << sliderP->getValue());
        player->setGain(sliderP->getValue());
    }
    if (sliderP == &speedSlider)
    {
        DBG("Speed slider moved " << sliderP->getValue());
        player->setSpeed(sliderP->getValue());
    }
    if (sliderP == &posSlider)
    {
        DBG("Position slider moved " << sliderP->getValue());
        player->setPositionRelative(sliderP->getValue());
    }
    if (sliderP == &lowPassSlider)
    {
        DBG("Low Pass slider moved " << sliderP->getValue());
        player->getAudioProcessor().setLowPassFrequency(sliderP->getValue());
    }
    if (sliderP == &bandPassSlider)
    {
        DBG("Band Pass slider moved " << sliderP->getValue());
        player->getAudioProcessor().setBandPassFrequency(sliderP->getValue());
    }
    if (sliderP == &highPassSlider)
    {
        DBG("High Pass slider moved " << sliderP->getValue());
        player->getAudioProcessor().setHighPassFrequency(sliderP->getValue());
    }

}

void DeckGUI::coordPlotValueChanged(CoordinatePlot* coordinatePlot)
{
    DBG("DeckGUI::coordPlotValueChanged called");
    if (coordinatePlot == &reverbPlot1)
    {
        DBG("Deck " << id << ": ReverbPlot1 was clicked");
        player->setRoomSize(coordinatePlot->getY());
        player->setDamping(coordinatePlot->getX());
    }
    if (coordinatePlot == &reverbPlot2)
    {
        DBG("Deck " << id << ": ReverbPlot2 was clicked");
        player->setWetLevel(coordinatePlot->getY());
        player->setDryLevel(coordinatePlot->getX());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag called. " 
        + std::to_string(files.size()) + " file(s) being dragged.");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{

    DBG("DeckGUI::filesDropped at " + std::to_string(x) 
        + "x and " + std::to_string(y) + "y" );
    if (files.size() == 1)
    {
        loadFile(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);

   
}


void DeckGUI::timerCallback()
{   
    //check if the relative position is greater than 0
    //otherwise loading file causes error
    if (player->getPositionRelative() > 0)
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
}


void DeckGUI::toggleLowPassFilter()
{
    lowPassEnabled = !lowPassEnabled;
}

void DeckGUI::toggleBandPassFilter()
{
    bandPassEnabled = !bandPassEnabled;
}

void DeckGUI::toggleHighPassFilter()
{
    highPassEnabled = !highPassEnabled;
}


void DeckGUI::setDJAudioPlayer(DJAudioPlayer* playerInstance)
{
    player = playerInstance;
}