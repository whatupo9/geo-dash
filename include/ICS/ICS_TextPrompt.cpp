#include "ICS_Game.h"		// the definition of ICS_Game
#include "ICS_TextPrompt.h"	// the definition of ICS_TextPrompt

#include <algorithm>		// for erase and remove

/**
 * The ICS_TextPrompt constructor.
 *
 * @param fontFileName	The name of the file containing the font.
 * @param fontHeight	The height of the font in pixels.
 * @param width			The width of the text field in pixels.
 * @param height		The height of the text field in pixels.
 */
ICS_TextPrompt::ICS_TextPrompt(std::string fontFileName, int fontHeight, int width, int height)
	:
	ICS_TextField(fontFileName, fontHeight, width, height),
	_historyIndex(0)
{
	// prompts are always "active"
	_active = true;

	// start receiving events
	ICS_Game::getInstance().addUpdateEventListener(this);
	ICS_Game::getInstance().addKeyboardEventListener(this);
}

/**
 * This handles a single key press on the keyboard.
 *
 * @param key	The id of the key that was pressed.  Key constants can be found in ICS_Constants.h
 */
void
ICS_TextPrompt::onKeyPress(int key)
{
	// make sure the text field is active and enabled
	if (_active and _enabled)
	{
		// when enter is pressed, move the prompt to the entered text and clear the prompt
		if (key == ICS_KEY_ENTER and _value.length() > 0)
		{
			// update the history
			updateHistory(_value);							// add the input to the history
			_historyIndex = (unsigned int)_history.size();	// move the index to the end

			// trigger an event
			if (_promptCallback)
			{
				_promptCallback(_value);
			}

			// clear the text
			_value = "";
		}

		// when up is pressed, navigate up through the history
		else if (key == ICS_KEY_UP and _historyIndex > 0)
		{
			_historyIndex--;					// move up in the history
			_value = _history[_historyIndex];	// set the input text
		}

		// when down is pressed, navigate down through the history
		else if (key == ICS_KEY_DOWN and _historyIndex < _history.size())
		{
			// move down in the history
			_historyIndex++;

			// past the end of the history is nothing
			if (_historyIndex == _history.size())
			{
				_value = "";
			}

			// set the input text to the historical text
			else
			{
				_value = _history[_historyIndex];
			}
		}

		// let ICS_TextField handle the rest
		else
		{
			ICS_TextField::onKeyPress(key);
		}
	}
}

/**
 * This updates the history of the text field (adds new text to the bottom of the history)
 *
 * @param input		The last text that was entered in the text field.
 */
void
ICS_TextPrompt::updateHistory(std::string input)
{
	// remove the text from the history
	_history.erase(std::remove(_history.begin(), _history.end(), input), _history.end());
	
	// add the text to the end of the history
	_history.push_back(input);
}
