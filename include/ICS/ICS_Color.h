/*

ICS_Color

	Created: ???

	Change log:

		2020-04-12
			- added a comparison operator

		2024-01-19
			- added a multiplication operator

		2024-01-21
			- added option to setRenderColor to use a greyscale version of the color

		2024-01-23
			- added a member function to generate a greyscale version of the color

		2024-01-27
			- added scaler multiplication functions

*/

#pragma once

#include "ICS_Constants.h"	// min and max color values
#include "ICS_Helpers.h"	// for ICS_clamp

/**
 * Represents a color with red, green, blue and alpha components.
 */
struct ICS_Color
{
	int red;	// the red component of the color.
	int green;	// the green component of the color.
	int blue;	// the blue component of the color.
	int alpha;	// the alpha component of the color.

	/**
	 * ICS_Color constructor.
	 *
	 * @param r		The red component of the color
	 * @param g		The green component of the color.
	 * @param b		The blue component of the color.
	 * @param a		The alpha component of the color.
	 */
	ICS_Color(int r = ICS_COLOR_MAX, int g = ICS_COLOR_MAX, int b = ICS_COLOR_MAX, int a = ICS_COLOR_MAX);

// helpers

	/**
	 * Sets the OpenGL rendering color to this color.
	 */
	void setRenderColor() const;

// setters

	/**
	 * Sets the clear color to this color.
	 */
	void setClearColor();

// operators

	/**
	 * Comparison operator.
	 *
	 * @param color		The color to compare with.
	 *
	 * @returns			true if the colors are the same, false otherwise.
	 */
	bool operator==(const ICS_Color& color) const
	{
		return (red == color.red && green == color.green && blue == color.blue && alpha == color.alpha);
	}
};

/**
 * Multiplication operator.  Treats each component as a percentage (of the max), multiplies the corresponding
 * components from each color, and generates a new color.
 *
 * @param color1	The first color to multiply.
 * @param color2	The second color to multiply.
 *
 * @returns			The new color.
 */
ICS_Color operator*(const ICS_Color& color1, const ICS_Color& color2);

/**
 * Scaler multiplication operators.  Multiplies each color component (except alpha) by the scaler value and generates a new color.
 *
 * @param color			The color to multiply.
 * @param scalerValue	The scaler value to multiply by.
 *
 * @returns				The new color.
 */
ICS_Color operator*(const ICS_Color& color, float scalerValue);

/**
 * Scaler multiplication operators.  Multiplies each color component (except alpha) by the scaler value and generates a new color.
 *
 * @param scalerValue	The scaler value to multiply by.
 * @param color			The color to multiply.
 *
 * @returns				The new color.
 */
ICS_Color operator*(float scalerValue, const ICS_Color& color);