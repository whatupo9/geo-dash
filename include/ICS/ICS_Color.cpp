#include "ICS_Color.h"	// the definition of ICS_Color

#include <winsock2.h>	// FFS WTF

#include <glut.h>		// the library for glut (OpenGL)

/**
 * ICS_Color constructor.
 *
 * @param r		The red component of the color
 * @param g		The green component of the color.
 * @param b		The blue component of the color.
 * @param a		The alpha component of the color.
 */
ICS_Color::ICS_Color(int r, int g, int b, int a)
	:
	red(r),
	green(g),
	blue(b),
	alpha(a)
{
	// restrict component values between the min and max
	ICS_clamp(red, ICS_COLOR_MIN, ICS_COLOR_MAX);
	ICS_clamp(green, ICS_COLOR_MIN, ICS_COLOR_MAX);
	ICS_clamp(blue, ICS_COLOR_MIN, ICS_COLOR_MAX);
	ICS_clamp(alpha, ICS_COLOR_MIN, ICS_COLOR_MAX);
}

/**
 * Sets the OpenGL rendering color to this color.
 */
void
ICS_Color::setRenderColor() const
{
	glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

/**
 * Sets the clear color to this color.
 */
void
ICS_Color::setClearColor()
{
	glClearColor(red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f);
}

/**
 * Multiplication operator.  Treats each component as a percentage (of the max), multiplies the corresponding
 * components from each color, and generates a new color.
 *
 * @param color1	The first color to multiply.
 * @param color2	The second color to multiply.
 *
 * @returns			The new color.
 */
ICS_Color operator*(const ICS_Color& color1, const ICS_Color& color2)
{
	return ICS_Color((color1.red * color2.red) / ICS_COLOR_MAX, (color1.green * color2.green) / ICS_COLOR_MAX, (color1.blue * color2.blue) / ICS_COLOR_MAX, (color1.alpha * color2.alpha) / ICS_COLOR_MAX);
}

/**
 * Scaler multiplication operator.  Multiplies each color component (except alpha) by the scaler value and generates a new color.
 *
 * @param color			The color to multiply.
 * @param scalerValue	The scaler value to multiply by.
 *
 * @returns				The new color.
 */
ICS_Color operator*(const ICS_Color& color, float scalerValue)
{
	return ICS_Color(color.red * scalerValue, color.green * scalerValue, color.blue * scalerValue, color.alpha);
}

/**
 * Scaler multiplication operators.  Multiplies each color component (except alpha) by the scaler value and generates a new color.
 *
 * @param scalerValue	The scaler value to multiply by.
 * @param color			The color to multiply.
 *
 * @returns				The new color.
 */
ICS_Color operator*(float scalerValue, const ICS_Color& color)
{
	return ICS_Color(color.red * scalerValue, color.green * scalerValue, color.blue * scalerValue, color.alpha);
}