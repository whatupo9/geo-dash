/*

ICS_EventListener

	Created: 2024-02-17

	Change log:

		20224-02-17
			- created ICS_EventListener to represent a game element that listens for events.

*/

#pragma once

/**
 *  A game element that listens for update events.
 */
class ICS_EventListener
{
	friend class ICS_Game;	// allow the game to access private members

public:

// constructor

	/**
	 *  The ICS_EventListener constructor.
	 */
	ICS_EventListener()
	{
	}

private:

// event handlers

	/**
	 * Handles game initialized events.
	 */
	virtual void handleGameInitializedEvent()
	{
	}

	/**
	 * Handles update events.
	 *
	 * @param elapsed	The number of seconds that have elapsed since the last update.
	 */
	virtual void handleUpdateEvent(float elapsed)
	{
	}

	/**
	 * This handles keyboard events.
	 *
	 * @param key			The id of the key that was pressed / released.  Key constants can be found in ICS_Constants.h
	 * @param eventType		The type of event (ICS_EVENT_PRESS or ICS_EVENT_RELEASE)
	 */
	virtual void handleKeyboardEvent(int key, int eventType)
	{
	}

	/**
	 * This handles mouse movement events.
	 *
	 * @param mouseX	The x position of the mouse cursor.
	 * @param mouseY	The y position of the mouse cursor.
	 */
	virtual void handleMouseMove(float mouseX, float mouseY)
	{
	}

	/**
	 * This is called when a mouse button is pressed or released.
	 *
	 * @param button		The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param x				The x component of the mouse cursor position.
	 * @param y				The y component of the mouse cursor position.
	 * @param eventType		The type of event (ICS_EVENT_PRESS or ICS_EVENT_RELEASE)
	 */
	virtual void handleMouseButtonEvent(int button, float x, float y, int eventType)
	{
	}

};