/*
  ==============================================================================
    AlertCallback.h
    Created: 5 Sep 2023 9:58:20pm
    Author:  Ali
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PlaylistComponent.h" // Include the header file for PlaylistComponent (Self-generated)

// Class: AlertCallback (Self-generated)
// Extends the juce::ModalComponentManager::Callback to handle modal state transitions.
// It holds a reference to a PlaylistComponent to perform actions when the modal state is finished.
class AlertCallback : public juce::ModalComponentManager::Callback
{
public:
    // Constructor: AlertCallback (Self-generated)
    // Purpose:
    //   Initializes an instance of the AlertCallback class.
    // Inputs:
    //   playlistComponent - A pointer to the PlaylistComponent instance that this AlertCallback will interact with.
    AlertCallback(PlaylistComponent* playlistComponent);

    // Function: modalStateFinished (Self-generated)
    // Purpose:
    //   Defines the actions to be performed when the modal state finishes.
    //   Specifically, it calls the alertWindowCallback function of the associated PlaylistComponent.
    // Inputs:
    //   result - An integer representing the result of the modal state (e.g., the button pressed in a modal dialog).
    void modalStateFinished(int result) override;

private:
    // Member Variable: playlistComponent (Self-generated)
    // Holds a pointer to the PlaylistComponent instance to interact with when the modal state finishes.
    PlaylistComponent* playlistComponent;
};
