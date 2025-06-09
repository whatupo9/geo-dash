/*

ICS_Constants

	Created: 2020-03-23

	Change log:

		2020-03-23
			- created ICS_Constants to store the various constants used in the engine
			- added constants for mouse buttons, color (min, max), position (x, y, z) and texture coordinates

		2020-03-25
			- added constants for key presses in keyboard events

		2020-03-26
			- added constants for dimensions (width and height)
			- added constants for directions (horizontal and vertical)

		2020-03-27
			- added constants for key / button presses and releases

		2020-04-01
			- added constants for text wrapping options (wrap, truncate, auto-fit)

		2020-04-08
			- added constants for anchor points for renderable objects

		2020-04-15
			- added pi

		2022-12-06
			- added constants for colors for the console window

		2023-01-25
			- added constants for horizontal and vertical anchor points

		2023-03-09
			- added constants for keys: shift, ctrl, caps lock, escape, delete and F1-F9
			- NOTE: the F10, F11, F12, print screen and alt keys do not seem to trigger keyboard events

		2023-10-18
			- invalid sound id was set to 0 which should be valid.
			- changed invalid sound id to -1

		2023-11-01
			- added constants for F10, F11 and F12 (only F11 is confirmed to work though... F10 and F12 don't seem to trigger events)

		2024-01-21
			- removed constants for texture coordinates as the were no longer used

		2024-01-24
			- added constants for button states (up, down, hover and disabled)

		2024-01-28
			- removed color constants which were no longer used
			- changed button events to generic events (press, release, click)
			- added events for mouse over, mouse off and zooming

		2024-02-01
			- added a constant to represent the number of mouse buttons

		2024-02-04
			- added an event for a window's content changing
			- added an event for a text field losing focus
			- removed key press and release and use the event press and release instead

		2024-02-07
			- added constant for minus key
			- added constant for character limit on text fields

		2024-05-05
			- removed anchor constants which are no longer needed

*/

#pragma once

const double ICS_PI = 3.1415926535897932384626433832795;	// many digits of the mathematical constant pi

const int ICS_KEY_INVALID = -1;		// value used for an invalid key

const int ICS_KEY_BACKSPACE = 8;	// value of the 'backspace' key in a keyboard event
const int ICS_KEY_TAB = 9;			// value of the 'tab' key in a keyboard event
const int ICS_KEY_ENTER = 13;		// value of the 'enter' key in a keyboard event
const int ICS_KEY_SHIFT = 16;		// value of the 'shift' key in a keyboard event
const int ICS_KEY_CTRL = 17;		// value of the 'ctrl' key in a keyboard event
const int ICS_KEY_CAPS_LOCK = 20;	// value of the 'caps lock' key in a keyboard event
const int ICS_KEY_ESC = 27;			// value of the 'esc' key in a keyboard event
const int ICS_KEY_SPACE = 32;		// value of the 'space' key in a keyboard event

const int ICS_KEY_LEFT = 37;		// value of the 'left' directional arrow in a keyboard event
const int ICS_KEY_UP = 38;			// value of the 'up' directional arrow in a keyboard event
const int ICS_KEY_RIGHT = 39;		// value of the 'right' directional arrow in a keyboard event
const int ICS_KEY_DOWN = 40;		// value of the 'down' directional arrow in a keyboard event

const int ICS_KEY_DELETE = 46;		// value of the 'delete' directional arrow in a keyboard event

const int ICS_KEY_0 = 48;			// value of the '0' key in a keyboard event
const int ICS_KEY_1 = 49;			// value of the '1' key in a keyboard event
const int ICS_KEY_2 = 50;			// value of the '2' key in a keyboard event
const int ICS_KEY_3 = 51;			// value of the '3' key in a keyboard event
const int ICS_KEY_4 = 52;			// value of the '4' key in a keyboard event
const int ICS_KEY_5 = 53;			// value of the '5' key in a keyboard event
const int ICS_KEY_6 = 54;			// value of the '6' key in a keyboard event
const int ICS_KEY_7 = 55;			// value of the '7' key in a keyboard event
const int ICS_KEY_8 = 56;			// value of the '8' key in a keyboard event
const int ICS_KEY_9 = 57;			// value of the '9' key in a keyboard event

const int ICS_KEY_A = 65;			// value of the 'a' key in a keyboard event
const int ICS_KEY_B = 66;			// value of the 'b' key in a keyboard event
const int ICS_KEY_C = 67;			// value of the 'c' key in a keyboard event
const int ICS_KEY_D = 68;			// value of the 'd' key in a keyboard event
const int ICS_KEY_E = 69;			// value of the 'e' key in a keyboard event
const int ICS_KEY_F = 70;			// value of the 'f' key in a keyboard event
const int ICS_KEY_G = 71;			// value of the 'g' key in a keyboard event
const int ICS_KEY_H = 72;			// value of the 'h' key in a keyboard event
const int ICS_KEY_I = 73;			// value of the 'i' key in a keyboard event
const int ICS_KEY_J = 74;			// value of the 'j' key in a keyboard event
const int ICS_KEY_K = 75;			// value of the 'k' key in a keyboard event
const int ICS_KEY_L = 76;			// value of the 'l' key in a keyboard event
const int ICS_KEY_M = 77;			// value of the 'm' key in a keyboard event
const int ICS_KEY_N = 78;			// value of the 'n' key in a keyboard event
const int ICS_KEY_O = 79;			// value of the 'o' key in a keyboard event
const int ICS_KEY_P = 80;			// value of the 'p' key in a keyboard event
const int ICS_KEY_Q = 81;			// value of the 'q' key in a keyboard event
const int ICS_KEY_R = 82;			// value of the 'r' key in a keyboard event
const int ICS_KEY_S = 83;			// value of the 's' key in a keyboard event
const int ICS_KEY_T = 84;			// value of the 't' key in a keyboard event
const int ICS_KEY_U = 85;			// value of the 'u' key in a keyboard event
const int ICS_KEY_V = 86;			// value of the 'v' key in a keyboard event
const int ICS_KEY_W = 87;			// value of the 'w' key in a keyboard event
const int ICS_KEY_X = 88;			// value of the 'x' key in a keyboard event
const int ICS_KEY_Y = 89;			// value of the 'y' key in a keyboard event
const int ICS_KEY_Z = 90;			// value of the 'z' key in a keyboard event

const int ICS_KEY_F1 = 112;			// value of the 'F1' key in a keyboard event
const int ICS_KEY_F2 = 113;			// value of the 'F2' key in a keyboard event
const int ICS_KEY_F3 = 114;			// value of the 'F3' key in a keyboard event
const int ICS_KEY_F4 = 115;			// value of the 'F4' key in a keyboard event
const int ICS_KEY_F5 = 116;			// value of the 'F5' key in a keyboard event
const int ICS_KEY_F6 = 117;			// value of the 'F6' key in a keyboard event
const int ICS_KEY_F7 = 118;			// value of the 'F7' key in a keyboard event
const int ICS_KEY_F8 = 119;			// value of the 'F8' key in a keyboard event
const int ICS_KEY_F9 = 120;			// value of the 'F9' key in a keyboard event
const int ICS_KEY_F10 = 121;		// value of the 'F10' key in a keyboard event
const int ICS_KEY_F11 = 122;		// value of the 'F11' key in a keyboard event
const int ICS_KEY_F12 = 123;		// value of the 'F12' key in a keyboard event

const int ICS_KEY_MINUS = 189;		// value of the '-' key in a keyboard event

// a look-up table mapping key event values to ASCII characters (no shift held)
const char ICS_KEY_CHAR_MAP[256] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, '\t', 0, 0, 0, 0, 0, 0,								// 0 - 15
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 16 - 31
	' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,								// 32 - 47
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 0, 0, 0, 0, 0, 0,				// 48 - 63
	0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',	// 64 - 79
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 0, 0, 0, 0, 0,			// 80 - 95
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 96 - 111
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 112 - 127
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 128 - 143
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 144 - 159
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 160 - 175
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ';', '=', ',', '-', '.', '/',						// 176 - 191
	'`', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,								// 192 - 207
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '[', '\\', ']', '\'', 0,						// 208 - 223
	0, 0, '\\', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,								// 224 - 239
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0									// 240 - 255
};

// a look-up table mapping key event values to ASCII characters (shift held)
const char ICS_SHIFT_KEY_CHAR_MAP[256] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 0 - 15
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 16 - 31
	' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,								// 32 - 47
	')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 0, 0, 0, 0, 0, 0,				// 48 - 63
	0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',	// 64 - 79
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0, 0, 0, 0, 0,			// 80 - 95
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 96 - 111
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 112 - 127
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 128 - 143
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 144 - 159
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,									// 160 - 175
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ':', '+', '<', '_', '>', '?',						// 176 - 191
	'~', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,								// 192 - 207
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '{', '|', '}', '"', 0,							// 208 - 223
	0, 0, '|', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,								// 224 - 239
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0									// 240 - 255
};

const int ICS_NO_CHARACTER_LIMIT = -1;			// represents no character limit for text fields

const int ICS_LEFT_MOUSE_BUTTON = 0;			// right mouse button
const int ICS_RIGHT_MOUSE_BUTTON = 1;			// left mouse button
const int ICS_MIDDLE_MOUSE_BUTTON = 2;			// middle mouse button
const int ICS_MOUSE_BUTTONS = 3;				// the number of mouse buttons

const int ICS_BUTTON_STATE_UP = 0;				// button state: button not being interacted with
const int ICS_BUTTON_STATE_DOWN = 1;			// button state: button being held down
const int ICS_BUTTON_STATE_HOVER = 2;			// button state: mouse cursor over the button
const int ICS_BUTTON_STATE_DISABLED = 3;		// button state: button disabled
const int ICS_BUTTON_STATES = 4;				// the number of button states

const int ICS_EVENT_PRESS = 0;					// event type: press
const int ICS_EVENT_RELEASE = 1;				// event type: release
const int ICS_EVENT_CLICK = 2;					// event type: click (press and release a game element)
const int ICS_EVENT_MOUSE_ENTER = 3;			// event type: mouse enter (cursor moves over a game element)
const int ICS_EVENT_MOUSE_LEAVE = 4;			// event type: mouse leave (cursor moves off of a game element)
const int ICS_EVENT_CHANGE = 5;					// event type: change (for text fields, sliders, window content, etc...)
const int ICS_EVENT_LOSE_FOCUS = 6;				// event type: lose focus (for text fields)

const int ICS_COLOR_MIN = 0;					// the minimum ICS_Color component value
const int ICS_COLOR_MAX = 255;					// the maximum ICS_Color component value

const int ICS_LEFT = 0;							// for text justification (left)
const int ICS_RIGHT = 1;						// for text justification (right)
const int ICS_CENTER = 2;						// for text justification (center)

const int ICS_X = 0;							// the index for X
const int ICS_Y = 1;							// the index for Y
const int ICS_Z = 2;							// the index for Z

const int ICS_WIDTH = 0;						// the index for the width component of an renderable's dimensions
const int ICS_HEIGHT = 1;						// the index for the height component of an renderable's dimensions

const int ICS_HORIZONTAL = 0;					// horizontal direction
const int ICS_VERTICAL = 1;						// vertical direction

const int ICS_WRAP = 0;							// for text rendering... wrap text to a new line
const int ICS_TRUNCATE = 1;						// for text rendering... cut text off at the end of a line
const int ICS_AUTO_FIT = 2;						// for text rendering... resize text box to fit long text 

const char ICS_NEW_LINE = '\n';					// for moving the cursor to a new line