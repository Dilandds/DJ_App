# Description of the JUCE DJ Application

![Screen shot](/images/1.jpg)

The developed DJ application, crafted using the JUCE library, integrates multiple classes and functionalities to create a comprehensive and intuitive environment for mixing and playing tracks.

## Application Requirements:

### R1A: Can Load Audio Files into Audio Players

- **PlaylistComponent.cpp**: Central to managing the library of tracks. Functionalities such as `importToLibrary` load new audio files and display them in a playlist. `Track.cpp` defines the Track objects.
- **DeckGUI.cpp & DJAudioPlayer.cpp**: Manage individual audio player components. Data from the Track object is conveyed to the player component.

### R1B: Can Play Two or More Tracks

- **DJAudioPlayer.cpp**: Handles audio playback. Features two (or more) instances of the DJAudioPlayer, each capable of playing a track independently.

### R1C: Can Mix the Tracks by Varying Each of Their Volumes

- **DeckGUI.cpp**: Each DeckGUI component has controls for adjusting the volume of the track loaded.

### R1D: Can Speed Up and Slow Down the Tracks

- **DJAudioPlayer.cpp**: Manages the playback speed of the loaded tracks.

## Additional Components:

- **Main.cpp & MainComponent.cpp**: Serve as the application's entry point.
- **WaveformDisplay.cpp**: Visualizes the waveform of the loaded tracks.
- **CustomLookAndFeel.cpp**: Customizes the visual aspects.
- **CoordinatePlot.cpp**: Used to set reverb.
- **AudioProcessorClass.cpp**: Processes the audio data.
- **AlertCallback.cpp**: Manages alert dialogs.

---

# Customizing the User Interface (R2)

Comparing the code you posted, here are the differences and new components:

## R2A: Changes in GUI layout

1. **New Components Added**:
   - `volLabel`
   - `speedLabel`
   - `posLabel`
   - `reverbPlot1`
   - `reverbPlot2`
   - `waveformDisplay`
2. **Modified Components**:

   - `volSlider`: Configuration modifications.
   - `speedSlider`: Configuration updates.
   - `posSlider`: Added configurations.

3. **Visual and Functional Enhancements**:
   - **LookAndFeel Customization**: Used to customize appearance.
   - **Reverb Plots**: New tooltips and labels.

### R2B: New Event Listener

1. **New Event Listeners**:
   - Added to `reverbSlider`, `reverbPlot1`, `reverbPlot2`.
   - `waveformDisplay.onPositionChanged`: New listener.

---

# Feature Description and Implementation

We enhanced our DJ application, inspired by a counterpart DJ application labeled as R3.

## Technical Aspects:

1. **AudioProcessorClass Module**: Pivotal in this enhancement.
2. **In-depth Analysis**: Features like Constructor, Destructor, `prepareToPlay`, and more.
3. **Real-time Processing**: Includes `processAudioBlock` and `processSingleChannel`.
4. **Backend Infrastructure**: Uses JUCE Framework and `AudioBuffer` and `dsp` Modules.
