/*
  ==============================================================================

    Track.cpp
    Created: 4 Aug 2023 10:16:10am
    Author:  Ali

  ==============================================================================
*/

#include "Track.h"
#include <filesystem>

// Constructor definition for the Track class.
// It initializes the file, title, and URL members of the class with the given file parameter.
// The title is derived from the file name without extension, and the URL is constructed from the file object.
// It also logs the creation of a new track.
Track::Track(juce::File _file)
    : file(std::move(_file)), // Moving the file object to avoid unnecessary copies
    title(file.getFileNameWithoutExtension()), // Initializing title with the filename without its extension
    URL(juce::URL{ file }) // Initializing URL with a juce::URL object created from the file
{
    // Logging the creation of a new track with its title
    DBG("Created new track with title: " << title);
}

// Overloading the equality operator to compare a Track object with a juce::String object (presumably a title).
// It returns true if the title member of the Track object is equal to the input juce::String object.
bool Track::operator==(const juce::String& other) const
{
    // Comparing the title member with the input string and returning the result
    return title == other;
}
