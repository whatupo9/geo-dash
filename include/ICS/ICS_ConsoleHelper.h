/*

ICS_ConsoleHelper

	Created: ???

	Change log:

		2023-06-25
			- switch text color to strings (ANSI escape sequences) instead of integers
			- now works with Linux, in addition to Windows
			- added function to set text color to any RGB value
			- added functions and constants to set background color

		2023-12-07
			- fixed a bug that occured when ICS_backgroundColor was called before ICS_CLEAR_SCREEN was assigned a value

		2023-12-23
			- added option to create background colors with or without clearing the console window

*/

#pragma once

#include <iostream>	// for cout
#include <string>	// for string
#include <thread>	// for milliseconds and sleepfor

// ANSI code for clearing the screen
// for example: cout << ICS_CLEAR_SCREEN;
const std::string ICS_CLEAR_SCREEN =				"\033[H\033[2J\033[3J";

// ANSI codes for setting text color
// for example: cout << ICS_BLUE_TEXT << "Blue text!" << endl;
const std::string ICS_BLACK_TEXT =					"\033[30m";
const std::string ICS_RED_TEXT =					"\033[31m";
const std::string ICS_GREEN_TEXT =					"\033[32m";
const std::string ICS_YELLOW_TEXT =					"\033[33m";
const std::string ICS_BLUE_TEXT =					"\033[34m";
const std::string ICS_MAGENTA_TEXT =				"\033[35m";
const std::string ICS_CYAN_TEXT =					"\033[36m";
const std::string ICS_WHITE_TEXT =					"\033[37m";
const std::string ICS_INTENSE_BLACK_TEXT =			"\033[90m";
const std::string ICS_INTENSE_RED_TEXT =			"\033[91m";
const std::string ICS_INTENSE_GREEN_TEXT =			"\033[92m";
const std::string ICS_INTENSE_YELLOW_TEXT =			"\033[93m";
const std::string ICS_INTENSE_BLUE_TEXT =			"\033[94m";
const std::string ICS_INTENSE_MAGENTA_TEXT =		"\033[95m";
const std::string ICS_INTENSE_CYAN_TEXT =			"\033[96m";
const std::string ICS_INTENSE_WHITE_TEXT =			"\033[97m";

// ANSI codes for setting the console window's color
// for example: cout << ICS_CYAN_BACKGROUND;
const std::string ICS_BLACK_BACKGROUND =			"\033[40m\033[H\033[2J\033[3J";
const std::string ICS_RED_BACKGROUND =				"\033[41m\033[H\033[2J\033[3J";
const std::string ICS_GREEN_BACKGROUND =			"\033[42m\033[H\033[2J\033[3J";
const std::string ICS_YELLOW_BACKGROUND =			"\033[43m\033[H\033[2J\033[3J";
const std::string ICS_BLUE_BACKGROUND =				"\033[44m\033[H\033[2J\033[3J";
const std::string ICS_MAGENTA_BACKGROUND =			"\033[45m\033[H\033[2J\033[3J";
const std::string ICS_CYAN_BACKGROUND =				"\033[46m\033[H\033[2J\033[3J";
const std::string ICS_WHITE_BACKGROUND =			"\033[47m\033[H\033[2J\033[3J";
const std::string ICS_INTENSE_BLACK_BACKGROUND =	"\033[100m\033[H\033[2J\033[3J";
const std::string ICS_INTENSE_RED_BACKGROUND =		"\033[101m\033[H\033[2J\033[3J";
const std::string ICS_INTENSE_GREEN_BACKGROUND =	"\033[102m\033[H\033[2J\033[3J";
const std::string ICS_INTENSE_YELLOW_BACKGROUND =	"\033[103m\033[H\033[2J\033[3J";
const std::string ICS_INTENSE_BLUE_BACKGROUND =		"\033[104m\033[H\033[2J\033[3J";
const std::string ICS_INTENSE_MAGENTA_BACKGROUND =	"\033[105m\033[H\033[2J\033[3J";
const std::string ICS_INTENSE_CYAN_BACKGROUND =		"\033[106m\033[H\033[2J\033[3J";
const std::string ICS_INTENSE_WHITE_BACKGROUND =	"\033[107m\033[H\033[2J\033[3J";

/**
 * Waits for the user to press a key before continuing with the program, similar
 * to system("cls") in Windows, but this function should be platform independent
 */
void ICS_pause();

/**
 * This causes the program to sleep for period of time.
 *
 * @param milliseconds	The number of milliseconds to sleep for.
 */
void ICS_sleep(int milliseconds);

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
std::string ICS_textColor(int red, int green, int blue);

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
std::string ICS_backgroundColor(int red, int green, int blue, bool clearScreen = true);