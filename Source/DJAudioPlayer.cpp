/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 23 Jul 2023 12:16:53pm
    Author:  Ali

  ==============================================================================
*/

#include "DJAudioPlayer.h"

// Constructor: initializes the format manager, sets default reverb settings, and prepares the audio processor
// Inputs: Reference to an existing AudioFormatManager instance
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
    // (Self-written code) Set up the initial reverb parameters
    reverbParameters.roomSize = 0;
    reverbParameters.damping = 0;
    reverbParameters.wetLevel = 0;
    reverbParameters.dryLevel = 1.0;
    reverbSource.setParameters(reverbParameters);

    // (Self-written code) Initialize the audio processor with initial sample rate and block size
    double initialSampleRate = 44100.0;
    int initialSamplesPerBlock = 512;
    audioProcessor.prepareToPlay(initialSampleRate, initialSamplesPerBlock);
}

// Destructor: clean up resources when the instance is destroyed
DJAudioPlayer::~DJAudioPlayer()
{
}

// Prepares various sources for playback with given sample rate and block size
// Inputs: Expected samples per block, Sample rate
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    audioProcessor.prepareToPlay(sampleRate, samplesPerBlockExpected);
}

// Processes the next block of audio
// Inputs: Information about the buffer to fill
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    reverbSource.getNextAudioBlock(bufferToFill);
    audioProcessor.processAudioBlock(*bufferToFill.buffer);
}

// Releases resources allocated by various sources
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
}

// Loads audio from a URL into the transport source
// Inputs: The URL of the audio to load
void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    DBG("DJAudioPlayer::loadURL called");
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    // (Self-written code) Load the reader into the transport source if valid
    if (reader != nullptr)
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

// Other methods follow a similar structure: simple, self-explanatory one-liners (self-written) with some debug information and parameter validation.

void DJAudioPlayer::play() { transportSource.start(); }
void DJAudioPlayer::stop() { transportSource.stop(); }
void DJAudioPlayer::setPosition(double posInSecs) { transportSource.setPosition(posInSecs); }

// A method to set the position relative to the length of the track
// Inputs: The relative position (between 0 and 1)
void DJAudioPlayer::setPositionRelative(double pos)
{
    // (Self-written code) Parameter validation and conversion to seconds
    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative position should be between 0 and 1");
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

// (Self-written code) Below methods are similar, setting various parameters with some validation. They change aspects such as gain, speed, and reverb settings.

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else { transportSource.setGain(gain); }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.25 || ratio > 4.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0.25 and 4");
    }
    else { resampleSource.setResamplingRatio(ratio); }
}

void DJAudioPlayer::setRoomSize(float size)
{
    DBG("DJAudioPlayer::setRoomSize called");
    if (size < 0 || size > 1.0)
    {
        DBG("DJAudioPlayer::setRoomSize size should be between 0 and 1.0");
    }
    else
    {
        reverbParameters.roomSize = size;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDamping(float dampingAmt)
{
    DBG("DJAudioPlayer::setDamping called");
    if (dampingAmt < 0 || dampingAmt > 1.0)
    {
        DBG("DJAudioPlayer::setDamping amount should be between 0 and 1.0");
    }
    else
    {
        reverbParameters.damping = dampingAmt;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setWetLevel(float wetLevel)
{
    DBG("DJAudioPlayer::setWetLevel called");
    if (wetLevel < 0 || wetLevel > 1.0)
    {
        DBG("DJAudioPlayer::setWetLevel level should be between 0 and 1.0");
    }
    else
    {
        reverbParameters.wetLevel = wetLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDryLevel(float dryLevel)
{
    DBG("DJAudioPlayer::setDryLevel called");
    if (dryLevel < 0 || dryLevel > 1.0)
    {
        DBG("DJAudioPlayer::setDryLevel level should be between 0 and 1.0");
    }
    else
    {
        reverbParameters.dryLevel = dryLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

// Returns the current position relative to the length of the track
// Outputs: The relative position as a double
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

// Returns the length of the current track in seconds
// Outputs: The length in seconds as a double
double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}

// Provides access to the internal audio processor instance
// Outputs: Reference to the internal audio processor instance
AudioProcessorClass& DJAudioPlayer::getAudioProcessor()
{
    return audioProcessor;
}
