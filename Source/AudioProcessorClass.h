/*
  ==============================================================================
    AudioProcessorClass.h
    Created: 10 Sep 2023 12:29:54pm
    Author:  Ali
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// This class manages the audio processing in your application, including filtering operations.
class AudioProcessorClass
{
public:
    // Constructor: Initializes member variables to default values (Self-written)
    AudioProcessorClass();

    // Destructor: Cleans up the resources before the object is deleted (Self-written)
    ~AudioProcessorClass();

    // Function: prepareToPlay (Self-written)
    // Purpose: Sets up the audio processor to be ready for play.
    // Inputs:
    // - double sampleRate: The sample rate to be used for audio processing.
    // - int samplesPerBlock: The number of samples in each block of audio data.
    void prepareToPlay(double sampleRate, int samplesPerBlock);

    // Function: releaseResources (Self-written)
    // Purpose: Releases any resources that were in use by the processor.
    void releaseResources();

    // Function: setLowPassFrequency (Self-written)
    // Purpose: Sets the frequency for the low pass filter and updates its coefficients accordingly.
    // Inputs: 
    // - double frequency: The new frequency value for the low pass filter.
    void setLowPassFrequency(double frequency);

    // Function: setBandPassFrequency (Self-written)
    // Purpose: Sets the frequency for the band pass filter and updates its coefficients accordingly.
    // Inputs: 
    // - double frequency: The new frequency value for the band pass filter.
    void setBandPassFrequency(double frequency);

    // Function: setHighPassFrequency (Self-written)
    // Purpose: Sets the frequency for the high pass filter and updates its coefficients accordingly.
    // Inputs: 
    // - double frequency: The new frequency value for the high pass filter.
    void setHighPassFrequency(double frequency);

    // Function: processAudioBlock (Self-written)
    // Purpose: Processes an audio buffer, applying filters to the audio data.
    // Inputs:
    // - juce::AudioBuffer<float>& buffer: A reference to the buffer containing the audio data to be processed.
    void processAudioBlock(juce::AudioBuffer<float>& buffer);

    // Function: processSingleChannel (Self-written)
    // Purpose: Processes a single channel of audio data.
    // Inputs:
    // - float* channelData: A pointer to the data for the channel to be processed.
    // - int numSamples: The number of samples in the channel data.
    void processSingleChannel(float* channelData, int numSamples);

private:
    // Filters and state variables for the audio processor (Self-written)
    juce::dsp::IIR::Filter<float> lowPassFilter;
    juce::dsp::IIR::Filter<float> bandPassFilter;
    juce::dsp::IIR::Filter<float> highPassFilter;

    double lowPassFrequency = 2000.0; // Initial value for low pass filter frequency
    double bandPassFrequency = 1000.0; // Initial value for band pass filter frequency
    double highPassFrequency = 500.0; // Initial value for high pass filter frequency

    double currentSampleRate;
    double lastSampleRate;
    int lastSamplesPerBlock;
};
