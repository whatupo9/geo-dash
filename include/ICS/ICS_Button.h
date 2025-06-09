/*

ICS_Button

	Created: 2020-03-27

	Change log:

		2020-03-27
			- created ICS_Button for buttons used in GUIs

		2020-03-30
			- ICS_Button inherits from ICS_AnimatedSprite (instead of having a sprite)
			- added animations for hover, up and down
			- added functionality to disable button

		2020-04-02
			- use mouse enter and mouse leave events instead of mouse move events

		2024-01-19
			- now inherits from ICS_Renderable instead of ICS_AnimatedSprite for ease of use
			- can now be created without a sprite (the button can be a colored rectangle with text)
			- now uses polygon color (brighten / darken) instead of multiple textures for animation

		2024-01-24
			- now uses a tileset (multiple textures) instead of polygon color 
			- added a state for the button (up, down, hover and disabled)

		2024-01-26
			- added functions to set and get text assigned to the button

		2024-01-28
			- added a function that can enable or disable the button

		2024-02-03
			- added default colors for the text and sprite and removed these parameters from the constructor / addText function
			- added a functions to set sprite and text color

		2024-02-10
			- added ability to set the text justification

		2024-02-17
			- inherits from ICS_Resource so it can be initialized when the game engine is initialized

		2024-05-08
			- fixed sprite and text positioning bug introduced by fixing anchor points in ICS_Renderable
			- the class still needs a proper copy constructor and assignment operator

		2024-06-06
			- added copy constructor and assignment operator

*/

#pragma once

#include "ICS_Renderable.h"	// ICS_Button inherits from ICS_Renderable
#include "ICS_Resource.h"	// ICS_Button inherits from ICS_Resource so it can be automatically initialized when the game engine is ready
#include "ICS_Types.h"		// for callback function pointer definitions

class ICS_Sprite;			// for rendering the button
class ICS_Text;				// for adding a label to the button
class ICS_Tileset;			// for swapping the sprite's texture

/**
 * A button for use in a 2D GUI.
 */
class ICS_Button : public ICS_Renderable, public ICS_Resource
{

private:

// member attributes

	std::string _tilesetFileName;				// the name of the file to use for the button's tileset
	int _tileWidth;								// the width of each tile in pixels
	int _tileHeight;							// the height of each tile in pixels

	ICS_Sprite* _sprite;						// for rendering the button
	ICS_Tileset* _tileset;						// for the textures to use for the sprite (up, down, hover, disabled)

	ICS_Text* _text;							// for adding a label to the button
	int _textJustification;						// where to locate the text on the button (ICS_LEFT, ICS_RIGHT or ICS_CENTER)
	int _fontHeight;							// the height of the font in pixels

	int _state;									// the state of the button (up, down, hover, disabled)
	ICS_Color _spriteColors[ICS_BUTTON_STATES];	// sprite colors for each state of the button.
	ICS_Color _textColors[ICS_BUTTON_STATES];	// text colors for each state of the button.

public:

// constructors / destructor

	/**
	 * The constructor which creates a button using a tileset.
	 *
	 * @param width			The width of the button (in pixels).
	 * @param height		The height of the button (in pixels).
	 * @param fileName		The name of the file containing the tileset.
	 * @param tileWidth		The width of each tile in pixels.
	 * @param tileHeight	The height of each tile in pixels.
	 */
	ICS_Button(float width, float height, const std::string& tilesetFileName, int tileWidth, int tileHeight);

	/**
	 * The constructor which creates a button using a colored sprite.
	 *
	 * @param width		The width of the button (in pixels).
	 * @param height	The height of the button (in pixels).
	 */
	ICS_Button(float width, float height);

	/**
	 * The copy constructor.
	 *
	 * @param button	The button to copy.
	 */
	ICS_Button(const ICS_Button& button);

	/**
	 * The destructor.
	 */
	~ICS_Button();

// assignment operator

	/**
	 * Assignment operator.
	 *
	 * @param button	The button to copy.
	 * 
	 * @returns			A reference to this instance.
	 */
	ICS_Button& operator=(const ICS_Button& button);

// setters

	/**
	 * Enables / disables the button.
	 *
	 * @param enabled	If true, the button is enabled.  If false, the button is disabled.
	 */
	void setEnabled(bool enabled);

	/**
	 * Sets the color of the button's sprite for each button state.
	 *
	 * @param upColor		The color to render the sprite when the button is up.
	 * @param downColor		The color to render the sprite when the button is down.
	 * @param hoverColor	The color to render the sprite when the mouse cursor is over the button.
	 * @param disabledColor	The color to render the sprite when the button is disabled.
	 */
	void setColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor, const ICS_Color& disabledColor);

// text functions

	/**
	 * Adds a text to the button.
	 *
	 * @param fontFileName		The name of the file containing the font.
	 * @param fontHeight		The height of the font (in pixels).
	 * @param text				The text to render on the button.
	 * @param justification		Where to locate the text on the button (ICS_LEFT, ICS_RIGHT or ICS_CENTER).
	 */
	void setText(std::string fontFileName, int fontHeight, std::string text, int justification);

	/**
	 * Sets the button's text, if text has already been added.
	 *
	 * @param text				The text to render on the button.
	 * @param justification		Where to locate the text on the button (ICS_LEFT, ICS_RIGHT or ICS_CENTER).
	 */
	void setText(std::string text, int justification);

	/**
	 * Sets the color of the button's text for each button state.
	 *
	 * @param upColor		The color to render the text when the button is up.
	 * @param downColor		The color to render the text when the button is down.
	 * @param hoverColor	The color to render the text when the mouse cursor is over the button.
	 * @param disabledColor	The color to render the text when the button is disabled.
	 */
	void setTextColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor, const ICS_Color& disabledColor);

	/**
	 * Gets the text assigned to the button.
	 *
	 * @returns		The text assigned to the button.
	 */
	std::string getText();

// mutators

	/**
	 * Disables the button.
	 */
	void disable();

	/**
	 * Enables the button.
	 */
	void enable();

// event handling

	/**
	 * This is called when the mouse moves over the button.
	 *
	 * @param mouseX	The x position of the mouse cursor.
	 * @param mouseY	The y position of the mouse cursor.
	 */
	void handleMouseEnter(float mouseX, float mouseY);

	/**
	 * This is called when the mouse moves off the button.
	 */
	void handleMouseLeave();

	/**
	 * This is called when a mouse button is pressed over the button.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param mouseX	The x position of the mouse cursor.
	 * @param mouseY	The y position of the mouse cursor.
	 */
	void handleMousePressOver(int button, float mouseX, float mouseY);

	/**
	 * This is called when a mouse button is pressed over the button but released off the button.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 */
	void handleMouseReleaseNotOver(int button);

	/**
	 * This is called when a mouse button is pressed and released over the button.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param mouseX	The x position of the mouse cursor.
	 * @param mouseY	The y position of the mouse cursor.
	 */
	void handleMouseClick(int button, float mouseX, float mouseY);

protected:

// helpers

	/**
	 * Sets the button's state and updates the children to reflect the state.
	 *
	 * @param state	The state of the button (up, down, hover, disabled).
	 */
	void setState(int state);

	/**
	 * Prepares the renderable for use.  Should be called after the graphics pipeline is initialized and before the renderable is used.
	 */
	void initialize();

	/**
	 * Updates the attributes of the text based on the justification and dimensions of the buttons.
	 */
	void updateText();

// transformation helpers

	/**
	 * Sets the dimensions of the object.
	 * This function is a catch-all for dimension changes.
	 * If the object's dimensions change, this function will be called.
	 *
	 * @param dimensions	The dimensions to assign to the object.
	 */
	void _setDimensions(const ICS_Pair<float>& dimensions);

};
