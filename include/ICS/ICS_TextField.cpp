#include "ICS_Game.h"		// the definition of ICS_Game
#include "ICS_TextField.h"	// the definition of ICS_TextField
#include "ICS_Font.h"		// the definition of ICS_Font
#include "ICS_Text.h"		// the definition of ICS_Text (for the optional label)

/**
 * The ICS_TextField constructor.
 *
 * @param fontFileName	The name of the file containing the font.
 * @param fontHeight	The height of the font in pixels.
 * @param width			The width of the text field in pixels.
 * @param height		The height of the text field in pixels.
 */
ICS_TextField::ICS_TextField(std::string fontFileName, int fontHeight, int width, int height)
	:
	ICS_TextRenderable(fontFileName, fontHeight, width, height),
	_cursorCharacter('_'),
	_cursorTimer(0.0f),
	_shiftCount(0),
	_repeatKey(ICS_KEY_INVALID),
	_repeatTimer(0.0f),
	_repeatDelay(0.5f),
	_repeatRate(0.05f),
	_repeat(false),
	_label(NULL),
	_active(false),
	_enabled(true),
	_digitsOnly(false),
	_characterLimit(ICS_NO_CHARACTER_LIMIT)
{
}

/**
 * The ICS_TextField copy constructor.
 *
 * @param textField		The text field to copy.
 */
ICS_TextField::ICS_TextField(const ICS_TextField& textField)
	:
	ICS_TextRenderable(textField),
	ICS_EventListener(textField),
	_cursorCharacter(textField._cursorCharacter),
	_cursorTimer(textField._cursorTimer),
	_shiftCount(textField._shiftCount),
	_value(textField._value),
	_repeatKey(textField._repeatKey),
	_repeatTimer(textField._repeatTimer),
	_repeatDelay(textField._repeatDelay),
	_repeatRate(textField._repeatRate),
	_repeat(textField._repeat),
	_label(NULL),
	_active(textField._active),
	_enabled(textField._enabled),
	_digitsOnly(textField._digitsOnly),
	_characterLimit(textField._characterLimit)
{
	// copy the label
	if (textField._label)
	{
		_label = new ICS_Text(*textField._label);
		addChild(_label);
	}

	// start recieving events
	if (_active)
	{
		ICS_Game::getInstance().addUpdateEventListener(this);
		ICS_Game::getInstance().addKeyboardEventListener(this);
	}
}

/**
 * The ICS_TextField destructor.
 */
ICS_TextField::~ICS_TextField()
{
	// free memory and stop listening for keyboard events
	reset();
}

/**
 * The assignment operator.
 *
 * @param textField		The text field to copy.
 *
 * @returns				A reference to this instance.
 */
ICS_TextField&
ICS_TextField::operator=(const ICS_TextField& textField)
{
	// free memory and stop listening for keyboard events
	reset();

	// invoke the parent assignment operators
	ICS_TextRenderable::operator=(textField);
	ICS_EventListener::operator=(textField);

	// copy each attribute
	_cursorCharacter = textField._cursorCharacter;
	_cursorTimer = textField._cursorTimer;
	_shiftCount = textField._shiftCount;
	_value = textField._value;
	_repeatKey = textField._repeatKey;
	_repeatTimer = textField._repeatTimer;
	_repeatDelay = textField._repeatDelay;
	_repeatRate = textField._repeatRate;
	_repeat = textField._repeat;
	_label = NULL;
	_active = textField._active;
	_enabled = textField._enabled;
	_digitsOnly = textField._digitsOnly;
	_characterLimit = textField._characterLimit;
	
	// copy the label
	if (textField._label)
	{
		_label = new ICS_Text(*textField._label);
		addChild(_label);
	}

	// start recieving events
	if (_active)
	{
		ICS_Game::getInstance().addUpdateEventListener(this);
		ICS_Game::getInstance().addKeyboardEventListener(this);
	}

	// return a reference to this instance
	return *this;
}

/**
 * Adds a text label centered vertically on the left side if the text field.
 *
 * @param text		The text to use for the label.
 * @param color		The color that the text should appear in.
 */
void
ICS_TextField::addLabel(const std::string& text, const ICS_Color& color)
{
	// check it the label needs to be created
	if (not _label)
	{
		_label = new ICS_Text(_font);
		_label->setAnchor(1.0f, 0.5f);
		_label->setY(getHeight() / 2.0f);
		_label->setColor(color);
		addChild(_label);
	}

	// set the text for the label
	_label->setText(text);
}

/**
 * Handles update events.
 *
 * @param elapsed	The number of seconds that have elapsed since the last update.
 */
void
ICS_TextField::handleUpdateEvent(float elapsed)
{
	// update the cursor
	_cursorTimer += elapsed;
	_cursorTimer = _cursorTimer - (int)_cursorTimer;

	// check for repeating keys
	if (_enabled and _repeatKey != ICS_KEY_INVALID)
	{
		// update the timer
		_repeatTimer += elapsed;

		// check for starting to repeat
		if (!_repeat and _repeatTimer >= _repeatDelay)
		{
			_repeatTimer -= _repeatDelay;
			_repeat = true;
		}

		// check for repeating a key
		while (_repeat and _repeatTimer >= _repeatRate)
		{
			_repeatTimer -= _repeatRate;
			onKeyPress(_repeatKey);
		}
	}
}

/**
 * This handles keyboard events.
 *
 * @param key			The id of the key that was pressed / released.  Key constants can be found in ICS_Constants.h
 * @param eventType		The type of event (ICS_EVENT_PRESS or ICS_EVENT_RELEASE)
 */
void
ICS_TextField::handleKeyboardEvent(int key, int eventType)
{
	// make sure the text field is active and enabled
	if (_active and _enabled)
	{
		// keep track of shift presses and releases (so it can be determined if the shift key is held)
		if (key == ICS_KEY_SHIFT)
		{
			_shiftCount += (eventType == ICS_EVENT_PRESS) ? 1 : -1;
		}

		// handle key presses
		else if (eventType == ICS_EVENT_PRESS)
		{
			// handle the key press
			onKeyPress(key);

			// check for repeats
			if (key != ICS_KEY_ENTER)
			{
				_repeatKey = key;
				_repeatTimer = 0.0f;
				_repeat = false;
			}
		}

		// handle key releases
		else
		{
			_repeatKey = ICS_KEY_INVALID;
		}
	}
}

/**
 * This is called when a mouse button is pressed over the text field.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 * @param mouseX	The x position of the mouse cursor.
 * @param mouseY	The y position of the mouse cursor.
 */
void
ICS_TextField::handleMousePressOver(int button, float mouseX, float mouseY)
{
	// invoke the parent function
	ICS_TextRenderable::handleMousePressOver(button, mouseX, mouseY);

	// the text field will accept input after it is left clicked on (active)
	if (button == ICS_LEFT_MOUSE_BUTTON)
	{
		// activate the text field
		_active = true;

		// start recieving events
		ICS_Game::getInstance().addUpdateEventListener(this);
		ICS_Game::getInstance().addKeyboardEventListener(this);
	}
}

/**
 * This is called when a mouse button is pressed somewhere NOT over the renderable.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 */
void
ICS_TextField::handleMousePressNotOver(int button)
{
	// invoke the parent function
	ICS_TextRenderable::handleMousePressNotOver(button);

	// the text field will not accept input after there is a left mouse click elsewhere
	if (_active and button == ICS_LEFT_MOUSE_BUTTON)
	{
		// stop recieving events
		ICS_Game::getInstance().removeUpdateEventListener(this);
		ICS_Game::getInstance().removeKeyboardEventListener(this);

		// deactivate the text field
		_active = false;

		// trigger an event
		triggerChildEvent(ICS_EVENT_LOSE_FOCUS);
	}
}

/**
 * This handles a single key press on the keyboard.
 *
 * @param key	The id of the key that was pressed.  Key constants can be found in ICS_Constants.h
 */
void
ICS_TextField::onKeyPress(int key)
{
	// check for invalid keys
	if (_digitsOnly and not (key >= ICS_KEY_0 and key <= ICS_KEY_9 or key == ICS_KEY_ENTER or key == ICS_KEY_BACKSPACE))
	{
		return;
	}

	// check for character limit
	if (_characterLimit != ICS_NO_CHARACTER_LIMIT and _value.length() >= (unsigned int)_characterLimit and not (key == ICS_KEY_ENTER or key == ICS_KEY_BACKSPACE))
	{
		return;
	}

	// make sure the text field is active and enabled
	if (_active and _enabled)
	{
		// if the enter jey is pressed, deactivate the field and trigger an event
		if (key == ICS_KEY_ENTER)
		{
			// stop recieving update events
			ICS_Game::getInstance().removeUpdateEventListener(this);

			// deactivate the text field
			_active = false;

			// trigger an event
			triggerChildEvent(ICS_EVENT_LOSE_FOCUS);

			// the key press was handled... quit the function
			return;
		}

		// when backspace is pressed, remove a character from the text field
		if (key == ICS_KEY_BACKSPACE)
		{
			// remove a character from the end of the text
			if (_value.length() > 0)
			{
				_value = _value.substr(0, _value.length() - 1);
			}
		}

		// if shift is NOT being held, add the ASCII value of the key to the prompt
		else if (_shiftCount == 0 and key < 256 and ICS_KEY_CHAR_MAP[key] != 0)
		{
			// don't allow the first character in the field to be a space
			if (_value.length() > 0 or ICS_KEY_CHAR_MAP[key] != ' ')
			{
				_value += ICS_KEY_CHAR_MAP[key];
			}
		}

		// if the shift key is being held, add the ASCII value of the shifted key to the prompt
		else if (_shiftCount > 0 and key < 256 and ICS_SHIFT_KEY_CHAR_MAP[key] != 0)
		{
			// don't allow the first character in the field to be a space
			if (_value.length() > 0 or ICS_SHIFT_KEY_CHAR_MAP[key] != ' ')
			{
				_value += ICS_SHIFT_KEY_CHAR_MAP[key];
			}
		}

		// trigger an event
		triggerChildEvent(ICS_EVENT_CHANGE);
	}
}

/**
 * Renders the text field.
 */
void
ICS_TextField::render()
{
	// make sure the font is initialized
	if (_font->isInitialized())
	{
		// determine the border size to center the text in the text field
		float border = ICS_max(0.0f, (getHeight() - _font->getHeight()) / 2);

		// get the text to render
		std::string temp = _value;

		// check if the cursor should be displayed
		if (_active and _enabled and _cursorTimer >= 0.5f)
		{
			temp += _cursorCharacter;
		}

		// determine the x position to start rendering
		int textWidth = _font->getTextWidth(_value) + _font->getCharacterWidth(_cursorCharacter);
		int x = ICS_min(border, (getWidth() - border) - textWidth);

		// set the color
		_color.setRenderColor();

		// enable the stencil test to clip text outside the specified area
		enableStencilTest(border, getWidth() - border, 0, getHeight());

		// render the text
		_font->render(x, border, temp);

		// disable the stencil test
		disableStencilTest();
	}
}

/**
 * Sets the dimensions of the text box.
 * This function is a catch-all for dimension changes.
 * If the text box's dimensions change, this function will be called.
 *
 * @param dimensions	The dimensions to assign to the text box.
 */
void
ICS_TextField::_setDimensions(const ICS_Pair<float>& dimensions)
{
	// invoke the parent function to set the dimensions of the renderable and calculate the inverse transformation matrix
	ICS_TextRenderable::_setDimensions(dimensions);

	// center the label vertically
	if (_label)
	{
		_label->setY(getHeight() / 2.0f);
	}
}

/**
 * Frees memory and stops listening for events.
 */
void
ICS_TextField::reset()
{
	// stop recieving events
	if (_active)
	{
		ICS_Game::getInstance().removeUpdateEventListener(this);
		ICS_Game::getInstance().removeKeyboardEventListener(this);
	}

	// delete the label
	if (_label)
	{
		delete _label;
		_label = NULL;
	}
}