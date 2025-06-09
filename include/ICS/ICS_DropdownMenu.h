/*

ICS_DropdownMenu

	Created: 2020-03-31

	Change log:

		2020-03-31
			- created ICS_DropdownMenu selecting a value from a set of options in a GUI

		2024-01-26
			- now uses ICS_Window and ICS_Button instances for the options
			- added a function to add options

		2024-02-03
			- added functions for setting colors for the button, text and options

		2024-02-06
			- added a callback for when the selected option changes
			- trigger child event when an option is selected
			- added a function to get the currently selected option

		2024-02-08
			- added a function to set the currently selected option

		2024-02-10
			- added a function update a value in the menu

		2024-02-12
			- added the ability to enable / disable the menu

		2024-05-17
			- fixed bug where option font and font size did not match the values passed in to the constructor (test code that was never replaced)

		2024-06-07
			- added copy constructor and assignment operator

*/

#pragma once

#include "ICS_Renderable.h"	// ICS_DropdownMenu inherits from ICS_Renderable
#include "ICS_Types.h"		// for ICS_EventFunction

class ICS_Button;			// for rendering the main button and options
class ICS_Window;			// for rendering the options in a scrolling window

/**
 * A button for use in a 2D GUI.
 */
class ICS_DropdownMenu : public ICS_Renderable
{

private:

// member attributes

	std::string _fontFileName;						// The name of the file containing the font.
	int _fontHeight;								// The height of the font (in pixels).

	ICS_Button* _button;							// the main button displaying the current value selected
	ICS_Window* _optionWindow;						// to represent the options window
	std::vector<ICS_Button*> _optionButtons;		// the options to choose from

	ICS_Color _optionColors[ICS_BUTTON_STATES];		// button colors for the option buttons.
	ICS_Color _optionTextColors[ICS_BUTTON_STATES];	// text colors for the option buttons.

	bool _enabled;									// indicates that the menu is enabled

public:

// constructors / destructor

	/**
	 * The constructor.
	 *
	 * @param fontFileName	The name of the file containing the font.
	 * @param fontHeight	The height of the font (in pixels).
	 * @param width			The width of the main button (in pixels).
	 * @param buttonHeight	The height of the main button (in pixels).
	 * @param windowHeight	The height of the options window (in pixels).
	 */
	ICS_DropdownMenu(std::string fontFileName, int fontHeight, float width, float buttonHeight, float windowHeight);

	/**
	 * The copy constructor.
	 *
	 * @param menu	The dropdown menu to copy.
	 */
	ICS_DropdownMenu(const ICS_DropdownMenu& menu);

	/**
	 * The destructor.
	 */
	~ICS_DropdownMenu();

// assignment operator

	/**
	 * Assignment operator.
	 *
	 * @param menu		The dropdown menu to copy.
	 * 
	 * @returns			A reference to this instance.
	 */
	ICS_DropdownMenu& operator=(const ICS_DropdownMenu& menu);

// modifiers

	/**
	 * Adds an option to the menu.  This is ignored if the option already exists.
	 *
	 * @param option	The option to add to the menu.
	 */
	void addOption(std::string option);

	/**
	 * Changes an option in the menu.
	 *
	 * @param oldValue		The value of the option to change.
	 * @param newValue		The value to assign to the option.
	 */
	void changeOption(std::string oldValue, std::string newValue);

	/**
	 * Enables the menu.  Changes the appearance and allows interaction with the mouse.
	 */
	void enable();

	/**
	 * Disables the menu.  Changes the appearance and prevents interaction with the mouse.
	 */
	void disable();

// setters

	/**
	 * Sets the value assigned to the menu.  Adds the value as an option if it wasn't already added.
	 *
	 * @param value		The value to assign to the menu.
	 */
	void setValue(std::string value);

	/**
	 * Sets the color of each state of the button.
	 *
	 * @param upColor		The color to render the button when it is not being interacted with.
	 * @param downColor		The color to render the button when it is being held.
	 * @param hoverColor	The color to render the button when the mouse cursor is over it.
	 * @param disabledColor	The color to render the button when the menu is disabled.
	 */
	void setButtonColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor, const ICS_Color& disabledColor);

	/**
	 * Sets the color of the text for each state of the button.
	 *
	 * @param upColor		The color to render the button text when the menu is not being interacted with.
	 * @param downColor		The color to render the button text when the button is being held.
	 * @param hoverColor	The color to render the button text when the mouse cursor is over the button.
	 * @param disabledColor	The color to render the button text when the menu is disabled.
	 */
	void setButtonTextColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor, const ICS_Color& disabledColor);
	
	/**
	 * Sets the color of each state of the option buttons.
	 *
	 * @param upColor		The color to render the button when it is not being interacted with.
	 * @param downColor		The color to render the button when it is being held.
	 * @param hoverColor	The color to render the button when the mouse cursor is over it.
	 */
	void setOptionColors(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor);

	/**
	 * Sets the color of the text for each state of the option buttons.
	 *
	 * @param upColor		The color to render the button text when the menu is not being interacted with.
	 * @param downColor		The color to render the button text when the button is being held.
	 * @param hoverColor	The color to render the button text when the mouse cursor is over the button.
	 */
	void setOptionTextColors(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor);

// getters

	/**
	 * Gets the option that is currently selected.
	 *
	 * @returns		The option that is currently selected.
	 */
	std::string getValue() const;

// event handlers

	/**
	 * This is called when a mouse button is pressed somewhere NOT over the renderable.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 */
	void handleMousePressNotOver(int button);

	/**
	 * This handles generic child events (for children to notify their parent).
	 *
	 * @param child			The child that triggered the event.
	 * @param eventType		The type of event.
	 */
	void handleChildEvent(ICS_Renderable* child, int eventType);

};