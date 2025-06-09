#include "ICS_Game.h"			// the definition of ICS_Game
#include "ICS_TextRenderable.h"	// the definition of ICS_TextRenderable
#include "ICS_Font.h"			// the definition of ICS_Font
#include "ICS_Sprite.h"			// the definition of ICS_Sprite
#include "ICS_Helpers.h"		// for ICS_clamp

/**
 * The ICS_TextRenderable constructor.
 *
 * @param fontFileName	The name of the file containing the font.
 * @param fontHeight	The height of the font in pixels.	
 * @param width			The width of the text box in pixels. 
 * @param height		The height of the text box in pixels.
 */
ICS_TextRenderable::ICS_TextRenderable(std::string fontFileName, int fontHeight, int width, int height)
	:
	_font(NULL),
	_background(NULL)
{
	// create the font
	_font = ICS_Font::createFont(fontFileName, fontHeight);

	// set the position in the top left corner of the text box
	setAnchor(0.0f, 0.0f);

	// set the dimensions
	setDimensions(width, height);
}

/**
 * The ICS_TextRenderable constructor.
 *
 * @param font		The font to use when rendering text.
 * @param width		The width of the text box in pixels.
 * @param height	The height of the text box in pixels.
 */
ICS_TextRenderable::ICS_TextRenderable(ICS_Font* font, int width, int height)
	:
	_font(NULL),
	_background(NULL)
{
	// copy the font
	_font = ICS_Font::copyFont(font);

	// set the position in the top left corner of the text box
	setAnchor(0.0f, 0.0f);

	// set the dimensions
	setDimensions(width, height);
}

/**
 * The ICS_TextRenderable copy constructor.
 *
 * @param textRenderable	The text renderable to copy.
 */
ICS_TextRenderable::ICS_TextRenderable(const ICS_TextRenderable& textRenderable)
	:
	ICS_Renderable(textRenderable)
{
	// copy the font
	_font = ICS_Font::copyFont(textRenderable._font);

	// copy the background
	if (textRenderable._background)
	{
		_background = new ICS_Sprite(*textRenderable._background);
		addChild(_background);
	}
}

/**
 * The ICS_TextRenderable destructor.
 */
ICS_TextRenderable::~ICS_TextRenderable()
{
	// delete the font
	ICS_Font::deleteFont(_font);

	// delete the background
	if (_background)
	{
		delete _background;
	}
}

/**
 * The assignment operator.
 *
 * @param textRenderable	The text renderable to copy.
 *
 * @returns					A reference to this instance.
 */
ICS_TextRenderable&
ICS_TextRenderable::operator=(const ICS_TextRenderable& textRenderable)
{
	// delete the old font
	ICS_Font::deleteFont(_font);

	// delete the background
	if (_background)
	{
		delete _background;
	}

	// invoke the parent assignment operator
	ICS_Renderable::operator=(textRenderable);

	// copy the font
	_font = ICS_Font::copyFont(textRenderable._font);

	// copy the background
	if (textRenderable._background)
	{
		_background = new ICS_Sprite(*textRenderable._background);
		addChild(_background);
	}

	// return a reference to this instance
	return *this;
}

/**
 * Adds a background sprite to the textbox.
 *
 * @param color		The color to render the background.
 */
void
ICS_TextRenderable::addBackground(const ICS_Color& color)
{
	// does the background already exist?
	if (_background)
	{
		_background->removeTexture();
		_background->setColor(color);
	}

	// does the background need to be created?
	else
	{
		_background = new ICS_Sprite(color, getWidth(), getHeight());
		_background->setAnchor(getAnchor());
		addChild(_background);
	}
}

/**
 * Adds a background sprite to the textbox.
 *
 * @param imageFileName		The name of the file containing the image.
 */
void
ICS_TextRenderable::addBackground(const std::string& imageFileName)
{
	// does the background already exist?
	if (_background)
	{
		_background->setColor(ICS_Color(ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX));
		_background->loadImage(imageFileName);
	}

	// does the background need to be created?
	else
	{
		_background = new ICS_Sprite(imageFileName, getWidth(), getHeight());
		_background->setAnchor(getAnchor());
		addChild(_background);
	}
}

/**
 * Sets the anchor of the renderable.
 * This function is a catch-all for anchor changes.
 * If the renderable's anchor changes, this function will be called.
 *
 * @param anchor	The anchor to assign to the renderable.
 */
void
ICS_TextRenderable::_setAnchor(ICS_Pair<float> anchor)
{
	// invoke the parent function
	ICS_Renderable::_setAnchor(anchor);

	// update the background
	if (_background)
	{
		_background->setAnchor(anchor);
	}
}

/**
 * Sets the dimensions of the renderable.
 * This function is a catch-all for dimension changes.
 * If the renderable's dimensions change, this function will be called.
 *
 * @param dimensions	The dimensions to assign to the renderable.
 */
void
ICS_TextRenderable::_setDimensions(const ICS_Pair<float>& dimensions)
{
	// invoke the parent function
	ICS_Renderable::_setDimensions(dimensions);

	// update the background
	if (_background)
	{
		_background->setDimensions(dimensions);
	}
}