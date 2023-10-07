/*
  ==============================================================================
    AlertCallback.cpp
    Created: 5 Sep 2023 9:58:39pm
    Author:  Ali
  ==============================================================================
*/

#include "AlertCallback.h"

/**
 * Constructor: AlertCallback (Self-generated)
 *
 * Purpose:
 *   Initializes a new instance of the AlertCallback class, associating it with a PlaylistComponent instance.
 *
 * Inputs:
 *   playlistComponent - A pointer to a PlaylistComponent instance. This will be used for invoking the alert window callback method.
 */
AlertCallback::AlertCallback(PlaylistComponent* playlistComponent)
    : playlistComponent(playlistComponent)
{
    // Constructor body: Initialization of playlistComponent member variable (Self-generated)
}

/**
 * Function: modalStateFinished (Self-generated)
 *
 * Purpose:
 *   This function is invoked when a modal state has ended. It triggers the alert window callback method in the associated PlaylistComponent instance.
 *
 * Inputs:
 *   result - An integer representing the result of the modal state (e.g., which button was pressed in a modal dialog).
 */
void AlertCallback::modalStateFinished(int result)
{
    // Triggering the alert window callback with the modal state result (Self-generated)
    playlistComponent->alertWindowCallback(result);
}
