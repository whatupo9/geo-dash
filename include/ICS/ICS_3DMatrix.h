/*

ICS_3DMatrix.h

	Created: 2020-06-04

	Change log:

		2020-06-04
			- created ICS_3DMatrix to represent a 3D transformation matrix

*/

#pragma once

#include "ICS_Constants.h"	// for ICS_PI

#include <math.h>			// for cos and sin
#include <string.h>			// for memcpy

/**
 * A 3D transformation matrix.
 */
template <typename T>
class ICS_3DMatrix
{

// member attributes

private:

	T _elements[12];

// constructor

public:

	/**
	 * The ICS_2DMatrix constructor.
	 */
	ICS_3DMatrix()
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

		_elements[4] = 0;
		_elements[5] = 1;
		_elements[6] = 0;
		_elements[7] = 0;

		_elements[8] = 0;
		_elements[9] = 0;
		_elements[10] = 1;
		_elements[11] = 0;
	}

	/**
	 * Applies scaling to the matrix.
	 *
	 * @param scaleX	The scaling factor in the x direction.	
	 * @param scaleY	The scaling factor in the y direction.	
	 * @param scaleZ	The scaling factor in the z direction.	
	 */
	void scale(T scaleX, T scaleY, T scaleZ)
	{
		_elements[0] = _elements[0] * scaleX;
		_elements[4] = _elements[4] * scaleX;
		_elements[8] = _elements[8] * scaleX;

		_elements[1] = _elements[1] * scaleY;
		_elements[5] = _elements[5] * scaleY;
		_elements[9] = _elements[9] * scaleY;

		_elements[2] = _elements[2] * scaleZ;
		_elements[6] = _elements[6] * scaleZ;
		_elements[10] = _elements[10] * scaleZ;
	}

	/**
	 * Applies a translation to the matrix.
	 *
	 * @param translationX		The translation in the x direction, measured in pixels.
	 * @param translationY		The translation in the y direction, measured in pixels.
	 */
	void translate(T translationX, T translationY)
	{
		_elements[3] = _elements[0] * translationX + _elements[1] * translationX + _elements[2] * translationX + _elements[3];
		_elements[7] = _elements[4] * translationX + _elements[5] * translationX + _elements[6] * translationX + _elements[7];
		_elements[11] = _elements[8] * translationX + _elements[9] * translationX + _elements[10] * translationX + _elements[11];
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