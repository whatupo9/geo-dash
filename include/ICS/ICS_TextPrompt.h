/*

ICS_TextPrompt

	Created: 2024-01-25

	Change log:

		2024-01-25
			- made this a child of ICS_TextField and took "prompt" functionality out of ICS_TextField

*/

#pragma once

#include "ICS_TextField.h"	// the definition of ICS_TextField (ICS_TextPrompt inherits from ICS_TextField)

#include <vector>			// a vector container (a variable length array)

/**
 * A text box with cursor for entering text.
 */
class ICS_TextPrompt: public ICS_TextField
{

private:

	std::vector<std::string> _history;				// the history of entered text (use up and down to access)
	unsigned int _historyIndex;						// keeps track of the position in text history the user has navigated to

	ICS_TextPromptEventFunction _promptCallback;	// the callback for entering text in the text prompt

public:

// constructor

	/**
	 * The ICS_TextPrompt constructor.
	 *
	 * @param fontFileName	The name of the file containing the font.
	 * @param fontHeight	The height of the font in pixels.
	 * @param width			The width of the text field in pixels.
	 * @param height		The height of the text field in pixels.
	 */
	ICS_TextPrompt(std::string fontFileName, int fontHeight, int width, int height);

// callback management

	/**
	 * Sets the callback for the text field being modified.
	 *
	 * @param callback	The function to call when text is entered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(std::string text)
	 *
	 *					Where text is the text that was entered in the propmpt.
	 */
	void setCallback(ICS_TextPromptEventFunction callback)
	{
		_promptCallback = callback;
	}

// event handlers

	/**
	 * This is called when a mouse button is pressed somewhere NOT over the renderable.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 */
	void handleMousePressNotOver(int button)
	{
		// bypass ICS_TextField functionality (text prompts are always active)
		ICS_Renderable::handleMousePressNotOver(button);
	}

private:

// helpers

	/**
	 * This handles a single key press on the keyboard.
	 *
	 * @param key	The id of the key that was pressed.  Key constants can be found in ICS_Constants.h
	 */
	void onKeyPress(int key);

	/**
	 * This updates the history of the text field (adds new text to the bottom of the history)
	 *
	 * @param input		The last text that was entered in the text field.
	 */
	void updateHistory(std::string input);

};