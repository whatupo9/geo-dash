/*

ICS_TextField

	Created: ???

	Change log:

		2020-03-22
			- flip y coordinates... top left of the screen is (0, 0)

		2020-04-11
			- use ICS_Font (FreeType font) instead of ICS_TextRenderer (raster font)

		2020-04-13
			- shift text over to show cursor when text is too long
			- added repeating keys

		2020-04-17
			- use stencil test instead of scissor test so ICS_TextField can be transformed

		2023-01-13
			- hide cursor when the text field is disabled

		2023-01-20
			- removed font initialization which is now done automatically by the game

		2024-01-24
			- text fields only accept input after they are clicked and stop accepting input when something else is clicked

		2024-01-25
			- ICS_TextField inherits from ICS_TextRenderable
			- callback now sends the key that was pressed instead of the text in the field
			- text is not deleted when enter is pressed
			- added functions to get the entered text and clear the text (so it can be used as a prompt for user input)
			- removed "prompt" functionality (like clearing when enter is pressed and tracking history) and moved it to ICS_TextPrompt subclass
			- text field notifies parent when a key is pressed

		2024-02-04
			- use stencil test on text but not background or other children
			- center area where text is rendered in the text field
			- added optional text label for the text field

		2024-02-07
			- added a function to assign text to the text field
			- added option to only accept digits as input
			- added option to limit the number of characters in the text field

		2024-02-13
			- added a function to assign an integer value to the text field

		2024-02-17
			- inherits from ICS_EventListener so it can receive update and keyboard events

		2024-06-07
			- added copy constructor and assignment operator

*/

#pragma once

#include "ICS_TextRenderable.h"	// the definition of ICS_Renderable (ICS_TextField inherits from ICS_TextRenderable)
#include "ICS_EventListener.h"	// the definition of ICS_EventListener (ICS_TextField inherits from ICS_EventListener)
#include "ICS_Helpers.h"		// includes the definition of ICS_intToString

class ICS_Text;					// for adding an optional label

/**
 * A text box with cursor for entering text.
 */
class ICS_TextField: public ICS_TextRenderable, public ICS_EventListener
{

protected:

	char _cursorCharacter;			// the character to use for the cursor
	float _cursorTimer;				// for animating the cursor
	int _shiftCount;				// for checking if the shift key is held
	std::string _value;				// the value that is currently entered in the text field

	int _repeatKey;					// the key that should be repeated
	float _repeatTimer;				// a timer used to repeat keys that are held down
	float _repeatDelay;				// the number of seconds a key is held before it should repeat
	float _repeatRate;				// the number of seconds between repeating a key that is held down
	bool _repeat;					// indicates a key has been held down long enough to repeat

	ICS_Text* _label;				// an optional label for the text field

	bool _active;					// indicates that the text field is in focus
	bool _enabled;					// indicates that the text field is accepting input

	bool _digitsOnly;				// indicates that the text field will only accept numbers
	int _characterLimit;			// the maximum characters that can be entered in the field

public:

// constructors / destructor

	/**
	 * The ICS_TextField constructor.
	 *
	 * @param fontFileName	The name of the file containing the font.
	 * @param fontHeight	The height of the font in pixels.
	 * @param width			The width of the text field in pixels.
	 * @param height		The height of the text field in pixels.
	 */
	ICS_TextField(std::string fontFileName, int fontHeight, int width, int height);

	/**
	 * The ICS_TextField copy constructor.
	 *
	 * @param textField		The text field to copy.
	 */
	ICS_TextField(const ICS_TextField& textField);

	/**
	 * The ICS_TextField destructor.
	 */
	~ICS_TextField();

// assignment operator

	/**
	 * The assignment operator.
	 *
	 * @param textField		The text field to copy.
	 * 
	 * @returns				A reference to this instance.
	 */
	ICS_TextField& operator=(const ICS_TextField& textField);

// modifiers

	/**
	 * Adds a text label centered vertically on the left side if the text field.
	 *
	 * @param text		The text to use for the label.
	 * @param color		The color that the text should appear in.
	 */
	void addLabel(const std::string& text, const ICS_Color& color);

	/**
	 * Allows the text field to accept input.
	 */
	void enable()
	{
		_enabled = true;
	}

	/**
	 * Prevents the text field from accepting input.
	 */
	void disable()
	{
		// disable
		_enabled = false;

		// turn off repeating keys
		_repeatKey = ICS_KEY_INVALID;
	}

	/**
	 * Clears the text field (deletes the text entered).
	 */
	void clear()
	{
		_value = "";
	}

// getters

	/**
	 * Gets the value that has been entered in the field.
	 *
	 * @returns		The value that has been entered in the field.
	 */
	std::string getValue()
	{
		return _value;
	}

// setters

	/**
	 * Assigns a value to the text field.
	 *
	 * @param value	The text that should appear in the text field.
	 */
	void setValue(std::string value)
	{
		_value = value;
	}

	/**
	 * Assigns a value to the text field.
	 *
	 * @param value	The text that should appear in the text field.
	 */
	void setValue(int value)
	{
		setValue(ICS_intToString(value));
	}

	/**
	 * Tells the text field whether or not to restrict input to digits.
	 *
	 * @param digitsOnly	If true, the text field will only accept numerical digits.
	 */
	void setDigitsOnly(bool digitsOnly)
	{
		_digitsOnly = digitsOnly;
	}

	/**
	 * Sets the maximum number of characters allowed in the text field.
	 *
	 * @param characterLimit	The max number of characters.  Should be positive or ICS_NO_CHARACTER_LIMIT for unlimited.
	 */
	void setCharacterLimit(int characterLimit)
	{
		_characterLimit = characterLimit;
	}

// inquiry

	/**
	 * Checks if the text field is enabled.
	 *
	 * @returns		true if the text field is enabled, false otherwise.
	 */
	bool isEnabled()
	{
		return _enabled;
	}

private:

// event handlers

	/**
	 * Handles update events.
	 *
	 * @param elapsed	The number of seconds that have elapsed since the last update.
	 */
	void handleUpdateEvent(float elapsed);

	/**
	 * This handles keyboard events.
	 *
	 * @param key			The id of the key that was pressed / released.  Key constants can be found in ICS_Constants.h
	 * @param eventType		The type of event (ICS_EVENT_PRESS or ICS_EVENT_RELEASE)
	 */
	void handleKeyboardEvent(int key, int eventType);

	/**
	 * This is called when a mouse button is pressed over the text field.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param mouseX	The x position of the mouse cursor.
	 * @param mouseY	The y position of the mouse cursor.
	 */
	void handleMousePressOver(int button, float mouseX, float mouseY);

	/**
	 * This is called when a mouse button is pressed somewhere NOT over the renderable.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 */
	void handleMousePressNotOver(int button);

protected:

// helpers

	/**
	 * This handles a single key press on the keyboard.
	 *
	 * @param key	The id of the key that was pressed.  Key constants can be found in ICS_Constants.h
	 */
	virtual void onKeyPress(int key);

	/**
	 * Renders the text field.
	 */
	void render();

	/**
	 * Sets the dimensions of the text box.
	 * This function is a catch-all for dimension changes.
	 * If the text box's dimensions change, this function will be called.
	 *
	 * @param dimensions	The dimensions to assign to the text box.
	 */
	void _setDimensions(const ICS_Pair<float>& dimensions);

private:

	/**
	 * Frees memory and stops listening for keyboard events.
	 */
	void reset();

};