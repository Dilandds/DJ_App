/*
  ==============================================================================

    CoordinatePlot.h
    Created: 10 Aug 2023 5:23:41pm
    Author:  Ali

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
   A class to represent a coordinate plot component. This component allows for
   plotting and visualizing 2D coordinates with various customization options
   such as setting ranges, grid lines, and labels.

   This class inherits from juce::Component and juce::SettableTooltipClient.
*/
class CoordinatePlot : public juce::Component,
    public juce::SettableTooltipClient
{
public:
    // Default constructor
    CoordinatePlot();

    // Destructor
    ~CoordinatePlot() override;

    // Overrides paint method from juce::Component to draw the component
    void paint(juce::Graphics&) override;

    // Overrides resized method from juce::Component to update component size
    void resized() override;

    // Event handler for mouse down events
    void mouseDown(const juce::MouseEvent& event) override;

    // Event handler for mouse up events
    void mouseUp(const juce::MouseEvent& event) override;

    // Event handler for mouse drag events
    void mouseDrag(const juce::MouseEvent& event) override;

    /**
     * Set the number of grid lines shown on the Coordinate Plot.
     * @param lineCount: The number of grid lines to be displayed.
     *                   Defaults to 4 (4 vertical, 4 horizontal).
     *                   Reduces lineCount by 1 if odd. Uses default if lineCount < 2.
     * - Created without assistance.
     */
    void setGridLineCount(int lineCount = 4);

    /**
     * Set the range of values for the coordinate plot.
     * @param min: The minimum value of the range.
     * @param max: The maximum value of the range.
     * - Created without assistance.
     */
    void setRange(float min = 0.0f, float max = 1.0f);

    /**
     * Get the current x coordinate.
     * @return The current x coordinate.
     * - Created without assistance.
     */
    float getX();

    /**
     * Get the current y coordinate.
     * @return The current y coordinate.
     * - Created without assistance.
     */
    float getY();

    // Nested class to represent a listener for the coordinate plot
    class Listener
    {
    public:
        // Constructor
        Listener();

        // Destructor
        ~Listener();

        // Pure virtual function to define the callback for listener
        virtual void coordPlotValueChanged(CoordinatePlot* coordinatePlot) = 0;
    };

    // Calls the callback method on all registered listeners
    void interactWithComponent();

    // Adds a listener to the list of registered listeners
    void addListener(Listener* l);

    // Removes a listener from the list of registered listeners
    void removeListener(Listener* l);

    // Sets the label texts for the top and bottom labels
    void setLabelText(const juce::String& topText, const juce::String& bottomText);

private:
    // A list of registered listeners
    juce::ListenerList<Listener> listeners;

    // A map to hold the raw coordinates
    std::map<char, float> coordsRaw;

    // A map to hold the raw range values
    std::map<char, float> rangeRaw;

    // Set the raw range values based on the current component bounds
    void setRangeRaw();

    // Initialize the coordinates with raw values
    void initCoords(float rawX, float rawY);

    // Set the coordinates with raw values, if within range
    void setCoords(float rawX, float rawY);

    // Update the coordinates based on the range and the component size
    void updateCoords();

    // Positional settings variables
    float midY, midX, left, right, top, bottom;

    // Function to set positional settings based on component bounds
    void setSettings();

    // User settings
    int gridLineCount; // The number of grid lines
    std::map<char, float> range; // A map to hold the range values

    // Drawing methods
    void drawPlot(juce::Graphics& g);
    void drawAxis(juce::Graphics& g);
    void drawGrid(juce::Graphics& g);
    void drawMarker(juce::Graphics& g);
    void drawText(juce::Graphics& g);

    // Indicates if the marker was moved
    bool markerMoved{ false };

    // Method to constrain coordinates within a range
    float constrain(float coord);

    // Method to invert coordinates within a given range
    float invertCoord(float coord, float min, float max);

    // Method to check if the raw coordinates are within range
    bool inRangeRaw(float rawX, float rawY);

    // Label components for displaying text at the top and bottom of the plot
    juce::Label topLabel;
    juce::Label bottomLabel;

    // Macro to declare non-copyable class and leak detector
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CoordinatePlot)
};
