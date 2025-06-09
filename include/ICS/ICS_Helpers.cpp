#include "ICS_Helpers.h"	// the declarations of the helper functions

#include <sstream>			// for stringstream (used for tokenizing)
#include <iomanip>			// for converting float to string with precision

using namespace std;		// for string and vector

/**
 * Converts all upper case letters in a string to lower case.
 *
 * @param text		The text to convert.
 *
 * @returns			A string equivalent to the input string with all upper case letters converted to lower case.
 */
string ICS_toLowerCase(string text)
{
	// go through the input string indexes
	for (unsigned int i = 0; i < text.length(); i++)
	{
		// convert lower case letters to upper case
		if (text[i] >= 'A' && text[i] <= 'Z')
		{
			text[i] += ('a' - 'A');
		}
	}

	// return the result
	return text;
}

/**
 * Converts all lower case letters in a string to upper case.
 *
 * @param text		The text to convert.
 *
 * @returns			A string equivalent to the input string with all lower case letters converted to upper case.
 */
string ICS_toUpperCase(string text)
{
	// go through the input string indexes
	for (unsigned int i = 0; i < text.length(); i++)
	{
		// convert lower case letters to upper case
		if (text[i] >= 'a' && text[i] <= 'z')
		{
			text[i] -= ('a' - 'A');
		}
	}

	// return the result
	return text;
}

/**
 * This converts the given integer to a string recursively.
 *
 * @param value		The integer to convert to a string.
 *
 * @returns			The string representation of the int value.
 */
string ICS_intToString(int value)
{
	// write the data to a string stream
	stringstream ss;
	ss << value;

	// return the result
	return ss.str();
}

/**
 * This converts the given float to string.
 *
 * @param value				The float to convert to string.
 * @param decimalDigits		The number of deciaml digits to use.
 *
 * @returns					The string representation of the float value.
 */
string ICS_floatToString(float value, int decimalDigits)
{
	// write the data to a string stream
	stringstream ss;
	ss << fixed << setprecision(decimalDigits) << value;

	// return the result
	return ss.str();
}

/**
 * This replaces all occurances of a substring within a string with alternate text.
 *
 * @param text				The text to alter.
 * @param textToFind		The substring to search for.
 * @param replacementText	The text to replace the substring with.
 *
 * @returns					The result after replacing all occurances of the substring with the replacement text.
 */
string ICS_findAndReplace(string text, string textToFind, string replacementText)
{
	// get the first occurrence
	size_t position = text.find(textToFind);

	// repeat till there are no more occurances
	while (position != string::npos)
	{
		// replace this occurance
		text.replace(position, textToFind.size(), replacementText);

		// get the next occurrence
		position = text.find(textToFind);
	}

	// return the result
	return text;
}

/**
 * Tokenizes a string 
 *
 * @param text			The array to resize.
 * @param delimiter		The current number of elements in the array.
 *
 * @returns				A vector of strings containing the tokens.
 */
vector<string> ICS_tokenize(string text, char delimiter)
{
	// declare variables to help with tokenizing
	vector<string> tokens;	// for the result
	stringstream ss;		// for tokenizing the input string
	string token = "";		// to hold each token

	// put the input text into the stream for tokenizing
	ss << text;

	// begin tokenizing
	while (getline(ss, token, delimiter))
	{
		tokens.push_back(token);
	}

	// return the result
	return tokens;
}