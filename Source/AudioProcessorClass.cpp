/*
  ==============================================================================
    AudioProcessorClass.cpp
    Created: 10 Sep 2023 12:29:23pm
    Author:  Ali
  ==============================================================================
*/

#include "AudioProcessorClass.h"
#include "JuceHeader.h"

// Constructor: Initializes the AudioProcessorClass with a default sample rate (Self-written)
AudioProcessorClass::AudioProcessorClass()
    : lastSampleRate(44100.0), currentSampleRate(44100.0)
{
}

// Destructor: Cleans up the AudioProcessorClass instance (Self-written)
AudioProcessorClass::~AudioProcessorClass()
{
}

// Function: prepareToPlay (Self-written)
// Purpose: Prepares the audio processor to play, setting up necessary filters with the initial configurations.
// Inputs:
//   sampleRate - The sample rate for the audio stream.
//   samplesPerBlock - The number of samples per block.
void AudioProcessorClass::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec { sampleRate, static_cast<uint32_t> (samplesPerBlock), 2 };

    lowPassFilter.prepare(spec);
    lowPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 20000.0f);

    bandPassFilter.prepare(spec);
    bandPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, 1000.0f, 0.7f);

    highPassFilter.prepare(spec);
    highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 20.0f);
}

// Function: releaseResources (Self-written)
// Purpose: Releases any resources acquired during the operation of the processor.
void AudioProcessorClass::releaseResources()
{
}

// Function: setLowPassFrequency (Self-written)
// Purpose: Sets the frequency of the low pass filter, with validation and error handling.
// Inputs:
//   frequency - The new frequency for the low pass filter.
void AudioProcessorClass::setLowPassFrequency(double frequency)
{
    if (frequency <= 0.0 || frequency > 20000.0)
    {
        std::cerr << "Invalid frequency value: " << frequency << ". Frequency should be in the range (0, 20000]" << std::endl;
        frequency = 1000.0;
    }

    auto coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(currentSampleRate, frequency);
    lowPassFilter.coefficients = coefficients;
    DBG("LowPass Frequency changed to: " + juce::String(frequency));
}
// Function: setBandPassFrequency (Self-written)
// Purpose: Sets the frequency of the band pass filter, with validation and error handling.
// Inputs:
//   frequency - The new frequency for the band pass filter.
void AudioProcessorClass::setBandPassFrequency(double frequency)
{
    if (frequency <= 0.0 || frequency > 20000.0)
    {
        // Log the error
        std::cerr << "Invalid frequency value: " << frequency << ". Frequency should be in the range (0, 20000]" << std::endl;

        // Set a fallback value
        frequency = 1000.0;  // Using 1000 Hz as the fallback value. You can choose any other valid value.
    }

    auto coefficients = juce::dsp::IIR::Coefficients<float>::makeBandPass(currentSampleRate, frequency, 0.7f);
    bandPassFilter.coefficients = coefficients;
}
// Function: setHighPassFrequency (Self-written)
// Purpose: Sets the frequency of the high pass filter, with validation and error handling.
// Inputs:
//   frequency - The new frequency for the band pass filter.
void AudioProcessorClass::setHighPassFrequency(double frequency)
{
    if (frequency <= 0.0 || frequency > 20000.0)
    {
        // Log the error
        std::cerr << "Invalid frequency value: " << frequency << ". Frequency should be in the range (0, 20000]" << std::endl;

        // Set a fallback value
        frequency = 500.0;  // Using 500 Hz as the fallback value. You can choose any other valid value.
    }

    auto coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(currentSampleRate, frequency);
    highPassFilter.coefficients = coefficients;

}


// This method processes an audio block which consists of several audio channels
void AudioProcessorClass::processAudioBlock(juce::AudioBuffer<float>& buffer)
{
    // Check if the buffer has any channels or samples, if not log an error and return
    if (buffer.getNumChannels() == 0 || buffer.getNumSamples() == 0)
    {
        DBG("Error: Buffer has no channels or no samples");
        return;
    }

    // Loop over all channels in the buffer and process each one individually
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        // Calling the function to process a single channel with the write pointer to the channel data and the number of samples
        processSingleChannel(buffer.getWritePointer(channel), buffer.getNumSamples());
    }
}

// This method processes a single channel of audio data
void AudioProcessorClass::processSingleChannel(float* channelData, int numSamples)
{
    // Creating an audio block object for a single channel by passing the channel data pointer and the number of samples
    juce::dsp::AudioBlock<float> audioBlock(&channelData, 1, numSamples);

    // Creating a processing context using the created audio block which will replace the original data with the processed data
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    // Applying various filters sequentially to the audio data in the context
    lowPassFilter.process(context); // Applying low pass filter
    bandPassFilter.process(context); // Applying band pass filter
    highPassFilter.process(context); // Applying high pass filter
}

