/*

ICS_2DMatrix.h

	Created: 2020-04-15

	Change log:

		2020-04-15
			- created ICS_2DMatrix to represent a 2D transformation matrix

*/

#pragma once

#include "ICS_Constants.h"	// for ICS_PI

#include <math.h>			// for cos and sin
#include <string.h>			// for memcpy

/**
 * A 2D transformation matrix.
 */
template <typename T>
class ICS_2DMatrix
{

// member attributes

private:

	T _elements[6];

// constructor

public:

	/**
	 * The ICS_2DMatrix constructor.
	 */
	ICS_2DMatrix()
	{
		// load the identity matrix
		identity();
	}

// mutators

	/**
	 * Loads the identity matrix (no transformations) into the matrix.
	 */
	void identity()
	{
		_elements[0] = 1;
		_elements[1] = 0;
		_elements[2] = 0;

		_elements[3] = 0;
		_elements[4] = 1;
		_elements[5] = 0;
	}

	/**
	 * Applies scaling to the matrix.
	 *
	 * @param scaleX	The scaling factor in the x direction.	
	 * @param scaleY	The scaling factor in the y direction.	
	 */
	void scale(T scaleX, T scaleY)
	{
		_elements[0] = _elements[0] * scaleX;
		_elements[3] = _elements[3] * scaleX;

		_elements[1] = _elements[1] * scaleY;
		_elements[4] = _elements[4] * scaleY;
	}

	/**
	 * Applies a translation to the matrix.
	 *
	 * @param translationX		The translation in the x direction, measured in pixels.
	 * @param translationY		The translation in the y direction, measured in pixels.
	 */
	void translate(T translationX, T translationY)
	{
		_elements[2] = _elements[0] * translationX + _elements[1] * translationY + _elements[2];
		_elements[5] = _elements[3] * translationX + _elements[4] * translationY + _elements[5];
	}

	/**
	 * Applies a rotation to the matrix.
	 *
	 * @param angle		The counter-clockwise rotation to apply, measured in degrees.
	 */
	void rotate(T angle)
	{
		// copy the array elements (to prevent skewing)
		float temp[6];

		memcpy(temp, _elements, 6 * sizeof(float));

		// calculate cos and sin of the angle
		float cosAngle = (float)cos(ICS_PI * angle / 180.0f);
		float sinAngle = (float)sin(ICS_PI * angle / 180.0f);

		// apply the transformation
		_elements[0] = temp[0] * cosAngle + temp[1] * sinAngle;
		_elements[3] = temp[3] * cosAngle + temp[4] * sinAngle;
		_elements[1] = -temp[0] * sinAngle + temp[1] * cosAngle;
		_elements[4] = -temp[3] * sinAngle + temp[4] * cosAngle;
	}

	/**
	 * Transforms the input corrdatinates using the current transformation.
	 *
	 * @param x		The x component of the coordinates.
	 * @param y		The y component of the coordinates.
	 */
	void transform(T& x, T& y) const
	{
		// transform the coordinates
		float transformedX = _elements[0] * x + _elements[1] * y + _elements[2];
		float transformedY = _elements[3] * x + _elements[4] * y + _elements[5];

		// assign the values
		x = transformedX;
		y = transformedY;
	}

};