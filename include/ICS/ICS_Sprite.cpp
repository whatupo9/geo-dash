#include "ICS_Game.h"		// the ICS_Game definition
#include "ICS_Sprite.h"		// the declaration of ICS_Sprite
#include "ICS_Helpers.h"	// for ICS_swap

#include <glut.h>			// the library for glut (OpenGL)

/**
 * ICS_Sprite default constructor.
 */
ICS_Sprite::ICS_Sprite()
	:
	_texture(NULL)
{
}

/**
 * ICS_Sprite constructor which loads the image data from file.
 *
 * @param fileName	The name of the file containing the image.
 * @param width		The width to render the sprite in pixels.
 * @param height	The height to render the sprite in pixels.
 */
ICS_Sprite::ICS_Sprite(std::string fileName, float width, float height)
	:
	_texture(ICS_Texture::createTexture(fileName))
{
	// set the dimensions
	setDimensions(width, height);
}

/**
 * ICS_Sprite constructor which takes a texture as input.
 *
 * @param texture	The texture to use for the sprite.
 * @param width		The width to render the sprite in pixels.
 * @param height	The height to render the sprite in pixels.
 */
ICS_Sprite::ICS_Sprite(ICS_Texture* texture, float width, float height)
	:
	_texture(ICS_Texture::copyTexture(texture))
{
	// set the dimensions
	setDimensions(width, height);
}

/**
 * ICS_Sprite constructor which is initialized with raw image data.
 *
 * @param imageData		The raw image data.
 * @param width			The width to render the sprite in pixels.
 * @param height		The height to render the sprite in pixels.
 * @param channels		The number of channels in the image data.
 */
ICS_Sprite::ICS_Sprite(unsigned char* imageData, float width, float height, int channels)
	:
	_texture(ICS_Texture::createTexture(imageData, width, height, channels))
{
	// set the dimensions
	setDimensions(width, height);
}

/**
 * ICS_Sprite constructor which does not create a texture.
 *
 * @param color		The color to render the sprite.
 * @param width		The width to render the sprite (in pixels).
 * @param height	The height to render the sprite (in pixels).
 */
ICS_Sprite::ICS_Sprite(const ICS_Color& color, float width, float height)
	:
	_texture(NULL)
{
	// set the color
	setColor(color);

	// set the dimensions
	setDimensions(width, height);
}

/**
 * ICS_Sprite copy constructor.
 *
 * @param sprite	The sprite to copy.
 */
ICS_Sprite::ICS_Sprite(const ICS_Sprite& sprite)
	:
	ICS_Renderable(sprite),
	_texture(ICS_Texture::copyTexture(sprite._texture))
{
}

/**
 * ICS_Sprite destructor.
 */
ICS_Sprite::~ICS_Sprite()
{
	// delete the texture
	ICS_Texture::deleteTexture(_texture);
}

/**
 * The assignment operator.
 *
 * @param sprite	The instance to copy.
 * 
 * @returns			A reference to this instance.
 */
ICS_Sprite&
ICS_Sprite::operator=(const ICS_Sprite& sprite)
{
	// invoke the parent assignment operator
	ICS_Renderable::operator=(sprite);

	// replace the texture
	ICS_Texture::deleteTexture(_texture);
	_texture = ICS_Texture::copyTexture(sprite._texture);

	// return this object
	return *this;
}

/**
 * Attempts to load an image from the specified file.
 * If an image is already loaded, it will be replaced.
 *
 * @param fileName	The name of the file containing the image
 */
void
ICS_Sprite::loadImage(std::string fileName)
{
	// replace the texture
	ICS_Texture::deleteTexture(_texture);
	_texture = ICS_Texture::createTexture(fileName);
}

/**
 * Assigns a texture to the sprite.
 *
 * @param texture	The texture to assign to the sprite.
 */
void
ICS_Sprite::setTexture(ICS_Texture* texture)
{
	// replace the texture
	ICS_Texture::deleteTexture(_texture);
	_texture = ICS_Texture::copyTexture(texture);
}

/**
 * Removes the texture from the sprite.
 */
void
ICS_Sprite::removeTexture()
{
	ICS_Texture::deleteTexture(_texture);
	_texture = NULL;
}

/**
 * Checks if the mouse coordinates are over the sprite or not.
 *
 * @param x			The x component of the mouse coordinates.
 * @param y			The y component of the mouse coordinates.
 * @param global	Indicates that the coordinates are in global space (not parent space).
 *
 * @returns			true if the coordinates are over the sprite, false if not.
 */
bool
ICS_Sprite::isMouseOver(float x, float y, bool global) const
{
	// if the renderable is not visible or the mouse is being ignored, ignore the event
	if (not _visible or _ignoreMouse)
	{
		return false;
	}

	// transform the coordinates into local space
	if (global)
	{
		globalToLocal(x, y);
	}
	else
	{
		inverseTransform(x, y);
	}

	// check if the coordinates are over a child
	if (isMouseOverChild(x, y))
	{
		return true;
	}

	// apply the anchor transformation after checking mouse over children
	anchorInverseTransform(x, y);

	// if there is a texture, check tha alpha value of the pixel
	if (_texture and getWidth() > 0 and getHeight() > 0)
	{
		// convert to texture coordinates
		int textureX = (int)(x * _texture->getWidth() / getWidth());
		int textureY = (int)(y * _texture->getHeight() / getHeight());

		// check if the mouse is over a non-transparent pixel
		return (_texture->getAlpha(textureX, textureY) > 0);
	}

	// no texture?  check if the coordinates are over the quad
	else
	{
		return (x >= 0.0f and x <= getWidth() and y >= 0.0f and y <= getHeight());
	}
}

/**
 * Renders the sprite.
 */
void
ICS_Sprite::render()
{
	// set the drawing color
	_color.setRenderColor();

	// bind the texture so it will be applied to the polygon (enable textures)
	if (_texture and _texture->bind())
	{
		glEnable(GL_TEXTURE_2D);
	}

	// if the texture could not be bound disable textures
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	// render the quad
	glBegin(GL_QUADS);
	{
		glTexCoord2d(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);

		glTexCoord2d(1.0f, 0.0f);
		glVertex2f(getWidth(), 0.0f);

		glTexCoord2d(1.0f, 1.0f);
		glVertex2f(getWidth(), getHeight());

		glTexCoord2d(0.0f, 1.0f);
		glVertex2f(0.0f, getHeight());
	}
	glEnd();
}