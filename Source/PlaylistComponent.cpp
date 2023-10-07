/*
  ==============================================================================
    PlaylistComponent.cpp
    Created: 24 Jul 2023 3:29:26pm
    Author:  Ali
  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "AlertCallback.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
    DeckGUI* _deckGUI2,
    DJAudioPlayer* _playerForParsingMetaData
)
    : deckGUI1(_deckGUI1),
    deckGUI2(_deckGUI2),
    playerForParsingMetaData(_playerForParsingMetaData)
{
    // Child components and initial settings setup (Self-written section)
    addAndMakeVisible(importButton);
    addAndMakeVisible(searchField);
    addAndMakeVisible(library);
    addAndMakeVisible(addToPlayer1Button);
    addAndMakeVisible(addToPlayer2Button);

    importButton.addListener(this);
    searchField.addListener(this);
    addToPlayer1Button.addListener(this);
    addToPlayer2Button.addListener(this);

    searchField.setTextToShowWhenEmpty("Search Tracks (enter to submit)", juce::Colours::orange);
    searchField.onReturnKey = [this] { searchLibrary(searchField.getText()); };

    // Table setup and library load (Self-written section)
    library.getHeader().addColumn("Tracks", 1, 1);
    library.getHeader().addColumn("Length", 2, 1);
    library.getHeader().addColumn("", 3, 1);
    library.setModel(this);
    loadLibrary();
}

PlaylistComponent::~PlaylistComponent()
{
    // Self-written destructor
    saveLibrary();
}

void PlaylistComponent::alertWindowCallback(int returnValue)
{
    // Callback for alert window (Self-written section)
    if (returnValue == 1)
    {
        // Actions for the OK button can be defined here
    }
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    // JUCE generated paint method, with custom drawing code added
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
}

void PlaylistComponent::resized()
{
    // Setting the bounds for each component (Self-written section)
    importButton.setBounds(0, 0, getWidth(), getHeight() / 16);
    library.setBounds(0, 1 * getHeight() / 16, getWidth(), 13 * getHeight() / 16);
    searchField.setBounds(0, 14 * getHeight() / 16, getWidth(), getHeight() / 16);
    addToPlayer1Button.setBounds(0, 15 * getHeight() / 16, getWidth() / 2, getHeight() / 16);
    addToPlayer2Button.setBounds(getWidth() / 2, 15 * getHeight() / 16, getWidth() / 2, getHeight() / 16);

    // Setting column widths (Self-written section)
    library.getHeader().setColumnWidth(1, 12.8 * getWidth() / 20);
    library.getHeader().setColumnWidth(2, 5 * getWidth() / 20);
    library.getHeader().setColumnWidth(3, 2 * getWidth() / 20);
}

int PlaylistComponent::getNumRows()
{
    // Return the number of tracks available (Self-written section)
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    // Setting row background colors (Self-written section)
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    // Display track titles and lengths (Self-written section)
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(tracks[rowNumber].title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].length, 2, 0, width - 4, height, juce::Justification::centred, true);
        }
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    // Creates delete buttons in each row (Self-written section)
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "X" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

// This function is a callback for when any button in the playlist component is clicked.
// Depending on the button clicked, it triggers different actions such as loading a track in a player or importing tracks to the library.
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // If the import button is clicked, logs the click, imports tracks to the library, and updates the library content.
    if (button == &importButton)
    {
        DBG("Load button clicked");
        importToLibrary();
        library.updateContent();
    }
    // If the add to player 1 button is clicked, logs the click and loads the selected track in player 1.
    else if (button == &addToPlayer1Button)
    {
        DBG("Add to Player 1 clicked");
        loadInPlayer(deckGUI1);
    }
    // If the add to player 2 button is clicked, logs the click and loads the selected track in player 2.
    else if (button == &addToPlayer2Button)
    {
        DBG("Add to Player 2 clicked");
        loadInPlayer(deckGUI2);
    }
    // For other buttons, retrieves the ID of the button clicked, logs the track removal and removes the track from the tracks vector, and updates the library content.
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(tracks[id].title + " removed from Library");
        deleteFromTracks(id);
        library.updateContent();
    }
}

// This function handles the loading of a selected track into a specified player.
// If no track is selected, it displays an alert window with a message prompting the user to select a track.
void PlaylistComponent::loadInPlayer(DeckGUI* deckGUI)
{
    int selectedRow{ library.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << tracks[selectedRow].title << " to Player");
        deckGUI->loadFile(tracks[selectedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
            "Add to Deck Information",
            "Please select a track to add to deck",
            "OK",
            nullptr,
            new AlertCallback(this));
    }
}

void PlaylistComponent::importToLibrary()
{
    DBG("PlaylistComponent::importToLibrary called");

    // initialize file chooser
    juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen()) // Reverted to the correct method
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInTracks(fileNameWithoutExtension)) // if not already loaded
            {
                Track newTrack{ file };
                juce::URL audioURL{ file };
                newTrack.length = getLength(audioURL);
                tracks.push_back(newTrack);
                DBG("loaded file: " << newTrack.title);
            }
            else // display info message
            {
                if (juce::AlertWindow::showOkCancelBox(juce::AlertWindow::InfoIcon,
                    "Load information:",
                    fileNameWithoutExtension + " already loaded"))
                {
                    // OK button was clicked, you can put any action you want to happen here
                }
            }
        }
    }
}

bool PlaylistComponent::isInTracks(juce::String fileNameWithoutExtension)
{
    return (std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
}

void PlaylistComponent::deleteFromTracks(int id)
{
    tracks.erase(tracks.begin() + id);
}

juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double seconds{ playerForParsingMetaData->getLengthInSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
}

juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    //find seconds and minutes and make into string
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };
    
    if (sec.length() < 2) // if seconds is 1 digit or less
    {
        //add '0' to seconds until seconds is length 2
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
}

void PlaylistComponent::searchLibrary(juce::String searchText)
{
    DBG("Searching library for: " << searchText);
    if (searchText != "")
    {
        int rowNumber = whereInTracks(searchText);
        library.selectRow(rowNumber);
    }
    else
    {
        library.deselectAllRows();
    }
}

int PlaylistComponent::whereInTracks(juce::String searchText)
{
    // finds index where track title contains searchText
    auto it = find_if(tracks.begin(), tracks.end(), 
        [&searchText](const Track& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != tracks.end())
    {
        i = std::distance(tracks.begin(), it);
    }

    return i;
}

void PlaylistComponent::saveLibrary()
{
    // create .csv to save library
    std::ofstream myLibrary("my-library.csv");

    // save library to file
    for (Track& t : tracks)
    {
        myLibrary << t.file.getFullPathName() << "," << t.length << "\n";
    }
}

void PlaylistComponent::loadLibrary()
{
    // create input stream from saved library
    std::ifstream myLibrary("my-library.csv");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (myLibrary.is_open())
    {
        while (getline(myLibrary, filePath, ',')) {
            juce::File file{ filePath };
            Track newTrack{ file };

            getline(myLibrary, length);
            newTrack.length = length;
            tracks.push_back(newTrack);
        }
    }
    myLibrary.close();
}