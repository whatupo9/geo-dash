/*

ICS_Text

	Created: 2023-01-30

	Change log:

		2023-01-30
			- created class to represent a single line of text like a score or timer

		2024-01-25
			- ICS_TextField inherits from ICS_TextRenderable

		2024-01-26
			- added getText to get the text being displayed

		2024-02-04
			- added a constructor which takes a font as input

		2024-02-10
			- added ability to clip characters that don't fit within a specified width (useful for buttons)

*/

#pragma once

#include "ICS_TextRenderable.h"		// the definition of ICS_TextRenderable (ICS_TextField inherits from ICS_TextRenderable)

// forward declarations
class ICS_Font;

/**
 * For rendering a single line of text on the screen.
 */
class ICS_Text: public ICS_TextRenderable
{

private:

	std::string _text;	// the text to render

	bool _clip;			// indicates the text should be clipped if it doesn't fit in the specified width
	float _clipWidth;	// the width to use when clipping is enabled

public:

// constructor / destructor

	/**
	 * The ICS_Text constructor.
	 *
	 * @param fontFileName	The name of the file containing the font.
	 * @param fontHeight	The height of the font in pixels.
	 */
	ICS_Text(std::string fontFileName, int fontHeight);

	/**
	 * The ICS_Text constructor.
	 *
	 * @param font	The font to use when rendering text.
	 */
	ICS_Text(ICS_Font* font);

// modifiers

	/**
	 * Enables clipping of text.  Text will be clipped when it does not fit in the specified width.
	 *
	 * @param width		The width to use when clipping is enabled.
	 */
	void enableClippling(float width)
	{
		_clip = true;
		_clipWidth = width;
	}

// setters

	/**
	 * Sets the text to be displayed.
	 *
	 * @param text		The text to be displayed.
	 */
	void setText(std::string text);

// getters

	/**
	 * Gets the text being displayed.
	 *
	 * @returns		The text being displayed.
	 */
	std::string getText()
	{
		return _text;
	}

private:

// helpers

	/**
	 * Renders the text.
	 */
	void render();

};