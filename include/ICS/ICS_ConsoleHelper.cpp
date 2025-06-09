#include "ICS_ConsoleHelper.h"
#include "ICS_Helpers.h"

/**
 * Waits for the user to press a key before continuing with the program, similar
 * to system("cls") in Windows, but this function should be platform independent
 */
void ICS_pause()
{
#ifdef _WIN32
	// on Windows, this will be exectuted
	system("pause");
#else
	// all other operating systems will execute this
	std::cout << "Press any key to continue . . .";
	getchar();
#endif
}

/**
 * This causes the program to sleep for period of time.
 *
 * @param milliseconds	The number of milliseconds to sleep for.
 */
void ICS_sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

/*
 * This generates an ANSI code to change the text color in the console window using RGB values.
 *
 *	Example:
 *
 *		cout << ICS_textColor(255, 0, 255);
 *
 * @param red		The red component of the color.  Must be between 0 and 255.
 * @param green		The green component of the color.  Must be between 0 and 255.
 * @param blue		The blue component of the color.  Must be between 0 and 255.
 *
 * @returns		An ANSI code for setting the text color in the console window.
 */
std::string ICS_textColor(int red, int green, int blue)
{
	// make sure the components are in the correct range
	ICS_clamp(red, 0, 255);
	ICS_clamp(green, 0, 255);
	ICS_clamp(blue, 0, 255);

	// create the ANSI escape code
	std::string code = "\033[38;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";

	// set the foreground color
	return code;
}

/**
 * This generates an ANSI code to change the background color of the console window using RGB values.
 *
 *	Example:
 *
 *		cout << ICS_backgroundColor(255, 0, 255);
 *
 * @param red			The red component of the color.  Must be between 0 and 255.
 * @param green			The green component of the color.  Must be between 0 and 255.
 * @param blue			The blue component of the color.  Must be between 0 and 255.
 * @param clearScreen	Indicates ICS_CLEAR_SCREEN should be appended to the escape sequence.
 *
 * @returns				An ANSI code for setting the background color of the console window.
 */
std::string ICS_backgroundColor(int red, int green, int blue, bool clearScreen)
{
	// make sure the components are in the correct range
	ICS_clamp(red, 0, 255);
	ICS_clamp(green, 0, 255);
	ICS_clamp(blue, 0, 255);

	// create the ANSI escape code
	std::string code = "\033[48;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";

	// return the code to set the background color
	if (clearScreen)
	{
		return (code + "\033[H\033[2J\033[3J");
	}
	else
	{
		return code;
	}
}