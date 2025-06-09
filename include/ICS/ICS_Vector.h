/*

ICS_Vector.h

	Created: ???

	Change log:

		2020-03-22
			- added constructor which takes input

		2020-03-23
			- using constants for X, Y and Z indexes

		2020-03-26
			- added an addition operator

		2020-03-28
			- added a subtraction operator

		2020-05-22
			- addded function for calculating the dot product of two vectors
			- added scaler multiplication operator
			- added function to normalize vector

		2020-05-27
			- added length function

*/

#pragma once

#include "ICS_Constants.h"	// constants for x, y and z coordinates

/**
 * Represents a 3-dimensional position or direction.
 */
template <typename T>
class ICS_Vector
{

private:

// member attributes

	T _coordinates[3];	// the x, y and z components of the vector

public:

	/**
	 * ICS_Vector default constructor.
	 */
	ICS_Vector()
	{
	}

	/**
	 * ICS_Vector constructor.
	 */
	ICS_Vector(T x, T y, T z)
	{
		_coordinates[ICS_X] = x;
		_coordinates[ICS_Y] = y;
		_coordinates[ICS_Z] = z;
	}

	/**
	 * Array index operator.
	 * This is used to assign a value to a components of the vector.
	 *
	 * @param index		The index of the component.  x is index 0, y is index 1 and z is index 2.
	 *
	 * @returns			A reference to the component of the vector indicated by the index.
	 */
	T& operator[](int index)
	{
		return _coordinates[index];
	}

	/**
	 * Array index operator.
	 * This is used to get the value of a component of the vector.
	 *
	 * @param index		The index of the component.  x is index 0, y is index 1 and z is index 2.
	 *
	 * @returns			A reference to the component of the vector indicated by the index.
	 */
	T operator[](int index) const
	{
		return _coordinates[index];
	}

	/**
	 * Calculates the length of the vector.
	 *
	 * @returns		The length of the vector.
	 */
	T length()
	{
		return sqrt(_coordinates[ICS_X] * _coordinates[ICS_X] + _coordinates[ICS_Y] * _coordinates[ICS_Y] + _coordinates[ICS_Z] * _coordinates[ICS_Z]);
	}

	/**
	 * Converts the vector to a unit vector (length 1).
	 */
	void normalize()
	{
		// calculate the length
		T l = length();

		// divide each component by the length
		_coordinates[ICS_X] /= l;
		_coordinates[ICS_Y] /= l;
		_coordinates[ICS_Z] /= l;
	}
};

/**
 * Scaler multiplication operator for ICS_Vector.
 *
 * @param scaler	The scaler to multiply by.
 * @param vector	The vector to multiply.
 *
 * @returns			The result of scaler * vector.
 */
template <typename T>
ICS_Vector<T> operator*(T scaler, const ICS_Vector<T> vector)
{
	ICS_Vector<T> result;

	result[ICS_X] = scaler * vector[ICS_X];
	result[ICS_Y] = scaler * vector[ICS_Y];
	result[ICS_Z] = scaler * vector[ICS_Z];

	return result;
}

/**
 * Scaler multiplication operator for ICS_Vector.
 *
 * @param vector	The vector to multiply.
 * @param scaler	The scaler to multiply by.
 *
 * @returns			The result of vector * scaler.
 */
template <typename T>
ICS_Vector<T> operator*(const ICS_Vector<T> vector, T scaler)
{
	ICS_Vector<T> result;

	result[ICS_X] = scaler * vector[ICS_X];
	result[ICS_Y] = scaler * vector[ICS_Y];
	result[ICS_Z] = scaler * vector[ICS_Z];

	return result;
}

/**
 * Addition operator for ICS_Vector.  Creates a new vector by added each component of the input vectors.
 *
 * @param vector1	The first vector to add.
 * @param vector2	The second vector to add.
 *
 * @returns			The result of vector1 + vector2.
 */
template <typename T>
ICS_Vector<T> operator+(const ICS_Vector<T> vector1, const ICS_Vector<T> vector2)
{
	ICS_Vector<T> result;

	result[ICS_X] = vector1[ICS_X] + vector2[ICS_X];
	result[ICS_Y] = vector1[ICS_Y] + vector2[ICS_Y];
	result[ICS_Z] = vector1[ICS_Z] + vector2[ICS_Z];

	return result;
}

/**
 * Subtraction operator for ICS_Vector.  Creates a new vector by subtracting each component of the input vectors.
 *
 * @param vector1	The vector to start with.
 * @param vector2	The vector to subtract.
 *
 * @returns			The result of vector1 - vector2.
 */
template <typename T>
ICS_Vector<T> operator-(const ICS_Vector<T> vector1, const ICS_Vector<T> vector2)
{
	ICS_Vector<T> result;

	result[ICS_X] = vector1[ICS_X] - vector2[ICS_X];
	result[ICS_Y] = vector1[ICS_Y] - vector2[ICS_Y];
	result[ICS_Z] = vector1[ICS_Z] - vector2[ICS_Z];

	return result;
}

/**
 * Calculates the dot product for two ICS_Vectors.
 *
 * @param vector1	The first vector in the operation.
 * @param vector2	The second vector in the operation.
 *
 * @returns			The dot product of vector1 and vector2.
 */
template <typename T>
T dot(const ICS_Vector<T> vector1, const ICS_Vector<T> vector2)
{
	return vector1[ICS_X] * vector2[ICS_X] + vector1[ICS_Y] * vector2[ICS_Y] + vector1[ICS_Z] * vector2[ICS_Z];
}