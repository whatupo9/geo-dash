/*

ICS_Resource

	Created: 2020-04-15

	Change log:

		2020-04-15
			- created class to represent objects that would like to receive updates from the game

		2023-01-27
			- renamed ICS_Updateable to ICS_Resource

		2024-01-23
			- automatically call initialize when the game engine is ready so child classes don't have to

		2024-02-17
			- inherits from ICS_EventListener so it can receive update events
			- changed class name from ICS_GameObject to ICS_Resource

*/

#pragma once

#include "ICS_EventListener.h"	// ICS_Resource inherits from ICS_EventListener

/**
 * Represents an non-renderable object that needs initialization and / or regular updating.
 */
class ICS_Resource: public ICS_EventListener
{

protected:

// attributes

	bool _initialized;	// indicates if the game object is initialized or not

public:

// constructor / destructor

	/**
	 * The ICS_Resource constructor.
	 */
	ICS_Resource();

// inquiry

	/**
	 * Checks if font is initialized or not.
	 *
	 * @returns		true if the font is initialized, false if not.
	 */
	bool isInitialized()
	{
		return _initialized;
	}

private:

// event handling

	/**
	 * Handles game initialized events.  Initializes the resource.
	 */
	void handleGameInitializedEvent();
	
// helpers

protected:

	/**
	 * Prepares the object for use.  Should be called after the graphics pipeline is initialized and before the object is used.
	 */
	virtual void initialize()
	{
	}

};