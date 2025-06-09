#include "ICS_Game.h"	// the definition of ICS_Game
#include "ICS_Text.h"	// the definition of ICS_TextField
#include "ICS_Font.h"	// the definition of ICS_Font

#include <Windows.h>	// for key codes
#include <algorithm>	// for erase and remove

/**
 * The ICS_Text constructor.
 *
 * @param fontFileName	The name of the file containing the font.
 * @param fontHeight	The height of the font in pixels.
 */
ICS_Text::ICS_Text(std::string fontFileName, int fontHeight)
	:
	ICS_TextRenderable(fontFileName, fontHeight, 0, 0),
	_clip(false),
	_clipWidth(0)
{
}

/**
 * The ICS_Text constructor.
 *
 * @param font	The font to use when rendering text.
 */
ICS_Text::ICS_Text(ICS_Font* font)
	:
	ICS_TextRenderable(font, 0, 0),
	_clip(false),
	_clipWidth(0)
{
}

/**
 * Sets the text to be displayed.
 *
 * @param text		The text to be displayed.
 */
void
ICS_Text::setText(std::string text)
{
	// set the text to render
	_text = text;

	// set the width and height of the renderable based on the text to render
	setDimensions(_font->getTextWidth(_text), _font->getHeight());
}

/**
 * Renders the text.
 */
void
ICS_Text::render()
{
	// determine the text to render
	std::string text = _text;

	// if clipping is enabled, clip the text until it fits in the specified width
	if (_clip)
	{
		// as long as the text does not fit, keep clipping characters
		while (_font->getTextWidth(text) > _clipWidth and text.length() > 0)
		{
			text = text.substr(0, text.length() - 1);
		}
	}

	// set the width and height of the renderable based on the text to render
	setDimensions(_font->getTextWidth(text), _font->getHeight());

	// set the color
	_color.setRenderColor();

	// render the text using the font
	_font->render(0, 0, text);
}
