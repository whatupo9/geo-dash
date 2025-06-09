/*

ICS_Pair.h

	Created: 2020-03-22

		2020-03-22
			- created ICS_Pair to represent a pair of values (like a position)

		2020-04-04
			- added addition and subtraction operators

		2020-04-08
			- added division and compound subtraction / assignment operators

*/

#pragma once

#include "ICS_Constants.h"	// for ICS_X and ICS_Y

/**
 * Represents a pair of values (like a 2-dimensional position)
 */
template <typename T>
class ICS_Pair
{

private:

// member attributes

	T _values[2];	// the pair of values

public:

	/**
	 * ICS_Pair default constructor.
	 */
	ICS_Pair()
	{
	}

	/**
	 * ICS_Pair constructor.
	 */
	ICS_Pair(T value1, T value2)
	{
		_values[0] = value1;
		_values[1] = value2;
	}

	/**
	 * Array index operator.
	 * This is used to set one of the pair values.
	 *
	 * @param index		The index of the value (0 or 1)
	 *
	 * @returns			A reference to the value indicated by the index.
	 */
	T& operator[](int index)
	{
		return _values[index];
	}

	/**
	 * Array index operator.
	 * This is used to get one of the pair values.
	 *
	 * @param index		The index of the value (0 or 1)
	 *
	 * @returns			A reference to the value indicated by the index.
	 */
	T operator[](int index) const
	{
		return _values[index];
	}

	/**
	 * Compound subtraction / assignment operator.
	 * Subtracts each component of the input pair from the corresponding component of this pair.
	 *
	 * @param pair		The pair to subtract from this pair.
	 */
	template <typename S>
	void operator-=(const ICS_Pair<S>& pair)
	{
		_values[0] -= pair[0];
		_values[1] -= pair[1];
	}

};

/**
 * Addition operator for ICS_Pair.  Creates a new pair by added each component of the input pairs.
 *
 * @param pair1		The first pair to add.
 * @param pair2		The second pair to add.
 *
 * @returns			The result of pair1 + pair2.
 */
template <typename T>
ICS_Pair<T> operator+(const ICS_Pair<T> pair1, const ICS_Pair<T> pair2)
{
	ICS_Pair<T> result;

	result[ICS_X] = pair1[ICS_X] + pair2[ICS_X];
	result[ICS_Y] = pair1[ICS_Y] + pair2[ICS_Y];

	return result;
}

/**
 * Subtraction operator for ICS_Pair.  Creates a new pair by subtracting each component of the input pairs.
 *
 * @param pair1		The pair to start with.
 * @param pair2		The pair to subtract.
 *
 * @returns			The result of pair1 - pair2.
 */
template <typename T>
ICS_Pair<T> operator-(const ICS_Pair<T> pair1, const ICS_Pair<T> pair2)
{
	ICS_Pair<T> result;

	result[ICS_X] = pair1[ICS_X] - pair2[ICS_X];
	result[ICS_Y] = pair1[ICS_Y] - pair2[ICS_Y];

	return result;
}


/**
 * Division operator for ICS_Pair.  Creates a new pair by dividing each component.
 *
 * @param pair		The pair to divide.
 * @param factor	The factor to divide by.
 *
 * @returns			The result of pair / factor.
 */
template <typename T, typename S>
ICS_Pair<T> operator/(const ICS_Pair<T> pair, S factor)
{
	ICS_Pair<T> result;

	result[ICS_X] = pair[ICS_X] / factor;
	result[ICS_Y] = pair[ICS_Y] / factor;

	return result;
}