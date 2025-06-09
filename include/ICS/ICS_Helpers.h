/*

ICS_Helpers

	Created: ???

	Change log:

		2023-01-23
			- added function for calculating remainder

		2023-09-28
			- fixed intToString and floatToString functions to work for negative numbers
			- fixed floatToString to keep digits of precision even when 0

		2023-10-31
			- added ICS_ prefix to function names (refactoring)

		2023-12-08
			- removed unused functions (solveQuadratic, calculateRemainder, resize)

		2024-01-27
			- added error checking to ICS_clamp (ensure that min is less than max)

*/

#pragma once

#include <string>	// for using std::string
#include <vector>	// for using std::vector

/**
 * Converts all upper case letters in a string to lower case.
 *
 * @param text		The text to convert.
 *
 * @returns			A string equivalent to the input string with all upper case letters converted to lower case.
 */
std::string ICS_toLowerCase(std::string text);

/**
* Converts all lower case letters in a string to upper case.
*
* @param text		The text to convert.
*
* @returns			A string equivalent to the input string with all lower case letters converted to upper case.
*/
std::string ICS_toUpperCase(std::string text);

/**
 * This converts the given integer to string recursively.
 *
 * @param value		The integer to convert to string.
 *
 * @returns			The string representation of the int value.
 */
std::string ICS_intToString(int value);

/**
 * This converts the given float to string.
 *
 * @param value				The float to convert to string.
 * @param decimalDigits		The number of deciaml digits to use.
 *
 * @returns					The string representation of the float value.
 */
std::string ICS_floatToString(float value, int decimalDigits);

/**
 * This replaces all occurances of a substring within a string with alternate text.
 *
 * @param text				The text to alter.
 * @param textToFind		The substring to search for.
 * @param replacementText	The text to replace the substring with.
 *
 * @returns					The result after replacing all occurances of the substring with the replacement text.
 */
std::string ICS_findAndReplace(std::string text, std::string textToFind, std::string replacementText);

/**
 * Tokenizes a string 
 *
 * @param text			The array to resize.
 * @param delimiter		The current number of elements in the array.
 *
 * @returns				A vector of strings containing the tokens.
 */
std::vector<std::string> ICS_tokenize(std::string text, char delimiter);

/**
 * This determines the minimum of two values.
 *
 * @param value1	The first of two values to find the minimum of.
 * @param value2	The second of two values to find the minimum of.
 *
 * @returns			The minimum of the two input values.
 */
template<typename T>
T ICS_min(T value1, T value2)
{
	if (value1 < value2)
	{
		return value1;
	}

	return value2;
}

/**
 * This determines the maximum of two values.
 *
 * @param value1	The first of two values to find the minimum of.
 * @param value2	The second of two values to find the minimum of.
 *
 * @returns			The maximum of the two input values.
 */
template<typename T>
T ICS_max(T value1, T value2)
{
	if (value1 > value2)
	{
		return value1;
	}

	return value2;
}

/**
 * This determines the absolute value of a value.
 *
 * @param value		The value to determine the absolute value of
 *
 * @returns			The absolute value of the input value.
 */
template<typename T>
T ICS_absoluteValue(T value)
{
	if (value < 0)
	{
		return -value;
	}

	return value;
}

/**
 * This swaps the values of the input parameters.
 *
 * @param x		The first of the values to swap.  Will have the value of y after the function returns.
 * @param y		The second of the values to swap.  Will have the value of x after the function returns.
 */
template<typename T>
void ICS_swap(T& x, T& y)
{
	T temp = x;
	x = y;
	y = temp;
}

/*
 * If value is less than min, it is set to min.
 * If value is greater than max, it is set to max.
 *
 * @param value		The value to clamp.
 * @param min		The minimum that value can be.
 * @param max		The maximum that value can be.
*/
template <typename T>
void ICS_clamp(T& value, T min, T max)
{
	// make sure min and max are in the correct order
	if (max < min)
	{
		ICS_swap(min, max);
	}

	// make sure value is no less than min
	if (value < min)
	{
		value = min;
	}

	// make sure value is no more than max
	if (value > max)
	{
		value = max;
	}
}