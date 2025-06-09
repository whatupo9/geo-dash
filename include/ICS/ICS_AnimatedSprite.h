/*

ICS_AnimatedSprite

	Created: ???

	Change log:

		2020-03-22
			- removed renderQuad (ICS_Sprite now has texture coordinates)
			- added and used helper for calculating texture coordinates

		2023-01-28
			- now use ICS_Tileset for textures to eliminate texture bleeding in sprite sheets
			- removed texture coordinates as they are no longer used

		2024-02-17
			- inherits from ICS_EventListener so it can receive update events

		2024-06-06
			- added copy constructor and assignment operator

*/

#pragma once

#include "ICS_Sprite.h"			// the definition of ICS_Sprite this class inherits from
#include "ICS_EventListener.h"	// the definition of ICS_EventListener this class inherits from
#include "ICS_Animation.h"		// the definition of ICS_Animation (each animated sprite has a set of animations)

#include <map>					// for std::map (used to map names to animations)
#include <vector>				// for std::vector (used for animation frames)

class ICS_Tileset;				// the textures for the sprite are stored in a tileset

/**
 * For rendering animated images in the game window.
 */
class ICS_AnimatedSprite: public ICS_Sprite, public ICS_EventListener
{

private:

// member attributes

	int										_frame;				// the current frame of the animation
	float									_elapsed;			// the time that has elapsed for the current animation 

	ICS_Tileset*							_tileset;			// the tileset containing all the textures					

	std::map<std::string, ICS_Animation>	_animations;		// the set of animations, indexed by name
	ICS_Animation*							_currentAnimation;	// the current animation, if there is one (NULL otherwise)

public:

// constructors / destructor

	/**
	 * The constructor.
	 *
	 * @param fileName	The name of the file containing the sprite animation data.
	 * @param width		The width to render the sprite in pixels.
	 * @param height	The height to render the sprite in pixels.
	 */
	ICS_AnimatedSprite(std::string fileName, float width, float height);

	/**
	 * The copy constructor.
	 *
	 * @param sprite	The animated sprite to copy.
	 */
	ICS_AnimatedSprite(const ICS_AnimatedSprite& sprite);

	/**
	 * The destructor.
	 */
	~ICS_AnimatedSprite();

// assignment operator

	/**
	 * Assignment operator.
	 *
	 * @param sprite	The animated sprite to copy.
	 * 
	 * @returns			A reference to this instance.
	 */
	ICS_AnimatedSprite& operator=(const ICS_AnimatedSprite& sprite);

public:

	/**
	 * Plays an animation.
	 *
	 * @param name	The name of the animation to play.  The name must match the one specified in addAnimation.
	 */
	void playAnimation(std::string name);

private:

// event handlers

	/**
	 * Handles update events.
	 *
	 * @param elapsed	The number of seconds that have elapsed since the last update.
	 */
	void handleUpdateEvent(float elapsed);

// helpers

	/**
	 * This is used to assist in parsing animation data files.  It removes any comments and trailing white space from a line of text.
	 *
	 * @param text	The text to remove the comment from.
	 *
	 * @returns		A copy of the text with the comment and trailing white space removed.
	 */
	std::string removeComment(std::string text);

	/**
	 * Adds an animation to the sprite.
	 *
	 * @param name				The name to give to the animation... this is needed to play the animation.
	 * @param frames			The sequence of frame indexes that make the animation.
	 * @param framesPerSecond	The number of frame to render per second.
	 * @param loop				Indicates whether or not the animation should loop.
	 */
	void addAnimation(std::string name, std::vector<ICS_Pair<int>> frames, int framesPerSecond, bool loop);

	/**
	 * Gets the appropriate texture from the tileset based on the current animation.
	 */
	void updateTexture();

};
