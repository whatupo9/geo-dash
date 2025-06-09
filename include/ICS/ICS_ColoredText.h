/*

ICS_TextBox

	Created: 2020-04-12

	Change log:

		2020-04-12
			- created struct to use with ICS_TextBox

*/

#pragma once

#include "ICS_Color.h"	// the definition of ICS_Color (to represent the color of the text)

#include <string>		// the definition of std::string (to represent the text)

/**
 * Represents text that can be displayed in any color.
 */
struct ICS_ColoredText
{

// member attributes

	std::string text;	// the text
	ICS_Color color;	// the color to display the text

// constructors

	/**
	 * The ICS_ColoredText default constructor.
	 */
	ICS_ColoredText()
	{
	}

	/**
	 * The ICS_ColoredText constructor.
	 *
	 * @param t		The text.
	 * @param c		The color to display the text.
	 */
	ICS_ColoredText(std::string t, ICS_Color c)
		:
		text(t),
		color(c)
	{
	}

// helpers

	/**
	 * Returns colored text which is a substring of this.
	 *
	 * @param pos		The starting index to get the substring.
	 * @param len		The number of characters in the substring.
	 */
	ICS_ColoredText substr(size_t pos, size_t len)
	{
		return ICS_ColoredText(text.substr(pos, len), color);
	}

};
