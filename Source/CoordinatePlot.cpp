/*
  ==============================================================================
    AudioProcessorClass.h
    Created: 10 Sep 2023 12:29:54pm
    Author:  Ali
  ==============================================================================
*/

#include <JuceHeader.h>
#include "CoordinatePlot.h"
#include <iomanip>
#include <sstream>

//==============================================================================
CoordinatePlot::CoordinatePlot()
{
    // The constructor initializes grid settings, coordinate values, and child components
    // No inputs.
    // Outputs are internal settings necessary for the coordinate plot display and interaction.

    // Initialize grid line count, range and coordinates with default values
    setGridLineCount(); // Self-generated code
    setRange(); // Self-generated code
    initCoords(75.0f, 75.0f); // Self-generated code

    // Adding and making labels visible, and setting their justification type
    addAndMakeVisible(topLabel); // Self-generated code
    addAndMakeVisible(bottomLabel); // Self-generated code
    topLabel.setJustificationType(juce::Justification::centredTop); // Self-generated code
    bottomLabel.setJustificationType(juce::Justification::centredBottom); // Self-generated code
}

CoordinatePlot::~CoordinatePlot() {}
CoordinatePlot::Listener::Listener() {}
CoordinatePlot::Listener::~Listener() {}

void CoordinatePlot::paint(juce::Graphics& g)
{
    // This function paints the background, the plot, and the markers on the coordinate plot
    // Input: juce::Graphics& g, a reference to the graphics context that should be used to paint the component.
    // Output: The function modifies the graphics context to paint the necessary elements on the component.

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // Self-generated code

    // Drawing plot and marker with respective colours
    g.setColour(juce::Colours::grey);
    drawPlot(g); // Self-generated code

    g.setColour(juce::Colours::orange);
    drawMarker(g); // Self-generated code

    g.setColour(juce::Colours::white);
    if (markerMoved) { drawText(g); } // Self-generated code

    // Capture the raw range for resizing reference
    setRangeRaw(); // Self-generated code
}

void CoordinatePlot::resized()
{
    // This function updates settings and coordinates when the component is resized
    // No inputs.
    // Output: Updates internal settings necessary for adjusting the display to the new size.

    setSettings(); // Self-generated code
    updateCoords(); // Self-generated code

    // Set bounds for top and bottom labels
    topLabel.setBounds(getLocalBounds()); // Self-generated code
    bottomLabel.setBounds(getLocalBounds()); // Self-generated code
}

// This method is called when the mouse is pressed down on the component.
// Inputs:
//   - event: a reference to a juce::MouseEvent object representing the details of the mouse event
void CoordinatePlot::mouseDown(const juce::MouseEvent& event)
{
    // [Self-Generated] Debug statement to log the coordinates where the mouse was clicked. The getX() and getY() methods are used here but might need to be replaced with direct access to event.getMouseDownX() and event.getMouseDownY() to get the accurate clicked coordinates.
    DBG("Mouse Clicked on plot at: " << getX() << "," << getY());

    // [Self-Generated] Setting a flag indicating that the marker has moved
    markerMoved = true;

    // Changing the mouse cursor to NoCursor when mouse is pressed down
    setMouseCursor(juce::MouseCursor::NoCursor);

    // [Self-Generated] Setting the coordinates of the marker to the point where the mouse was clicked
    setCoords(float(event.getMouseDownX()), float(event.getMouseDownY()));

    // [Self-Generated] Invoking interaction with the components listening to this plot
    interactWithComponent();

    // Requesting a repaint of the component to reflect the new marker position
    repaint();
}

// This method is called when the mouse is dragged on the component.
// Inputs:
//   - event: a reference to a juce::MouseEvent object representing the details of the mouse event
void CoordinatePlot::mouseDrag(const juce::MouseEvent& event)
{
    // [Self-Generated] Debug statement to log the current coordinates of the drag event
    DBG("Mouse dragged to: " << getX() << ", " << getY());

    // Retrieving the current position of the mouse
    juce::Point<int> rawPos(event.getPosition());
    float rawX = float(rawPos.getX());
    float rawY = float(rawPos.getY());

    // [Self-Generated] Updating the coordinates of the marker as the mouse is dragged
    setCoords(rawX, rawY);

    // [Self-Generated] Invoking interaction with the components listening to this plot
    interactWithComponent();

    // Requesting a repaint of the component to reflect the new marker position
    repaint();
}

// This method is called when the mouse button is released on the component.
// Inputs:
//   - event: a reference to a juce::MouseEvent object representing the details of the mouse event
void CoordinatePlot::mouseUp(const juce::MouseEvent& event)
{
    // Restoring the mouse cursor to its normal state when the mouse button is released
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

// [Self-Generated] This method notifies all the listeners about the change in value of the plot
void CoordinatePlot::interactWithComponent()
{
    // Invoking the coordPlotValueChanged method on all the listeners with this plot as the argument
    listeners.call([this](Listener& l) { l.coordPlotValueChanged(this); });
}

// [Self-Generated] Method to add a listener to this plot
// Inputs:
//   - l: pointer to the listener object to be added
void CoordinatePlot::addListener(Listener* l)
{
    listeners.add(l);
}

// [Self-Generated] Method to remove a listener from this plot
// Inputs:
//   - l: pointer to the listener object to be removed
void CoordinatePlot::removeListener(Listener* l)
{
    listeners.remove(l);
}

// [Self-Generated] Method to get the constrained X coordinate of the marker
// Outputs:
//   - returns the constrained X coordinate of the marker
float CoordinatePlot::getX()
{
    return constrain(coordsRaw['x']);
}

// [Self-Generated] Method to get the inverted and constrained Y coordinate of the marker
// Outputs:
//   - returns the inverted and constrained Y coordinate of the marker
float CoordinatePlot::getY()
{
    return invertCoord(constrain(coordsRaw['y']), range['min'], range['max']);
}

// [Self-Generated] Method to set the count of grid lines on the plot, ensuring it is even
// Inputs:
//   - lineCount: the desired number of grid lines
void CoordinatePlot::setGridLineCount(int lineCount)
{
    // Ensuring that the lineCount is even by decrementing it if it is odd
    if (lineCount % 2 == 1) { --lineCount; }
    gridLineCount = lineCount;
}

// This function sets the range of values that can be displayed on the plot.
// [Self-Generated]
// Inputs:
//   - min: the minimum value of the range
//   - max: the maximum value of the range
void CoordinatePlot::setRange(float min, float max)
{
    range['min'] = min;
    range['max'] = max;
}

// This function initializes the coordinates of the plot to the given values.
// [Self-Generated]
// Inputs:
//   - rawX: the initial X coordinate
//   - rawY: the initial Y coordinate
void CoordinatePlot::initCoords(float rawX, float rawY)
{
    coordsRaw['x'] = rawX;
    coordsRaw['y'] = rawY;
}

// This function sets the coordinates if they are within the specified range.
// [Self-Generated]
// Inputs:
//   - rawX: the X coordinate to be set
//   - rawY: the Y coordinate to be set
void CoordinatePlot::setCoords(float rawX, float rawY)
{
    if (inRangeRaw(rawX, rawY)) { coordsRaw['x'] = rawX, coordsRaw['y'] = rawY; }
}

// This function updates the coordinates based on the initial range and the current size of the component.
// [Self-Generated]
void CoordinatePlot::updateCoords()
{
    // Computing the ratios based on the initial range
    double xRatio = double(coordsRaw['x'] / (rangeRaw['max'] - rangeRaw['min']));
    double yRatio = double(coordsRaw['y'] / (rangeRaw['max'] - rangeRaw['min']));

    // Calculating the new coordinates based on the current size and previous ratios
    float newX = float(right * xRatio);
    float newY = float(bottom * yRatio);

    setCoords(newX, newY);
}

// This function sets the raw range based on the local bounds of the component.
// [Self-Generated]
void CoordinatePlot::setRangeRaw()
{
    rangeRaw['min'] = getLocalBounds().getX();
    rangeRaw['max'] = getLocalBounds().getWidth();
}

// This function draws the plot, including an outline, axis, and grid.
// [Self-Generated]
// Inputs:
//   - g: a reference to a juce::Graphics object used for drawing
void CoordinatePlot::drawPlot(juce::Graphics& g)
{
    g.drawRect(getLocalBounds(), 3);// draw an outline around the component
    drawAxis(g);
    drawGrid(g);
}

// This function draws the x and y axis on the plot.
// [Self-Generated]
// Inputs:
//   - g: a reference to a juce::Graphics object used for drawing
void CoordinatePlot::drawAxis(juce::Graphics& g)
{
    // Drawing the x and y axis
    g.drawLine(left, midY, right, midY, 2);
    g.drawLine(midX, left, midX, bottom, 2);
}

// This function draws a grid on the plot.
// [Self-Generated]
// Inputs:
//   - g: a reference to a juce::Graphics object used for drawing
void CoordinatePlot::drawGrid(juce::Graphics& g)
{
    const float myDashLength[] = { 3, 3 };
    float offset = float(getLocalBounds().getWidth() / (gridLineCount + 2));

    for (int i = 0; i < (gridLineCount/2); ++i)
    {
        int d{ i + 1 }; //degrees away from axis
        //draw to left/right of Y-axis and top/bottom of X-axis
        g.drawDashedLine(juce::Line<float>(midX - offset * d, top, midX - offset * d, bottom),
            &myDashLength[0], 2, 1.0, 0);
        g.drawDashedLine(juce::Line<float>(midX + offset * d, top, midX + offset * d, bottom),
            &myDashLength[0], 2, 1.0, 0);
        g.drawDashedLine(juce::Line<float>(left, midY - offset * d, right, midY - offset * d),
            &myDashLength[0], 2, 1.0, 0);
        g.drawDashedLine(juce::Line<float>(left, midY + offset * d, right, midY + offset * d),
            &myDashLength[0], 2, 1.0, 0);
    }
}
// This function draws a marker on the plot.
// [Self-Generated]
// Inputs:
//   - g: a reference to a juce::Graphics object used for drawing
void CoordinatePlot::drawMarker(juce::Graphics& g)
{
    //set length of cursor
    float length = float(getLocalBounds().getWidth() / 15);

    //create lines
    juce::Line<float> lineH(juce::Point<float>(coordsRaw['x'] - length, coordsRaw['y']),
        juce::Point<float>(coordsRaw['x'] + length, coordsRaw['y']));
    juce::Line<float> lineV(juce::Point<float>(coordsRaw['x'], coordsRaw['y'] - length),
        juce::Point<float>(coordsRaw['x'], coordsRaw['y'] + length));
    
    //draw lines
    g.drawLine(lineH, 2.0f);
    g.drawLine(lineV, 2.0f);
}

// This function draws the x and y coordinates as text on the plot.
// [Self-Generated]
// Inputs:
//   - g: a reference to a juce::Graphics object used for drawing
void CoordinatePlot::drawText(juce::Graphics& g)
{
    g.setFont(float(getWidth()/12));
    int textHeight = int(g.getCurrentFont().getHeight());

    //Draw Y
    std::stringstream streamY;
    streamY << std::fixed << std::setprecision(2) << getY();
    g.drawText(streamY.str(), int(midX), int(top), int(midX), textHeight, 
        juce::Justification::centredLeft, true);

    //Draw X
    std::stringstream streamX;
    streamX << std::fixed << std::setprecision(2) << getX();
    g.drawText(streamX.str(), int(midX), int(midY), int(midX), textHeight, 
        juce::Justification::centredRight, true);
}
// This function recalculates all the settings, updating the variables representing different points and boundaries in the local bounds of the component.
// [Self-Generated]
void CoordinatePlot::setSettings()
{
    //recalculate all the settings
    midY = float(getLocalBounds().getCentreY());
    midX = float(getLocalBounds().getCentreX());
    left = float(getLocalBounds().getX());
    right = float(getLocalBounds().getRight());
    top = float(getLocalBounds().getY());
    bottom = float(getLocalBounds().getBottom());
}
// This function constrains a coordinate to be within a specific range, based on a transformation of ranges.
// [Self-Generated]
// Inputs:
//   - coord: the coordinate to be constrained
// Outputs:
//   - returns the constrained coordinate
float CoordinatePlot::constrain(float coord)
{
    float oldRangeMin = float(getLocalBounds().getX());
    float oldRangeMax = float(getLocalBounds().getWidth());
    float oldRange = oldRangeMax - oldRangeMin;
    float newRange = range['max'] - range['min'];

    float newValue = (((coord - oldRangeMin) * newRange) / oldRange) + range['min'];
    return newValue;
}
// This function inverts a coordinate within a specified range.
// [Self-Generated]
// Inputs:
//   - coord: the coordinate to be inverted
//   - min: the minimum value of the range
//   - max: the maximum value of the range
// Outputs:
//   - returns the inverted coordinate
/**Inverts coord within a range between min and max*/
float CoordinatePlot::invertCoord(float coord, float min, float max)
{
    return (min + max) - coord;
}
// This function checks if the raw coordinates are within the valid range of the plot.
// [Self-Generated]
// Inputs:
//   - rawX: the raw X coordinate to be checked
//   - rawY: the raw Y coordinate to be checked
// Outputs:
//   - returns true if the coordinates are within the range, false otherwise
bool CoordinatePlot::inRangeRaw(float rawX, float rawY)
{
    return (rawX >= left && rawX <= right && rawY >= top && rawY <= bottom);
}

void CoordinatePlot::setLabelText(const juce::String& topText, const juce::String& bottomText)
{
    // This function sets the text of the top and bottom labels in the coordinate plot.
    // Inputs: 
    //   const juce::String& topText - the text to be set for the top label
    //   const juce::String& bottomText - the text to be set for the bottom label
    // Output: Modifies the topLabel and bottomLabel components to display the specified text.

    topLabel.setText(topText, juce::dontSendNotification); // Self-generated code
    bottomLabel.setText(bottomText, juce::dontSendNotification); // Self-generated code

    // Disabling the interception of mouse clicks on both labels
    topLabel.setInterceptsMouseClicks(false, false); // Self-generated code
    bottomLabel.setInterceptsMouseClicks(false, false); // Self-generated code
}
