/*

ICS_TextBox

	Created: ???

	Change log:

		2020-03-22
			- flip y coordinates... top left of the screen is (0, 0)

		2020-04-01
			- added options for handling text that is longer then the width of the text box (wrap, truncate, auto fit)

		2020-04-12
			- use ICS_Font (FreeType font) instead of ICS_TextRenderer (raster font)
			- added and used struct for pairs of text and colors instead of maintaining separate lists (to simplify code and reduce chance of error)
			- optimized rendering by rendering many characters of the same color at once rather than one at a time
			- got rid of max lines
			- added the ability to regenerate text lines when the box resizes

		2020-04-13
			- added line spacing

		2020-04-17
			- use stencil test instead of scissor test so ICS_TextBox can be transformed

		2023-01-20
			- removed font initialization which is now done automatically by the game

		2024-01-24
			- added function for setting the default text color
			- added ability to make text transparent
			- added ability to add a background to the textbox (solid color or image from file)

		2024-01-25
			- ICS_TextBox inherits from ICS_TextRenderable

*/

#pragma once

#include "ICS_TextRenderable.h"	// the definition of ICS_TextRenderable (ICS_TextBox inherits from ICS_TextRenderable)
#include "ICS_ColoredText.h"	// the definition of ICS_ColoredText (displayed text can be any color)

#include <vector>				// the definition of std::vector (a variable length array)
#include <sstream>				// for converting various types to string

class ICS_Font;					// for rendering text
class ICS_Sprite;				// for rendering a background for the textbox

/**
 * A rectangular screen area for rendering multiple lines of text.
 * Can use mouse wheel to scroll up and down.
 */
class ICS_TextBox: public ICS_TextRenderable
{

private:

	int _fontSize;										// the height of the font in pixels
	int _lineSpacing;									// the number of pixels between each line

	float _scroll;										// for scrolling the text up and down
	float _minScroll;									// the minimum value scroll can be
	float _maxScroll;									// the maximum value scroll can be

	bool _fillDown;										// indicates that text will be drawn at the top of the text box when it is not full
	int _wrapMethod;									// indicates that text should be wrapped at the end of line rather than truncated

	std::vector<ICS_ColoredText> _rawText;				// all text to be rendered
	std::vector<std::vector<ICS_ColoredText>> _lines;	// the text to be rendered, separated into lines

	bool _newLine;										// indicates that a new line should be inserted before the next text is added
	bool _regenerateLines;								// indicates that the lines of text need to be regenerated
	bool _calculateScrollLimits;						// indicates that the scroll limits need to be recalculated

public:

// constructor / destructor

	/**
	 * The ICS_TextBox constructor.
	 *
	 * @param fontFileName	The name of the file containing the font.
	 * @param fontHeight	The height of the font in pixels.
	 * @param lineSpacing	The number of pixels between each line.
	 * @param width			The width of the text box in pixels. 
	 * @param height		The height of the text box in pixels.
	 * @param fillDown		Indicates that the lines of text should start at the top of the box, rather than the bottom.
	 */
	ICS_TextBox(std::string fontFileName, int fontHeight, int lineSpacing, int width, int height, bool fillDown);

// setters

	/**
	 * Sets the method of handling text that is too long to fit in the text box.
	 *
	 * @param method		ICS_WRAP (continue text on the next line),
	 *						ICS_TRUNCATE (cut text off),
	 *						or ICS_AUTOFIT (automatically resize textbox to fit the text)
	 */
	void setWrapMethod(int method)
	{
		// make sure the method changed and it is valid
		if (method != _wrapMethod && (method == ICS_WRAP || method == ICS_TRUNCATE || method == ICS_AUTO_FIT))
		{
			_wrapMethod = method;		// set the method
			_regenerateLines = true;	// regenerate the lines
		}
	}

// mutators

	/**
	 * Writes to the text box.
	 *
	 * @param data		The data to write to the text box.
	 * @param color		The color to render the data in.
	 */
	template <typename T>
	void write(T data, const ICS_Color& color)
	{
		// write the data to a string stream
		std::stringstream ss;
		ss << data;

		// copy the data to a string
		std::string text(std::istreambuf_iterator<char>(ss), {});

		// is the previous text the same color? just append it
		if (_rawText.size() > 0 && color == _rawText[_rawText.size() - 1].color)
		{
			_rawText[_rawText.size() - 1].text += text;
		}

		// otherwise, add a new piece of text
		else
		{
			_rawText.push_back(ICS_ColoredText(text, color));
		}

		// add the text to the lines
		addToLines(ICS_ColoredText(text, color));
	}

	/**
	 * Writes to the text box using the current color.
	 *
	 * @param data		The data to write to the text box.
	 */
	template <typename T>
	void write(T data)
	{
		write(data, _color);
	}

	/**
	 * Writes to the text box.
	 *
	 * @param data		The data to write to the text box.
	 * @param red		The red component of the color to render the data in.
	 * @param green		The green component of the color to render the data in.
	 * @param blue		The blue component of the color to render the data in.
	 * @param alpha		The alpha component of the color to render the data in.
	 */
	template <typename T>
	void write(T data, int red, int green, int blue, int alpha = ICS_COLOR_MAX)
	{
		write(data, ICS_Color(red, green, blue, alpha));
	}

	/**
	 * Writes to the text box using the current color.
	 *
	 * @param data		The data to write to the text box.
	 */
	template <typename T>
	ICS_TextBox& operator<<(T data)
	{
		write(data, _color);
		return *this;
	}

	/**
	 * Clears all text from the text box
	 */
	void clear();

// event handlers

	/**
	 * This handles mouse wheel events that occur when the cursor is over the renderable.
	 *
	 * @param mouseX			The x position of the mouse cursor.
	 * @param mouseY			The y position of the mouse cursor.
	 * @param wheelRotation		The wheel rotation value.  A positive value indicates that the wheel was rotated forward, away
	 *							from the user; a negative value indicates that the wheel was rotated backward, toward the user.
	 */
	void handleMouseWheelOver(float mouseX, float mouseY, int wheelRotation);

private:

// helpers

	/**
	 * Sets the dimensions of the text box.
	 * This function is a catch-all for dimension changes.
	 * If the text box's dimensions change, this function will be called.
	 *
	 * @param dimensions	The dimensions to assign to the text box.
	 */
	void _setDimensions(const ICS_Pair<float>& dimensions);

	/**
	 * Adds text to the lines which have already been generated
	 *
	 * @param data		The text to add to the lines.
	 */
	void addToLines(const ICS_ColoredText& newText);

	/**
	 * Separates the raw text into lines for rendering.
	 */
	void regenerateLines();

	/**
	 * Updates the min and max scroll values based on the number of lines of text in the text box.
	 */
	void calculateScrollLimits();

	/**
	 * Renders the text box.
	 */
	void render();

};
