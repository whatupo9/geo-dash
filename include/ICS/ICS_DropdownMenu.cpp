#include "ICS_DropdownMenu.h"	// the declaration of ICS_DropdownMenu
#include "ICS_Button.h"			// the declaration of ICS_Button (for selecting options)
#include "ICS_Window.h"			// the declaration of ICS_Window (for holding all the options)

/**
 * The constructor.
 *
 * @param fontFileName	The name of the file containing the font.
 * @param fontHeight	The height of the font (in pixels).
 * @param width			The width of the main button (in pixels).
 * @param buttonHeight	The height of the main button (in pixels).
 * @param windowHeight	The height of the options window (in pixels).
 */
ICS_DropdownMenu::ICS_DropdownMenu(std::string fontFileName, int fontHeight, float width, float buttonHeight, float windowHeight)
	:
	_fontFileName(fontFileName),
	_fontHeight(fontHeight),
	_enabled(true)
{
	// place the anchor point in the top left corner of the menu
	setAnchor(0.0f, 0.0f);

	// some colors for initialization
	ICS_Color black(ICS_COLOR_MIN, ICS_COLOR_MIN, ICS_COLOR_MIN);
	ICS_Color white(ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX);
	ICS_Color blue(ICS_COLOR_MIN, ICS_COLOR_MIN, ICS_COLOR_MAX);

	// create a button for displaying the selected value and toggling the options list
	_button = new ICS_Button(width, buttonHeight);
	_button->setAnchor(0.0f, 0.0f);
	_button->setText(fontFileName, fontHeight, "", ICS_CENTER);
	addChild(_button);

	// create a window for displaying all the options
	_optionWindow = new ICS_Window(width, windowHeight);
	_optionWindow->setPosition(0.0f, (float)buttonHeight);
	_optionWindow->setVisible(false);
	_optionWindow->enableScrolling();
	addChild(_optionWindow);

	// set the default option colors (white, blue hover, black text)
	setOptionColors(white, blue, blue);
	setOptionTextColors(black, black, black);

	// set the dimensions of the menu
	setDimensions(width, buttonHeight);
}

/**
 * The copy constructor.
 *
 * @param menu	The dropdown menu to copy.
 */
ICS_DropdownMenu::ICS_DropdownMenu(const ICS_DropdownMenu& menu)
	:
	ICS_Renderable(menu),
	_fontFileName(menu._fontFileName),
	_fontHeight(menu._fontHeight),
	_button(new ICS_Button(*menu._button)),
	_optionWindow(new ICS_Window(*menu._optionWindow)),
	_enabled(menu._enabled)
{
	// add the children
	addChild(_button);
	addChild(_optionWindow);
	
	// copy the colors
	for (int i = 0; i < ICS_BUTTON_STATES; i++)
	{
		_optionColors[i] = menu._optionColors[i];
		_optionTextColors[i] = menu._optionTextColors[i];
	}

	// copy the options
	for (unsigned int i = 0; i < menu._optionButtons.size(); i++)
	{
		addOption(menu._optionButtons[i]->getText());
	}
}

/**
 * The destructor.
 */
ICS_DropdownMenu::~ICS_DropdownMenu()
{
	// delete each option button
	for (unsigned int i = 0; i < _optionButtons.size(); i++)
	{
		delete _optionButtons[i];
	}

	_optionButtons.clear();

	// delete the main button
	delete _button;

	// delete the window for displaying the options
	delete _optionWindow;
}

/**
 * Assignment operator.
 *
 * @param menu		The dropdown menu to copy.
 *
 * @returns			A reference to this instance.
 */
ICS_DropdownMenu&
ICS_DropdownMenu::operator=(const ICS_DropdownMenu& menu)
{
	// delete each option button
	for (unsigned int i = 0; i < _optionButtons.size(); i++)
	{
		delete _optionButtons[i];
	}

	_optionButtons.clear();

	// delete the main button
	delete _button;

	// delete the window for displaying the options
	delete _optionWindow;

	// invoke the parent assignment operator
	ICS_Renderable::operator=(menu);

	// copy each attribute
	_fontFileName = menu._fontFileName;
	_fontHeight = menu._fontHeight;
	_button = new ICS_Button(*menu._button);
	_optionWindow = new ICS_Window(*menu._optionWindow);
	_enabled = menu._enabled;

	// add the children
	addChild(_button);
	addChild(_optionWindow);

	// copy the colors
	for (int i = 0; i < ICS_BUTTON_STATES; i++)
	{
		_optionColors[i] = menu._optionColors[i];
		_optionTextColors[i] = menu._optionTextColors[i];
	}

	// copy the options
	for (unsigned int i = 0; i < menu._optionButtons.size(); i++)
	{
		addOption(menu._optionButtons[i]->getText());
	}

	// return a reference to this instance
	return *this;
}

/**
 * Adds an option to the menu.  This is ignored if the option already exists.
 *
 * @param option	The option to add to the menu.
 */
void
ICS_DropdownMenu::addOption(std::string option)
{
	// check if the option already exists
	for (unsigned int i = 0; i < _optionButtons.size(); i++)
	{
		// if the option already exists, exit the function
		if (option == _optionButtons[i]->getText())
		{
			return;
		}
	}

	// create a button to represent the option
	ICS_Button* button = new ICS_Button(getWidth(), getHeight());
	button->setAnchor(0.0f, 0.0f);
	button->setColor(_optionColors[ICS_BUTTON_STATE_UP], _optionColors[ICS_BUTTON_STATE_DOWN], _optionColors[ICS_BUTTON_STATE_HOVER], _optionColors[ICS_BUTTON_STATE_DISABLED]);
	button->setText(_fontFileName, _fontHeight, option, ICS_CENTER);
	button->setTextColor(_optionTextColors[ICS_BUTTON_STATE_UP], _optionTextColors[ICS_BUTTON_STATE_DOWN], _optionTextColors[ICS_BUTTON_STATE_HOVER], _optionTextColors[ICS_BUTTON_STATE_DISABLED]);
	button->setPosition(0.0f, (float)(_optionButtons.size() * getHeight()));

	// add the button to the option window
	_optionWindow->addChild(button);
	_optionWindow->setContentDimensions(getWidth(), getHeight() * (_optionButtons.size() + 1), false);

	// add the button to the list of options
	_optionButtons.push_back(button);
}

/**
 * Changes an option in the menu.
 *
 * @param oldValue		The value of the option to change.
 * @param newValue		The value to assign to the option.
 */
void
ICS_DropdownMenu::changeOption(std::string oldValue, std::string newValue)
{
	// check if the option exists
	for (unsigned int i = 0; i < _optionButtons.size(); i++)
	{
		// if the option exists, update it
		if (oldValue == _optionButtons[i]->getText())
		{
			_optionButtons[i]->setText(newValue, ICS_CENTER);
		}
	}

	// if this is the currently selected option, update the button as well
	if (oldValue == _button->getText())
	{
		_button->setText(newValue, ICS_CENTER);
	}
}

/**
 * Enables the menu.  Changes the appearance and allows interaction with the mouse.
 */
void
ICS_DropdownMenu::enable()
{
	// set the member attribute to allow mouse interaction
	_enabled = true;

	// enable the button
	_button->enable();
}

/**
 * Disables the menu.  Changes the appearance and prevents interaction with the mouse.
 */
void
ICS_DropdownMenu::disable()
{
	// set the member attribute to prevent mouse interaction
	_enabled = false;

	// disable the button
	_button->disable();

	// hide the options (no need to disable because the user can't see or interact with invisible objects)
	_optionWindow->setVisible(false);
}

/**
 * Sets the value assigned to the menu.  Adds the value as an option if it wasn't already added.
 *
 * @param value		The value to assign to the menu.
 */
void
ICS_DropdownMenu::setValue(std::string value)
{
	// add the option
	addOption(value);

	// sets the option as the currently selected value
	_button->setText(value, ICS_CENTER);
}

/**
 * Sets the color of each state of the button.
 *
 * @param upColor		The color to render the button when it is not being interacted with.
 * @param downColor		The color to render the button when the slider is being held.
 * @param hoverColor	The color to render the button when the mouse cursor is over it.
 * @param disabledColor	The color to render the button when the menu is disabled.
 */
void
ICS_DropdownMenu::setButtonColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor, const ICS_Color& disabledColor)
{
	_button->setColor(upColor, downColor, hoverColor, upColor);
}

/**
 * Sets the color of the text for each state of the button.
 *
 * @param upColor		The color to render the button text when the menu is not being interacted with.
 * @param downColor		The color to render the button text when the button is being held.
 * @param hoverColor	The color to render the button text when the mouse cursor is over the button.
 * @param disabledColor	The color to render the button text when the menu is disabled.
 */
void
ICS_DropdownMenu::setButtonTextColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor, const ICS_Color& disabledColor)
{
	_button->setTextColor(upColor, downColor, hoverColor, upColor);
}

/**
 * Sets the color of each state of the option buttons.
 *
 * @param upColor		The color to render the button when it is not being interacted with.
 * @param downColor		The color to render the button when it is being held.
 * @param hoverColor	The color to render the button when the mouse cursor is over it.
 */
void
ICS_DropdownMenu::setOptionColors(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor)
{
	// keep track of the colors for new options added
	_optionColors[ICS_BUTTON_STATE_UP] = upColor;
	_optionColors[ICS_BUTTON_STATE_DOWN] = downColor;
	_optionColors[ICS_BUTTON_STATE_HOVER] = hoverColor;
	_optionColors[ICS_BUTTON_STATE_DISABLED] = upColor;

	// update exisiting options
	for (unsigned int i = 0; i < _optionButtons.size(); i++)
	{
		_optionButtons[i]->setColor(upColor, downColor, hoverColor, upColor);
	}
}

/**
 * Sets the color of the text for each state of the option buttons.
 *
 * @param upColor		The color to render the button text when the menu is not being interacted with.
 * @param downColor		The color to render the button text when the button is being held.
 * @param hoverColor	The color to render the button text when the mouse cursor is over the button.
 */
void
ICS_DropdownMenu::setOptionTextColors(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor)
{
	// keep track of the colors for new options added
	_optionTextColors[ICS_BUTTON_STATE_UP] = upColor;
	_optionTextColors[ICS_BUTTON_STATE_DOWN] = downColor;
	_optionTextColors[ICS_BUTTON_STATE_HOVER] = hoverColor;
	_optionTextColors[ICS_BUTTON_STATE_DISABLED] = upColor;

	// update exisiting options
	for (unsigned int i = 0; i < _optionButtons.size(); i++)
	{
		_optionButtons[i]->setTextColor(upColor, downColor, hoverColor, upColor);
	}
}

/**
 * Gets the option that is currently selected.
 *
 * @returns		The option that is currently selected.
 */
std::string
ICS_DropdownMenu::getValue() const
{
	return _button->getText();
}

/**
 * This is called when a mouse button is pressed somewhere NOT over the renderable.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 */
void
ICS_DropdownMenu::handleMousePressNotOver(int button)
{
	// invoke the parent function
	ICS_Renderable::handleMousePressNotOver(button);

	// hide the options when a left click occurs somewhere else
	if (button == ICS_LEFT_MOUSE_BUTTON)
	{
		_optionWindow->setVisible(false);
	}
}

/**
 * This handles generic child events (for children to notify their parent).
 *
 * @param child			The child that triggered the event.
 * @param eventType		The type of event.
 */
void
ICS_DropdownMenu::handleChildEvent(ICS_Renderable* child, int eventType)
{
	// invoke the parent function
	ICS_Renderable::handleChildEvent(child, eventType);

	// make sure the menu is enabled
	if (_enabled)
	{
		// if the main button was clicked, toggle visibility of the options
		if (child == _button and eventType == ICS_EVENT_PRESS)
		{
			_optionWindow->setVisible(not _optionWindow->isVisible());
		}

		// if something was clicked, check if it was an option button
		else if (eventType == ICS_EVENT_PRESS)
		{
			// check each option button
			bool found = false;

			for (unsigned int i = 0; i < _optionButtons.size() and not found; i++)
			{
				// check if this option was clicked
				if (child == _optionButtons[i])
				{
					// hide the options
					_optionWindow->setVisible(false);

					// make sure this is a different option than the one that was already selected
					if (_button->getText() != _optionButtons[i]->getText())
					{
						// update the value of the dropdown menu
						_button->setText(_optionButtons[i]->getText(), ICS_CENTER);

						// invoke the callbacks
						triggerChildEvent(ICS_EVENT_CHANGE);
					}
				}
			}
		}
	}
}