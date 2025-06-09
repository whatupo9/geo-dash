#include "ICS_Game.h"	// contains the declaration of ICS_Game
#include "ICS_Slider.h"	// contains the declaration of ICS_Slider
#include "ICS_Sprite.h"	// contains the declaration of ICS_Sprite
#include "ICS_Button.h"	// contains the declaration of ICS_Button

/**
 * The constructor.
 *
 * @param type			The type of slider (ICS_HORIZONTAL or ICS_VERTICAL).
 * @param thumbSize		The size of the thumb sprite (in pixels).
 * @param trackLength	The length of the track (in pixels).
 */
ICS_Slider::ICS_Slider(int type, float thumbSize, float trackLength)
	:
	_type(type),
	_trackSprite(NULL),
	_thumbSprite(NULL),
	_thumbScale(trackLength != 0.0f ? thumbSize / trackLength : 0.0f),
	_sliding(false),
	_mouseOffset(0.0f),
	_sliderValue(0.0f)
{
	// set the anchor to the top left corner
	setAnchor(0.0f, 0.0f);

	// create the thumb sprite
	_thumbSprite = new ICS_Button(thumbSize, thumbSize);
	_thumbSprite->setPriority(1);
	addChild(_thumbSprite);

	// center the thumb and create the track
	if (_type == ICS_X)
	{
		_thumbSprite->setAnchor(0.5f, 0.0f);
		_trackSprite = new ICS_Sprite(ICS_Color(ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX), trackLength, thumbSize);
	}
	else
	{
		_thumbSprite->setAnchor(0.0f, 0.5f);
		_trackSprite = new ICS_Sprite(ICS_Color(ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX), thumbSize, trackLength);
	}

	_trackSprite->setAnchor(0.0f, 0.0f);
	addChild(_trackSprite);

	// set the dimensions
	setDimensions(thumbSize, thumbSize);
	setDimension(_type, trackLength);
}

/**
 * The copy constructor.
 *
 * @param slider	The slider to copy.
 */
ICS_Slider::ICS_Slider(const ICS_Slider& slider)
	:
	ICS_Renderable(slider),
	ICS_EventListener(slider),
	_type(slider._type),
	_trackSprite(new ICS_Sprite(*slider._trackSprite)),
	_thumbSprite(new ICS_Button(*slider._thumbSprite)),
	_thumbScale(slider._thumbScale),
	_sliding(slider._sliding),
	_mouseOffset(slider._mouseOffset),
	_sliderValue(slider._sliderValue)
{
	// add children
	addChild(_thumbSprite);
	addChild(_trackSprite);
}

/**
 * The destructor.
 */
ICS_Slider::~ICS_Slider()
{
	// stop receiving mouse events
	ICS_Game::getInstance().removeMouseMoveEventListener(this);
	ICS_Game::getInstance().removeMouseButtonEventListener(this);

	// delete the thumb sprite
	delete _thumbSprite;

	// delete the track sprite
	delete _trackSprite;
}

/**
 * Assignment operator.
 *
 * @param slider	The slider to copy.
 *
 * @returns			A reference to this instance.
 */
ICS_Slider&
ICS_Slider::operator=(const ICS_Slider& slider)
{
	// stop receiving mouse events
	ICS_Game::getInstance().removeMouseMoveEventListener(this);
	ICS_Game::getInstance().removeMouseButtonEventListener(this);

	// delete the thumb sprite
	delete _thumbSprite;

	// delete the track sprite
	delete _trackSprite;

	// invoke the parent assignment operators
	ICS_Renderable::operator=(slider);
	ICS_EventListener::operator=(slider);

	// copy each attribute
	_type = slider._type;
	_trackSprite = new ICS_Sprite(*slider._trackSprite);
	_thumbSprite = new ICS_Button(*slider._thumbSprite);
	_thumbScale = slider._thumbScale;
	_sliding = slider._sliding;
	_mouseOffset = slider._mouseOffset;
	_sliderValue = slider._sliderValue;

	// add children
	addChild(_thumbSprite);
	addChild(_trackSprite);

	// return a reference to this instance
	return *this;
}

/**
 * Sets the slider value.
 *
 * @param value		The value to assign to the slider (0 is the min and 1 is the max).
 */
void
ICS_Slider::setSliderValue(float value)
{
	// make sure the value is between 0 and 1
	ICS_clamp(value, 0.0f, 1.0f);

	// assign the member attribute
	_sliderValue = value;

	// update the value
	updateThumb();
}

/**
 * Sets the color of the track.
 *
 * @param color		The color to render the track.
 */
void
ICS_Slider::setTrackColor(const ICS_Color& color)
{
	_trackSprite->setColor(color);
}

/**
 * Sets the amount to scale the thumb relative to the track.
 *
 * @param value		The amount to scale the thumb by (between 0.0 and 1.0f).
 */
void
ICS_Slider::setThumbScale(float value)
{
	// clamp the value
	ICS_clamp(value, 0.0f, 1.0f);

	// set the value
	_thumbScale = value;

	// set the sprite's position based on it's size and the slider value
	updateThumb();
}

/**
 * Sets the color of each state of the thumb.
 *
 * @param upColor		The color to render the thumb when it is not being interacted with.
 * @param downColor		The color to render the thumb when it is being held.
 * @param hoverColor	The color to render the thumb when the mouse cursor is over it.
 */
void
ICS_Slider::setThumbColor(const ICS_Color& upColor, const ICS_Color& downColor, const ICS_Color& hoverColor)
{
	_thumbSprite->setColor(upColor, downColor, hoverColor, upColor);
}

/**
 * This handles mouse movement events.
 *
 * @param mouseX	The x position of the mouse cursor.
 * @param mouseY	The y position of the mouse cursor.
 */
void
ICS_Slider::handleMouseMove(float mouseX, float mouseY)
{
	// if the slider is sliding, update the sprite position and slider value
	if (_sliding)
	{
		// transform the coordinates into local space
		globalToLocal(mouseX, mouseY);

		// update the thumb
		handleMouseEvent(ICS_Pair<float>(mouseX, mouseY));
	}
}

/**
 * This handles mouse wheel events that occur when the cursor is over the renderable.
 *
 * @param mouseX			The x position of the mouse cursor.
 * @param mouseY			The y position of the mouse cursor.
 * @param wheelRotation		The wheel rotation value.  A positive value indicates that the wheel was rotated forward, away
 *							from the user; a negative value indicates that the wheel was rotated backward, toward the user.
 */
void
ICS_Slider::handleMouseWheelOver(float mouseX, float mouseY, int wheelRotation)
{
	// invoke the parent function
	ICS_Renderable::handleMouseWheelOver(mouseX, mouseY, wheelRotation);

	// ignore this event if the slider is being held down
	if (_thumbScale < 1.0f and not _sliding)
	{
		// keep track of the old slider value to make sure the value changed before triggering an event
		float oldSliderValue = _sliderValue;

		// update the slider value
		if (_type == ICS_HORIZONTAL)
		{
			_sliderValue += (wheelRotation / 960.0f) * _thumbScale;
		}
		else
		{
			_sliderValue -= (wheelRotation / 960.0f) * _thumbScale;
		}

		// keep the slider between the min and max
		ICS_clamp(_sliderValue, 0.0f, 1.0f);

		// make sure the slider value changed before triggering an event
		if (_sliderValue != oldSliderValue)
		{
			// calculate the min and max pixel values for the thumb
			float range = (float)(getDimension(_type) - _thumbSprite->getDimension(_type));
			float min = _thumbSprite->getDimension(_type) / 2.0f;
			float max = min + range;

			// update the thumb position
			_thumbSprite->setPositionComponent(_type, min + _sliderValue * range);

			// notify the owner
			triggerChildEvent(ICS_EVENT_CHANGE);

			// invoke the "handleMouseMoveOver" function so the thumb will animate correctly relative to the cursor position
			ICS_Renderable::handleMouseMoveOver(mouseX, mouseY);
		}
	}
}

/**
 * This is called when a mouse button is pressed or released.
 *
 * @param button		The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 * @param x				The x component of the mouse cursor position.
 * @param y				The y component of the mouse cursor position.
 * @param eventType		The type of event (ICS_EVENT_PRESS or ICS_EVENT_RELEASE)
 */
void
ICS_Slider::handleMouseButtonEvent(int button, float x, float y, int eventType)
{
	// left button released? done sliding
	if (button == ICS_LEFT_MOUSE_BUTTON and eventType == ICS_EVENT_RELEASE)
	{
		// stop sliding
		_sliding = false;

		// stop receiving mouse events
		ICS_Game::getInstance().removeMouseMoveEventListener(this);
		ICS_Game::getInstance().removeMouseButtonEventListener(this);
	}
}

/**
 * This is called when a mouse button is pressed over the slider.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 * @param mouseX	The x position of the mouse cursor.
 * @param mouseY	The y position of the mouse cursor.
 */
void
ICS_Slider::handleMousePressOver(int button, float mouseX, float mouseY)
{
	// invoke the parent function
	ICS_Renderable::handleMousePressOver(button, mouseX, mouseY);

	// only react to left mouse button events
	if (button == ICS_LEFT_MOUSE_BUTTON)
	{
		// transform the mouse coordinates into local space
		inverseTransform(mouseX, mouseY);

		// update the sprite position and slider value
		_sliding = true;

		// start receiving mouse events
		ICS_Game::getInstance().addMouseMoveEventListener(this);
		ICS_Game::getInstance().addMouseButtonEventListener(this);

		// center the thumb over the mouse coordinates
		_mouseOffset = 0.0f;

		// if the mouse is on the thumb, calculate the offset between the center of the thumb and the mouse coordinates
		if (_thumbSprite->isMouseOver(mouseX, mouseY, false))
		{
			ICS_Pair<float> mouseCoordinates(mouseX, mouseY);
			_mouseOffset = mouseCoordinates[_type] - _thumbSprite->getPosition(_type);
			ICS_clamp(_mouseOffset, -_thumbSprite->getDimension(_type) / 2.0f, _thumbSprite->getDimension(_type) / 2.0f);
		}

		// begin sliding
		handleMouseEvent(ICS_Pair<float>(mouseX, mouseY));
	}
}

/**
 * Sets the dimensions of the slider.
 * This function is a catch-all for dimension changes.
 * If the slider's dimensions change, this function will be called.
 *
 * @param dimensions	The dimensions to assign to the slider.
 */
void
ICS_Slider::_setDimensions(const ICS_Pair<float>& dimensions)
{
	// set the dimensions
	ICS_Renderable::_setDimensions(dimensions);

	// set the dimensions of the track sprite
	if (_trackSprite)
	{
		_trackSprite->setDimensions(dimensions);
	}

	// set the dimensions of the slider sprite
	updateThumb();
}

/**
 * Updates the slider value and sprite position based on the mouse coordinates.
 *
 * @param coordinates	The current coordinates of the mouse.
 */
void
ICS_Slider::handleMouseEvent(ICS_Pair<float> coordinates)
{
	// adjust the coordinates using the mouse offest so the slider will not jump when clicked on
	coordinates[_type] -= _mouseOffset;

	// calculate the min and max pixel values for the slider
	float range = (float)(getDimension(_type) - _thumbSprite->getDimension(_type));
	float min = _thumbSprite->getDimension(_type) / 2.0f;
	float max = min + range;

	// keep the slider between the min and max
	ICS_clamp(coordinates[_type], min, max);

	_thumbSprite->setPositionComponent(_type, coordinates[_type]);

	// calculate the slider value
	_sliderValue = 0.0f;

	if (range > 0)
	{
		_sliderValue = (float)(coordinates[_type] - min) / range;
	}

	// notify the owner
	triggerChildEvent(ICS_EVENT_CHANGE);
}

/**
 * Updates the thumb sprite's scale and position.
 */
void
ICS_Slider::updateThumb()
{
	// set the thumb sprite size
	_thumbSprite->setDimension(_type, getDimension(_type) * _thumbScale);

	// if the thumb fills the track, don't show it
	_thumbSprite->setVisible(_thumbScale < 1.0f);

	// calcuate the range and min pixel position
	float range = getDimension(_type) - _thumbSprite->getDimension(_type);
	float min = _thumbSprite->getDimension(_type) / 2;

	// set the sprite's position based on the slider value
	_thumbSprite->setPositionComponent(_type, min + range * _sliderValue);
}
