/*

ICS_TextRenderable

	Created: 2024-01-25

	Change log:

		2024-01-25
			- created class to represent a renderable that uses a font to render text on the screen

		2024-02-04
			- added a constructor which takes a font as input

		2024-05-07
			- fixed background positioning bug introduced by fixing anchor points in ICS_Renderable

		2024-06-06
			- added copy constructor and assignment operator

*/

#pragma once

#include "ICS_Renderable.h"		// the definition of ICS_Renderable (ICS_TextRenderable inherits from ICS_Renderable)
#include "ICS_ColoredText.h"	// the definition of ICS_ColoredText (displayed text can be any color)

#include <vector>				// the definition of std::vector (a variable length array)
#include <sstream>				// for converting various types to string

class ICS_Font;					// for rendering text
class ICS_Sprite;				// for rendering a background for the textbox

/**
 * A rectangular screen area for rendering multiple lines of text.
 * Can use mouse wheel to scroll up and down.
 */
class ICS_TextRenderable: public ICS_Renderable
{

protected:

	ICS_Font* _font;			// for rendering text
	ICS_Sprite* _background;	// for rendering a background

public:

// constructors / destructor

	/**
	 * The ICS_TextRenderable constructor.
	 *
	 * @param fontFileName	The name of the file containing the font.
	 * @param fontHeight	The height of the font in pixels.
	 * @param width			The width of the text box in pixels.
	 * @param height		The height of the text box in pixels.
	 */
	ICS_TextRenderable(std::string fontFileName, int fontHeight, int width, int height);

	/**
	 * The ICS_TextRenderable constructor.
	 *
	 * @param font		The font to use when rendering text.
	 * @param width		The width of the text box in pixels.
	 * @param height	The height of the text box in pixels.
	 */
	ICS_TextRenderable(ICS_Font* font, int width, int height);

	/**
	 * The ICS_TextRenderable copy constructor.
	 *
	 * @param textRenderable	The text renderable to copy.
	 */
	ICS_TextRenderable(const ICS_TextRenderable& textRenderable);

	/**
	 * The ICS_TextRenderable destructor.
	 */
	~ICS_TextRenderable();

// assignment operator

	/**
	 * The assignment operator.
	 *
	 * @param textRenderable	The text renderable to copy.
	 * 
	 * @returns					A reference to this instance.
	 */
	ICS_TextRenderable& operator=(const ICS_TextRenderable& textRenderable);

// optional initialization

	/**
	 * Adds a background sprite to the textbox.
	 *
	 * @param color		The color to render the background.
	 */
	void addBackground(const ICS_Color& color);

	/**
	 * Adds a background sprite to the textbox.
	 *
	 * @param red		The red component of the color to render the background in.
	 * @param green		The green component of the color to render the background in.
	 * @param blue		The blue component of the color to render the background in.
	 * @param alpha		The alpha component of the color to render the background in.
	 */
	void addBackground(int red, int green, int blue, int alpha = ICS_COLOR_MAX)
	{
		addBackground(ICS_Color(red, green, blue, alpha));
	}

	/**
	 * Adds a background sprite to the textbox.
	 *
	 * @param imageFileName		The name of the file containing the image.
	 */
	void addBackground(const std::string& imageFileName);

protected:

// transformation helpers

	/**
	 * Sets the anchor of the renderable.
	 * This function is a catch-all for anchor changes.
	 * If the renderable's anchor changes, this function will be called.
	 *
	 * @param anchor	The anchor to assign to the renderable.
	 */
	virtual void _setAnchor(ICS_Pair<float> anchor);

	/**
	 * Sets the dimensions of the renderable.
	 * This function is a catch-all for dimension changes.
	 * If the renderable's dimensions change, this function will be called.
	 *
	 * @param dimensions	The dimensions to assign to the renderable.
	 */
	void _setDimensions(const ICS_Pair<float>& dimensions);

};
