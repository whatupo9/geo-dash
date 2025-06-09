#include "ICS_Game.h"		// the definition of ICS_Game
#include "ICS_TextBox.h"	// the definition of ICS_TextBox
#include "ICS_Font.h"		// the definition of ICS_Font
#include "ICS_Sprite.h"		// the definition of ICS_Sprite
#include "ICS_Helpers.h"	// for ICS_clamp

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
ICS_TextBox::ICS_TextBox(std::string fontFileName, int fontHeight, int lineSpacing, int width, int height, bool fillDown)
	:
	ICS_TextRenderable(fontFileName, fontHeight, width, height),
	_fontSize(fontHeight),
	_lineSpacing(lineSpacing),
	_scroll(0),
	_minScroll(0),
	_maxScroll(0),
	_fillDown(fillDown),
	_wrapMethod(ICS_WRAP),
	_newLine(false),
	_regenerateLines(false),
	_calculateScrollLimits(false)
{
	// use the stencil test when rendering
	enableWindowMode();
}

/**
 * Clears all text from the text box
 */
void
ICS_TextBox::clear()
{
	// remove all text
	_rawText.clear();
	_lines.clear();

	// regenerate lines and scroll values
	_regenerateLines = true;
	_calculateScrollLimits = true;
}

/**
 * This handles mouse wheel events that occur when the cursor is over the renderable.
 *
 * @param mouseX			The x position of the mouse cursor.
 * @param mouseY			The y position of the mouse cursor.
 * @param wheelRotation		The wheel rotation value.  A positive value indicates that the wheel was rotated forward, away
 *							from the user; a negative value indicates that the wheel was rotated backward, toward the user.
 */
void
ICS_TextBox::handleMouseWheelOver(float mouseX, float mouseY, int wheelRotation)
{
	// invoke the parent function
	ICS_TextRenderable::handleMouseWheelOver(mouseX, mouseY, wheelRotation);

	// update the scroll
	_scroll += wheelRotation / 12.0f;

	// limit the scroll value between the min and mix
	ICS_clamp(_scroll, _minScroll, _maxScroll);
}

/**
 * Sets the dimensions of the text box.
 * This function is a catch-all for dimension changes.
 * If the text box's dimensions change, this function will be called.
 *
 * @param dimensions	The dimensions to assign to the text box.
 */
void
ICS_TextBox::_setDimensions(const ICS_Pair<float>& dimensions)
{
	// if the width changed, regenerate the lines
	if (dimensions[ICS_WIDTH] != getWidth())
	{
		_regenerateLines = true;
	}

	// if the height changed, recalculate scroll limits
	if (dimensions[ICS_HEIGHT] != getHeight())
	{
		_calculateScrollLimits = true;
	}

	// invoke the parent function
	ICS_TextRenderable::_setDimensions(dimensions);
}

/**
 * Adds text to the lines which have already been generated
 *
 * @param data		The text to add to the lines.
 */
void
ICS_TextBox::addToLines(const ICS_ColoredText& newText)
{
	// make sure the font is initialized
	if (_font->isInitialized())
	{
		// create a temporary line to add to the lines
		std::vector<ICS_ColoredText> line;
		int lineLength = 0;

		// grab the last line of text from the lines
		if (_lines.size() > 0)
		{
			// get the line
			line = _lines[_lines.size() - 1];

			// determine the length
			for (unsigned int i = 0; i < line.size(); i++)
			{
				lineLength += _font->getTextWidth(line[i].text);
			}

			// pop it off the set (it may be modified and it will be re-added)
			_lines.pop_back();
		}

		// create text to add to the line
		ICS_ColoredText textBoxText = ICS_ColoredText("", newText.color);

		// go through each character
		for (unsigned int j = 0; j < newText.text.size(); j++)
		{
			// get the character and its length
			char character = newText.text[j];
			int characterLength = _font->getCharacterWidth(character);

			// if it is a new line character...
			if (character == '\n')
			{
				// blank line?
				if (_newLine)
				{
					// add the text and add the line
					if (textBoxText.text.length() > 0)
					{
						line.push_back(textBoxText);
					}

					_lines.push_back(line);

					// reset attributes
					line.clear();
					lineLength = 0;

					textBoxText.text = "";
				}

				// add the new line later if needed
				_newLine = true;
			}

			// the character is not a new line
			else
			{
				// new line? insert before new text is added
				if (_newLine)
				{
					// add the text and add the line
					if (textBoxText.text.length() > 0)
					{
						line.push_back(textBoxText);
					}

					_lines.push_back(line);

					// reset attributes
					_newLine = false;

					line.clear();
					lineLength = 0;

					textBoxText.text = "";
				}

				// if the character is not a new line and it fits...
				if (lineLength == 0 || lineLength + characterLength <= getWidth() || _wrapMethod != ICS_WRAP)
				{
					// add the character if it should not be truncated
					if (_wrapMethod == ICS_AUTO_FIT || lineLength < getWidth())
					{
						textBoxText.text += character;
						lineLength += characterLength;
					}

					// check for auto-fitting the text box to the contents
					if (_wrapMethod == ICS_AUTO_FIT and lineLength > getWidth())
					{
						setWidth(lineLength);
					}
				}

				// if the character is not a new line and it does not fit...
				else
				{
					// complete the line
					if (textBoxText.text.length() > 0)
					{
						line.push_back(textBoxText);
					}

					// reset the text to add to the line
					textBoxText.text = "";

					// add the character
					textBoxText.text += character;
					lineLength += characterLength;

					// find the last space character
					unsigned int textIndex = 0;
					unsigned int charIndex = 0;
					bool foundSpace = false;

					// go through each piece of text in the line backwards
					for (int x = line.size() - 1; x >= 0 and !foundSpace; x--)
					{
						// go through each character in the pice of text backwards
						for (int y = line[x].text.size(); y >= 0 and !foundSpace; y--)
						{
							// if it is a space, keep track of the location
							if (line[x].text[y] == ' ')
							{
								textIndex = x;
								charIndex = y;
								foundSpace = true;
							}
						}
					}

					// if a space was found, split the line into two
					if (foundSpace)
					{
						// create a temporary line to copy into
						std::vector<ICS_ColoredText> tempLine;
						int tempLineLength = 0;

						// go through each piece of text in the line
						for (unsigned int x = 0; x < line.size(); x++)
						{
							// if this is the pice of text containing the last space, split it
							if (x == textIndex)
							{
								// add the text and add the line
								tempLine.push_back(line[x].substr(0, charIndex));
								_lines.push_back(tempLine);

								// reset attributes
								tempLine.clear();
								tempLineLength = 0;

								// if there is remaining text, add it to the next line
								if (line[x].text.size() > (charIndex + 1))
								{
									tempLine.push_back(line[x].substr(charIndex + 1, line[x].text.size() - (charIndex + 1)));
									tempLineLength += _font->getTextWidth(tempLine[tempLine.size() - 1].text);
								}
							}

							// add the piece of text to the line
							else
							{
								tempLine.push_back(line[x]);
								tempLineLength += _font->getTextWidth(tempLine[tempLine.size() - 1].text);
							}
						}

						// continue adding to the temporary line
						line = tempLine;
						lineLength = tempLineLength;
					}

					// if no space was found, wrap the text to the next line
					else
					{
						// add the line
						_lines.push_back(line);

						// reset attributes
						line.clear();
						lineLength = 0;

						textBoxText.text = "";

						// add the character
						textBoxText.text += character;
						lineLength += characterLength;
					}
				}
			}
		}

		// if there is text remaining after each character, add it to the line
		if (textBoxText.text.length() > 0)
		{
			line.push_back(textBoxText);
		}

		// add the line that was popped off back in
		_lines.push_back(line);

		// recalculate the scroll limits
		_calculateScrollLimits = true;
	}

	// generate the lines before rendering
	else
	{
		_regenerateLines = true;
	}
}

/**
 * Separates the raw text into lines for rendering.
 */
void
ICS_TextBox::regenerateLines()
{
	// make sure the font is initialized
	if (_font->isInitialized())
	{
		// clear all the lines... these will be regenerated
		_lines.clear();

		// create a temporary line to add to the lines
		std::vector<ICS_ColoredText> line;
		int lineLength = 0;

		// go through each of the pieces of text
		for (unsigned int i = 0; i < _rawText.size(); i++)
		{
			addToLines(_rawText[i]);
		}

		// all done, don't do it again until the width changes
		_regenerateLines = false;
	}
}

/**
 * Updates the min and max scroll values based on the number of lines of text in the text box.
 */
void
ICS_TextBox::calculateScrollLimits()
{
	// fill the box from the top down
	if (_fillDown)
	{
		_minScroll = ICS_min(0.0f, getHeight() - _lines.size() * (_fontSize + _lineSpacing));
		_maxScroll = _minScroll > 0 ? _minScroll : 0;
	}

	// fill the box from the bottom up
	else
	{
		_minScroll = getHeight() - (int)_lines.size() * (_fontSize + _lineSpacing);
		_maxScroll = _minScroll > 0 ? _minScroll : 0;
	}

	// move the window to the bottom to show the most recent text
	_scroll = _minScroll;

	// limts have been calculated
	_calculateScrollLimits = false;
}

/**
 * Renders the text box.
 */
void
ICS_TextBox::render()
{
	// make sure the font is initialized
	if (_font->isInitialized())
	{
		// if the width of the box changed, regenerate the lines
		if (_regenerateLines)
		{
			regenerateLines();
		}

		// if the height of the box or the number of lines changed, calculate the scroll limits
		if (_calculateScrollLimits)
		{
			calculateScrollLimits();
		}

		// render each line
		for (unsigned int i = 0; i < _lines.size(); i++)
		{
			// keep track of the x position for the text
			int xPosition = 0;

			// render each piece of text
			for (unsigned int j = 0; j < _lines[i].size(); j++)
			{
				// calculate the y position of the line of text
				float yPosition = i * (_fontSize + _lineSpacing) + _scroll;

				// make sure the text would actually be rendered (not clipped)
				if (yPosition + _fontSize > 0 and yPosition < getHeight())
				{
					// set the drawing color
					_lines[i][j].color.setRenderColor();

					// render the text
					xPosition += _font->render(xPosition, yPosition, _lines[i][j].text);
				}
			}
		}
	}
}
