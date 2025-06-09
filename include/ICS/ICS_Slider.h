/*

ICS_Slider

	Created: 2020-03-26

	Change log:

		2020-03-26
			- created ICS_Slider for sliders and scrollbars used in GUIs

		2020-03-28
			- added variable and setter for the thumb size relative to the track size (_thumbScale)
			- use mouse offset so bar doesn't jump when clicked on

		2020-03-30
			- added optional sprite to represent the track behind the thumb

		2020-03-31
			- thumb uses ICS_Button so it will animate when hovering or clicking

		2024-01-19
			- center the thumb on the mouse coordinates when the track is clicked
			- added a track sprite by default (texture no longer required)

		2024-01-21
			- a texture is no longer required for the thumb (a color can be specified instead)

		2024-01-27
			- added mouse wheel support for sliding

		2024-01-29
			- added a function to set the slider value

		2024-02-03
			- removed thumb colors from constructor
			- added functions to set the thumb and track colors

		2024-02-17
			- inherits from ICS_EventListener so it can receive mouse movement and button events

		2024-05-05
			- set anchor point to the top left by default
			- calculate thumb scale based on initial dimensions passed in
			- fixed "mouse move" callback (inverse transformation was being applied twice)
			- the class still needs a proper copy constructor and assignment operator

		2024-06-07
			- added copy constructor and assignment operator

*/

#pragma once

#include "ICS_Renderable.h"		// ICS_Slider inherits from ICS_Renderable
#include "ICS_EventListener.h"	// ICS_Slider inherits from ICS_EventListener

class ICS_Sprite;	// declare ICS_Sprite (used for the visual representation of the track)
class ICS_Button;	// declare ICS_Button (used for the visual representation of the thumb)

/**
 * A slider (scrollbar) for use in a 2D GUI.
 */
class ICS_Slider : public ICS_Renderable, public ICS_EventListener
{

private:

// member attributes

	int _type;						// the type of slider (ICS_HORIZONTAL or ICS_VERTICAL)

	ICS_Sprite* _trackSprite;		// add a sprite for the track behind the thumb

	ICS_Button* _thumbSprite;		// the visual representation of the thumb (the element the user clicks on)
	float _thumbScale;				// the amount to scale the sprite relative to the track

	bool _sliding;					// indicates the slider is currently being moved
	float _mouseOffset;				// the offeset between the mouse coordinates and sprite position when sliding

	float _sliderValue;				// the value of the slider (0 is the min and 1 is the max)

public:

// constructors / destructor

	/**
	 * The constructor.
	 *
	 * @param type			The type of slider (ICS_HORIZONTAL or ICS_VERTICAL).
	 * @param thumbSize		The size of the thumb sprite (in pixels).
	 * @param trackLength	The length of the track (in pixels).
	 */
	ICS_Slider(int type, float thumbSize, float trackLength);

	/**
	 * The copy constructor.
	 *
	 * @param slider	The slider to copy.
	 */
	ICS_Slider(const ICS_Slider& slider);

	/**
	 * The destructor.
	 */
	~ICS_Slider();

// assignment operator

	/**
	 * Assignment operator.
	 *
	 * @param slider	The slider to copy.
	 * 
	 * @returns			A reference to this instance.
	 */
	ICS_Slider& operator=(const ICS_Slider& slider);

// getters

	/**
	 * Gets the slider value.
	 *
	 * @returns		The slider value (0 is the min and 1 is the max).
	 */
	float getSliderValue()
	{
		return _sliderValue;
	}

// setters

	/**
	 * Sets the slider value.
	 *
	 * @param value		The value to assign to the slider (0 is the min and 1 is the max).
	 */
	void setSliderValue(float value);

	/**
	 * Sets the color of the track.
	 *
	 * @param color		The color to render the track.
	 */
	void setTrackColor(const ICS_Color& color);

	/**
	 * Sets the amount to scale the thumb relative to the track.
	 *
	 * @param value		The amount to scale the thumb by (between 0.0 and 1.0).
	 */
	void setThumbScale(float value);

	/**
	 * Sets the color of each state of the thumb.
	 *
	 * @param upColor		The color to render the thumb when it is not being interacted with.
	 * @param downColor		The color to render the thumb when it is being held.
	 * @param hoverColor	The color to render the thumb when the mouse cursor is over it.
	 */
	void setThumbColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor);

private:

// event handling

	/**
	 * This handles mouse movement events.
	 *
	 * @param mouseX	The x position of the mouse cursor.
	 * @param mouseY	The y position of the mouse cursor.
	 */
	void handleMouseMove(float mouseX, float mouseY);

	/**
	 * This handles mouse wheel events that occur when the cursor is over the renderable.
	 *
	 * @param mouseX			The x position of the mouse cursor.
	 * @param mouseY			The y position of the mouse cursor.
	 * @param wheelRotation		The wheel rotation value.  A positive value indicates that the wheel was rotated forward, away
	 *							from the user; a negative value indicates that the wheel was rotated backward, toward the user.
	 */
	void handleMouseWheelOver(float mouseX, float mouseY, int wheelRotation);

	/**
	 * This is called when a mouse button is pressed or released.
	 *
	 * @param button		The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param x				The x component of the mouse cursor position.
	 * @param y				The y component of the mouse cursor position.
	 * @param eventType		The type of event (ICS_EVENT_PRESS or ICS_EVENT_RELEASE)
	 */
	void handleMouseButtonEvent(int button, float x, float y, int eventType);

	/**
	 * This is called when a mouse button is pressed over the slider.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param mouseX	The x position of the mouse cursor.
	 * @param mouseY	The y position of the mouse cursor.
	 */
	void handleMousePressOver(int button, float mouseX, float mouseY);

protected:

// helpers

	/**
	 * Sets the dimensions of the slider.
	 * This function is a catch-all for dimension changes.
	 * If the slider's dimensions change, this function will be called.
	 *
	 * @param dimensions	The dimensions to assign to the slider.
	 */
	void _setDimensions(const ICS_Pair<float>& dimensions);

	/**
	 * Updates the slider value and sprite position based on the mouse coordinates.
	 *
	 * @param coordinates	The current coordinates of the mouse.
	 */
	void handleMouseEvent(ICS_Pair<float> coordinates);

	/**
	 * Updates the thumb sprite's scale and position.
	 */
	void updateThumb();
};
