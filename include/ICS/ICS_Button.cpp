#include "ICS_Game.h"		// the declaration of ICS_Game
#include "ICS_Button.h"		// the declaration of ICS_Button
#include "ICS_Sprite.h"		// for rendering the button
#include "ICS_Text.h"		// for adding a label to the button
#include "ICS_Tileset.h"	// the declaration of ICS_Tileset

/**
 * The constructor which creates a button using a tileset.
 *
 * @param width			The width of the button (in pixels).
 * @param height		The height of the button (in pixels).
 * @param fileName		The name of the file containing the tileset.
 * @param tileWidth		The width of each tile in pixels.
 * @param tileHeight	The height of each tile in pixels.
 */
ICS_Button::ICS_Button(float width, float height, const std::string& tilesetFileName, int tileWidth, int tileHeight)
	:
	_tilesetFileName(tilesetFileName),
	_tileWidth(tileWidth),
	_tileHeight(tileHeight),
	_tileset(NULL),
	_text(NULL),
	_state(ICS_BUTTON_STATE_UP)
{
	// create the sprite
	_sprite = new ICS_Sprite(ICS_Color(ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX), width, height);
	_sprite->setAnchor(0.0f, 0.0f);
	addChild(_sprite);

	// anchor the button in the top left corner and apply the button's anchor point to the children
	setAnchor(0.0f, 0.0f);
	positionChildrenRelativeToOrigin();

	// set the dimensions of the button
	setDimensions(width, height);

	// attempt to initialize elements that that require ICS_Game to be initialized as well
	initialize();
}

/**
 * The constructor which creates a button using a colored sprite.
 *
 * @param width		The width of the button (in pixels).
 * @param height	The height of the button (in pixels).
 */
ICS_Button::ICS_Button(float width, float height)
	:
	_tileset(NULL),
	_text(NULL)
{
	// create the sprite
	_sprite = new ICS_Sprite(_spriteColors[ICS_BUTTON_STATE_UP], width, height);
	_sprite->setAnchor(0.0f, 0.0f);
	addChild(_sprite);

	// set the default colors (blue with white text)
	setColor(ICS_Color(0, 0, 185), ICS_Color(82, 73, 255), ICS_Color(128, 128, 255), ICS_Color(127, 127, 127));
	setTextColor(ICS_Color(192, 192, 192), ICS_Color(224, 224, 224), ICS_Color(255, 255, 255), ICS_Color(0, 0, 0));

	// update the appearance of the button
	setState(ICS_BUTTON_STATE_UP);

	// anchor the button in the top left corner and apply the button's anchor point to the children
	setAnchor(0.0f, 0.0f);
	positionChildrenRelativeToOrigin();

	// set the dimensions of the button
	setDimensions(width, height);
}

/**
 * The copy constructor.
 *
 * @param button	The button to copy.
 */
ICS_Button::ICS_Button(const ICS_Button& button)
	:
	ICS_Renderable(button),
	ICS_Resource(button),
	_tilesetFileName(button._tilesetFileName),
	_tileWidth(button._tileWidth),
	_tileHeight(button._tileHeight),
	_tileset(ICS_Tileset::copyTileset(button._tileset)),
	_text(NULL),
	_textJustification(button._textJustification),
	_fontHeight(button._fontHeight),
	_state(button._state)
{
	// copy the sprite
	_sprite = new ICS_Sprite(*button._sprite);
	addChild(_sprite);

	// copy the text
	if (button._text)
	{
		_text = new ICS_Text(*button._text);
		addChild(_text);
	}

	// copy the colors
	for (int i = 0; i < ICS_BUTTON_STATES; i++)
	{
		_spriteColors[i] = button._spriteColors[i];
		_textColors[i] = button._textColors[i];
	}
}

/**
 * The destructor.
 */
ICS_Button::~ICS_Button()
{
	// delete the sprite
	delete _sprite;

	// delete the tileset for the textures
	if (_tileset)
	{
		ICS_Tileset::deleteTileset(_tileset);
	}

	// delete the text for the label
	if (_text)
	{
		delete _text;
	}
}

/**
 * Assignment operator.
 *
 * @param button	The button to copy.
 *
 * @returns			A reference to this instance.
 */
ICS_Button&
ICS_Button::operator=(const ICS_Button& button)
{
	// delete the sprite
	delete _sprite;

	// delete the tileset for the textures
	if (_tileset)
	{
		ICS_Tileset::deleteTileset(_tileset);
	}

	// delete the text for the label
	if (_text)
	{
		delete _text;
	}

	// invoke the parent assignment operators
	ICS_Renderable::operator=(button);
	ICS_Resource::operator=(button);

	// copy each attribute
	_tilesetFileName = button._tilesetFileName;
	_tileWidth = button._tileWidth;
	_tileHeight = button._tileHeight;
	_tileset = ICS_Tileset::copyTileset(button._tileset);
	_text = NULL;
	_textJustification = button._textJustification;
	_fontHeight = button._fontHeight;
	_state = button._state;

	// copy the sprite
	_sprite = new ICS_Sprite(*button._sprite);
	addChild(_sprite);

	// copy the text
	if (button._text)
	{
		_text = new ICS_Text(*button._text);
		addChild(_text);
	}

	// copy the colors
	for (int i = 0; i < ICS_BUTTON_STATES; i++)
	{
		_spriteColors[i] = button._spriteColors[i];
		_textColors[i] = button._textColors[i];
	}

	// return a reference to this instance
	return *this;
}

/**
 * Enables / disables the button.
 *
 * @param enabled	If true, the button is enabled.  If false, the button is disabled.
 */
void
ICS_Button::setEnabled(bool enabled)
{
	// enable the button?
	if (enabled)
	{
		enable();
	}

	// disable the button?
	else
	{
		disable();
	}
}

/**
 * Sets the color of the button's sprite for each button state.
 *
 * @param upColor		The color to render the sprite when the button is up.
 * @param downColor		The color to render the sprite when the button is down.
 * @param hoverColor	The color to render the sprite when the mouse cursor is over the button.
 * @param disabledColor	The color to render the sprite when the button is disabled.
 */
void
ICS_Button::setColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor, const ICS_Color& disabledColor)
{
	// set the colors
	_spriteColors[ICS_BUTTON_STATE_UP] = upColor;
	_spriteColors[ICS_BUTTON_STATE_DOWN] = downColor;
	_spriteColors[ICS_BUTTON_STATE_HOVER] = hoverColor;
	_spriteColors[ICS_BUTTON_STATE_DISABLED] = disabledColor;

	// update the sprite's color
	_sprite->setColor(_spriteColors[_state]);
}

/**
 * Adds a text to the button.
 *
 * @param fontFileName		The name of the file containing the font.
 * @param fontHeight		The height of the font (in pixels).
 * @param text				The text to render on the button.
 * @param justification		Where to locate the text on the button (ICS_LEFT, ICS_RIGHT or ICS_CENTER).
 */
void
ICS_Button::setText(std::string fontFileName, int fontHeight, std::string text, int justification)
{
	// set the text attributes
	_textJustification = justification;
	_fontHeight = fontHeight;

	// delete the old label
	if (_text)
	{
		delete _text;
	}

	_text = new ICS_Text(fontFileName, fontHeight);	// create a new text object for labelling the button
	_text->setPriority(1);							// render the text on top of the sprite
	_text->setText(text);							// set the text to render
	_text->setColor(_textColors[_state]);			// set the color to diplay the text in based on the current state
	addChild(_text);								// add the text as a child of the button

	// position the text according to the justification
	updateText();
}

/**
 * Sets the button's text, if text has already been added.
 *
 * @param text				The text to render on the button.
 * @param justification		Where to locate the text on the button (ICS_LEFT, ICS_RIGHT or ICS_CENTER).
 */
void
ICS_Button::setText(std::string text, int justification)
{
	// update the text
	if (_text)
	{
		_text->setText(text);
	}
	
	// set the justification attribute
	_textJustification = justification;
	updateText();
}

/**
 * Sets the color of the button's text for each button state.
 *
 * @param upColor		The color to render the text when the button is up.
 * @param downColor		The color to render the text when the button is down.
 * @param hoverColor	The color to render the text when the mouse cursor is over the button.
 * @param disabledColor	The color to render the text when the button is disabled.
 */
void
ICS_Button::setTextColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor, const ICS_Color& disabledColor)
{
	// set the colors
	_textColors[ICS_BUTTON_STATE_UP] = upColor;
	_textColors[ICS_BUTTON_STATE_DOWN] = downColor;
	_textColors[ICS_BUTTON_STATE_HOVER] = hoverColor;
	_textColors[ICS_BUTTON_STATE_DISABLED] = disabledColor;

	// set the color to diplay the text in based on the current state
	if (_text)
	{
		_text->setColor(_textColors[_state]);
	}
}

 /**
  * Gets the text assigned to the button.
  *
  * @returns		The text assigned to the button.
  */
std::string
ICS_Button::getText()
{
	// make sure the text renderer has been created
	if (_text)
	{
		return _text->getText();
	}

	// default to empty string
	return "";
}

/**
 * Disables the button.
 */
void
ICS_Button::disable()
{
	setState(ICS_BUTTON_STATE_DISABLED);
}

/**
 * Enables the button.
 */
void
ICS_Button::enable()
{
	// check if the button is disabled
	if (_state == ICS_BUTTON_STATE_DISABLED)
	{
		// enable the button
		setState(ICS_BUTTON_STATE_UP);
	}
}

/**
 * This is called when the mouse moves over the button.
 *
 * @param mouseX	The x position of the mouse cursor.
 * @param mouseY	The y position of the mouse cursor.
 */
void
ICS_Button::handleMouseEnter(float mouseX, float mouseY)
{
	// invoke the parent punction
	ICS_Renderable::handleMouseEnter(mouseX, mouseY);

	// if the button is enabled and not being held down...
	if (_state == ICS_BUTTON_STATE_UP)
	{
		// the mouse cursor is over the button
		setState(ICS_BUTTON_STATE_HOVER);
	}
}

/**
 * This is called when the mouse moves off the button.
 */
void
ICS_Button::handleMouseLeave()
{
	// invoke the parent punction
	ICS_Renderable::handleMouseLeave();

	// if the button is enabled and not being held down...
	if (_state == ICS_BUTTON_STATE_HOVER)
	{
		// the mouse cursor is not over the button
		setState(ICS_BUTTON_STATE_UP);
	}
}

/**
 * This is called when a mouse button is pressed over the button.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 * @param mouseX	The x position of the mouse cursor.
 * @param mouseY	The y position of the mouse cursor.
 */
void
ICS_Button::handleMousePressOver(int button, float mouseX, float mouseY)
{
	// invoke the parent function
	ICS_Renderable::handleMousePressOver(button, mouseX, mouseY);

	// if the mouse button was not the left one or if this button is disabled, ignore the event
	if (button == ICS_LEFT_MOUSE_BUTTON and _state != ICS_BUTTON_STATE_DISABLED)
	{
		// the button is being held down
		setState(ICS_BUTTON_STATE_DOWN);

		// trigger the callbacks
		triggerChildEvent(ICS_EVENT_PRESS);
	}
}

/**
 * This is called when a mouse button is pressed over the button but released off the button.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 */
void
ICS_Button::handleMouseReleaseNotOver(int button)
{
	// invoke the parent function
	ICS_Renderable::handleMouseReleaseNotOver(button);

	// if the mouse button was not the left one or this button is disabled, ignore mouse input
	if (button == ICS_LEFT_MOUSE_BUTTON and _state != ICS_BUTTON_STATE_DISABLED)
	{
		// the button is no longer being held down
		setState(ICS_BUTTON_STATE_UP);

		// trigger the callbacks
		triggerChildEvent(ICS_EVENT_RELEASE);
	}
}

/**
 * This is called when a mouse button is pressed and released over the button.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 * @param mouseX	The x position of the mouse cursor.
 * @param mouseY	The y position of the mouse cursor.
 */
void
ICS_Button::handleMouseClick(int button, float mouseX, float mouseY)
{
	// invoke the parent function
	ICS_Renderable::handleMouseClick(button, mouseX, mouseY);

	// if the mouse button was not the left one or this button is disabled, ignore mouse input
	if (button == ICS_LEFT_MOUSE_BUTTON and _state != ICS_BUTTON_STATE_DISABLED)
	{
		// the button is no longer being held down
		setState(ICS_BUTTON_STATE_HOVER);

		// trigger the callbacks
		triggerChildEvent(ICS_EVENT_CLICK);
	}
}

/**
 * Sets the button's state and updates the children to reflect the state.
 *
 * @param state	The state of the button (up, down, hover, disabled).
 */
void
ICS_Button::setState(int state)
{
	// set the state
	_state = state;

	// update the sprite's color
	_sprite->setColor(_spriteColors[_state]);

	// update the sprite's texture
	if (_tileset)
	{
		_sprite->setTexture(_tileset->getTexture(_state));
	}

	// update the text color
	if (_text)
	{
		_text->setColor(_textColors[_state]);
	}
}

/**
 * Prepares the renderable for use.  Should be called after the graphics pipeline is initialized and before the renderable is used.
 */
void
ICS_Button::initialize()
{
	// make sure the engine is ready and the button hasn't already been initialized
	if (ICS_Game::getInstance().isInitialized() and not _initialized)
	{
		// check if a tileset needs to be created
		if (_tilesetFileName.length() > 0)
		{
			// create the tileset
			_tileset = ICS_Tileset::createTileset(_tilesetFileName, _tileWidth, _tileHeight);

			// update the sprite's texture to reflect the button's state
			_sprite->setTexture(_tileset->getTexture(_state));
		}

		// the button is initialized
		_initialized = true;
	}
}

/**
 * Updates the attributes of the text based on the justification and dimensions of the buttons.
 */
void
ICS_Button::updateText()
{
	// make sure there is text for this button
	if (_text)
	{
		// calculate the border to put around the text based on the height values of the button and the text
		float border = ICS_max(0.0f, getHeight() - _fontHeight) / 2.0f;

		// ensure the text does not render off the sides of the button
		_text->enableClippling(getWidth() - border * 2);

		// position the text over the button based on the justification
		if (_textJustification == ICS_CENTER)
		{
			_text->setAnchor(0.5f, 0.5f);
			_text->setPosition(getWidth() / 2.0f, getHeight() / 2.0f);
		}
		else if (_textJustification == ICS_RIGHT)
		{
			_text->setAnchor(1.0f, 0.5);
			_text->setPosition(getWidth() - border, getHeight() / 2.0f);
		}
		else
		{
			_text->setAnchor(0.0f, 0.5);
			_text->setPosition(border, getHeight() / 2.0f);
		}
	}
}

/**
 * Sets the dimensions of the object.
 * This function is a catch-all for dimension changes.
 * If the object's dimensions change, this function will be called.
 *
 * @param dimensions	The dimensions to assign to the object.
 */
void
ICS_Button::_setDimensions(const ICS_Pair<float>& dimensions)
{
	// invoke the parent function
	ICS_Renderable::_setDimensions(dimensions);

	// the sprite's dimensions should match the button's
	if (_sprite)
	{
		_sprite->setDimensions(dimensions);
	}
	
	// position the text according to the justification
	updateText();
}
