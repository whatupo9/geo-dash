/*

ICS_Sprite

	Created: ???

	Change log:

		2020-03-22
			- changed sprite anchor point from lower left to center
			- added texture coordinates
			- removed renderQuad (no longer needed)
			- flip texture vertically to match screen coordinates change (top left is the origin)

		2020-03-23
			- use ICS_Quad for texture coordinates
			- added getter for texture coordinates
			- overloaded setter for texture coordinates using ICS_Quad<double> as input

		2020-03-24
			- made render function independent of event handling (for manual or automatic rendering)
			- moved width and height getters and setters to the parent class

		2020-04-17
			- added per-pixel accuracy for mouse interaction (isMouseOver)
			- removed vertical flip and horizontal flip (same effect can be accomplished with scaling)

		2020-05-22
			- added ability to create a sprite from raw image data

		2023-01-25
			- moved function for rotating the sprite to ICS_Renderable

		2023-01-26
			- added function to calculate texture coordinates (useful for sprite sheets)

		2023-01-28
			- sprites are now initialized when they are constructed instead of after the engine is ready
			- added default constructor and a constructor which copies the dimensions of the texture
			- removed texture coordinates as animated sprites now use tilesets

		2023-05-31
			- fixed bug where sprites created with the default constructor have a width and height of 0

		2024-01-15
			- now the sprites assigned color is used when rendering a sprite without a texture (instead of magenta)

		2024-01-19
			- added a constructor to make a sprite with no texture (a colored rectangle)
			- added ability to render a greyscale version of the sprite

		2024-01-23
			- added a function to assign a texture to the sprite.
			- added a constructor which takes a texture as input.

		2024-01-23
			- added a function to remove the texture from the sprite.

		2024-01-27
			- removed moveForward function (should be in a subclass)

		2024-02-10
			- improved rendering by applying common OpenGL settings to the root node instead of each time a sprite is rendered.

		2024-03-21
			- overrode ICS_Renderable::applyTransformations so dimensions could be determined from the texture before rendering (fixed a glitch)

		2024-05-05
			- removed feature that used the texture's dimensions for the sprite (too messy for an unnecessary feature)

		2024-05-07
			- fixed bug with isMouseOver function not correctly detecting when the mouse coordinates are over a child
			- isMouseOver now requires an extra parameter to indicate whether the coordinates are in parent space or global space

		2024-05-09
			- modified isMouseOver to support anchor points that can be applied to children or not

		2024-06-06
			- fixed bug where ICS_Renderable copy constructor was not being called by the ICS_Sprite copy constructor

*/

#pragma once

#include <string>			// for string

#include "ICS_Renderable.h"	// the definition of ICS_Renderable (ICS_Sprite inherits from ICS_Renderable)
#include "ICS_Texture.h"	// the definition of ICS_Texture (a texture which can be applied to a polygon)
#include "ICS_Pair.h"		// the definition of ICS_Pair (for scale)

/**
 * For rendering images to the game window.
 */
class ICS_Sprite: public ICS_Renderable
{

protected:

// member attributes

	ICS_Texture* _texture;	// the texture to apply to the sprite

public:

// constructors / destructor

	/**
	 * ICS_Sprite default constructor.
	 */
	ICS_Sprite();

	/**
	 * ICS_Sprite constructor which loads the image data from file.
	 *
	 * @param fileName	The name of the file containing the image.
	 * @param width		The width to render the sprite in pixels.
	 * @param height	The height to render the sprite in pixels.
	 */
	ICS_Sprite(std::string fileName, float width, float height);

	/**
	 * ICS_Sprite constructor which takes a texture as input.
	 *
	 * @param texture	The texture to use for the sprite.
	 * @param width		The width to render the sprite in pixels.
	 * @param height	The height to render the sprite in pixels.
	 */
	ICS_Sprite(ICS_Texture* texture, float width, float height);

	/**
	 * ICS_Sprite constructor which is initialized with raw image data.
	 *
	 * @param imageData		The raw image data.
	 * @param width			The width to render the sprite in pixels.
	 * @param height		The height to render the sprite in pixels.
	 * @param channels		The number of channels in the image data.
	 */
	ICS_Sprite(unsigned char* imageData, float width, float height, int channels);

	/**
	 * ICS_Sprite constructor which does not create a texture.
	 *
	 * @param color		The color to render the sprite.
	 * @param width		The width to render the sprite (in pixels).
	 * @param height	The height to render the sprite (in pixels).
	 */
	ICS_Sprite(const ICS_Color& color, float width, float height);

	/**
	 * ICS_Sprite copy constructor.
	 *
	 * @param sprite	The sprite to copy.
	 */
	ICS_Sprite(const ICS_Sprite& sprite);

	/**
	 * ICS_Sprite destructor.
	 */
	~ICS_Sprite();

// assignment operator

	/**
	 * The assignment operator.
	 *
	 * @param sprite	The instance to copy.
	 * 
	 * @returns			A reference to this instance.
	 */
	ICS_Sprite& operator=(const ICS_Sprite& sprite);

// texture modifiers

	/**
	 * Attempts to load an image from the specified file.
	 * If an image is already loaded, it will be replaced.
	 *
	 * @param fileName	The name of the file containing the image
	 */
	void loadImage(std::string fileName);

	/**
	 * Assigns a texture to the sprite.
	 *
	 * @param texture	The texture to assign to the sprite.
	 */
	void setTexture(ICS_Texture* texture);

	/**
	 * Removes the texture from the sprite.
	 */
	void removeTexture();

// getters

	/**
	 * Gets the name of the file used for the sprite.
	 *
	 * @returns		The name of the file containing the image for the sprite.
	 */
	std::string getImageFileName() const
	{
		if (_texture)
		{
			return _texture->getFileName();
		}

		return "";
	}

// inquiry

	/**
	 * Checks if the mouse coordinates are over the sprite or not.
	 *
	 * @param x			The x component of the mouse coordinates.
	 * @param y			The y component of the mouse coordinates.
	 * @param global	Indicates that the coordinates are in global space (not parent space).
	 *
	 * @returns			true if the coordinates are over the sprite, false if not.
	 */
	bool isMouseOver(float x, float y, bool global = true) const;

// helpers

protected:

	/**
	 * Renders the sprite.
	 */
	void render();

};
