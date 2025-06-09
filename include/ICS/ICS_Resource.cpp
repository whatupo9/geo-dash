#include "ICS_Resource.h"	// the declaration of ICS_Resource
#include "ICS_Game.h"		// ICS_Resource objects are added and removed from the game automatically

/**
 * The ICS_Resource constructor.
 */
ICS_Resource::ICS_Resource()
	:
	_initialized(false)
{
	// start recieving game initialized events
	ICS_Game::getInstance().addGameInitializedEventListener(this);
}

/**
 * Handles game initialized events.  Initializes the resource.
 */
void
ICS_Resource::handleGameInitializedEvent()
{
	// perform initialization
	initialize();

	// stop recieving game initialized events
	ICS_Game::getInstance().removeGameInitializedEventListener(this);
}