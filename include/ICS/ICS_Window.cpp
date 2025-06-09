#include "ICS_Window.h"		// contains the declaration of ICS_Window
#include "ICS_Game.h"		// contains the declaration of ICS_Game
#include "ICS_Helpers.h"	// contains the definition of ICS_clamp

/**
 * ICS_Window constructor.
 *
 * @param width		The width of the window in pixels.
 * @param height	The height of the window in pixels.
 */
ICS_Window::ICS_Window(float width, float height)
	:
	_scrollEnabled(false),
	_zoomEnabled(false),
	_zoomScale(1.0f),
	_maxZoomScale(1.0f),
	_zoomFactor(1.0f),
	_dragEnabled(false),
	_dragMouseButton(ICS_RIGHT_MOUSE_BUTTON),
	_dragging(false)
{
	// set the position in the top left corner of the window
	setAnchor(0.0f, 0.0f);
	positionChildrenRelativeToOrigin();

	// set the dimensions
	setDimensions(width, height);

	// use the stencil test when rendering
	enableWindowMode();

	// create the renderable to hold the content of the window
	_content = new ICS_Renderable;
	_content->setAnchor(0.0f, 0.0f);
	_content->positionChildrenRelativeToOrigin();

	ICS_Renderable::addChild(_content);
}

/**
 * ICS_Window copy constructor.
 *
 * @param window	The window to copy.
 */
ICS_Window::ICS_Window(const ICS_Window& window)
	:
	ICS_Renderable(window),
	_scrollEnabled(window._scrollEnabled),
	_zoomEnabled(window._zoomEnabled),
	_zoomScale(window._zoomScale),
	_maxZoomScale(window._maxZoomScale),
	_zoomFactor(window._zoomFactor),
	_dragEnabled(window._dragEnabled),
	_dragMouseButton(window._dragMouseButton),
	_dragging(window._dragging),
	_dragContentOffset(window._dragContentOffset),
	_dragMouseStart(window._dragMouseStart)
{
	// create the renderable to hold the content of the window
	_content = new ICS_Renderable(*window._content);
	ICS_Renderable::addChild(_content);
}


/**
 * ICS_Window destructor.
 */
ICS_Window::~ICS_Window()
{
	// stop receiving mouse button events
	ICS_Game::getInstance().removeMouseButtonEventListener(this);

	// delete the content renderable
	delete _content;
}

/**
 * Assignment operator.
 *
 * @param window	The window to copy.
 *
 * @returns			A reference to this instance.
 */
ICS_Window&
ICS_Window::operator=(const ICS_Window& window)
{
	// stop receiving mouse button events
	ICS_Game::getInstance().removeMouseButtonEventListener(this);

	// delete the old content renderable
	delete _content;

	// invoke the parent assignment operators
	ICS_Renderable::operator=(window);
	ICS_EventListener::operator=(window);

	// copy each attribute
	_scrollEnabled = window._scrollEnabled;
	_zoomEnabled = window._zoomEnabled;
	_zoomScale = window._zoomScale;
	_maxZoomScale = window._maxZoomScale;
	_zoomFactor = window._zoomFactor;
	_dragEnabled = window._dragEnabled;
	_dragMouseButton = window._dragMouseButton;
	_dragging = window._dragging;
	_dragContentOffset = window._dragContentOffset;
	_dragMouseStart = window._dragMouseStart;
	
	// create the renderable to hold the content of the window
	_content = new ICS_Renderable(*window._content);
	ICS_Renderable::addChild(_content);

	// return a reference to this instance
	return *this;
}

/**
 * Sets the dimensions of the window's content.
 *
 * @param width		The width to assign to the content (in pixels).
 * @param height	The height to assign to the content (in pixels).
 * @param autoFit	Indicates that the content should be automatically scaled to fill the window.
 */
void
ICS_Window::setContentDimensions(float width, float height, bool autoFit)
{
	// set the dimensions
	_content->setDimensions(width, height);

	// fit the content to the window
	if (autoFit)
	{
		fitContent();
	}
}

/**
 * Sets the position of the window's content relative to the window itself.
 *
 * @param dimension		The component of the position to set (ICS_X or ICS_Y).
 * @param ratio			A value between 0.0f (left / top edge is visible) and 1.0f (right / bottom edge is visible).
 */
void
ICS_Window::setContentPosition(int dimension, float ratio)
{
	// ensire the ratio is between 0 and 1
	ICS_clamp(ratio, 0.0f, 1.0f);

	// calculate the minimum and maximum values for the position
	float min = _content->getAnchor(dimension) * _content->getDimension(dimension) * _content->getScale(dimension);
	float max = getDimension(dimension) - (_content->getDimension(dimension) - _content->getAnchor(dimension) * _content->getDimension(dimension)) * _content->getScale(dimension);
	
	// set the position of the content
	_content->setPositionComponent(dimension, min + (max - min) * ratio);
}

/**
 * Gets the position of the window's content relative to the window itself.
 *
 * @param dimension		The component of the position to get (ICS_X or ICS_Y).
 *
 * @returns				A value between 0.0f (left / top edge is visible) and 1.0f (right / bottom edge is visible).
 */
float
ICS_Window::getContentPosition(int dimension)
{
	// determine the min and max values for the position
	float min = _content->getAnchor(dimension) * _content->getDimension(dimension) * _content->getScale(dimension);
	float max = getDimension(dimension) - (_content->getDimension(dimension) - _content->getAnchor(dimension) * _content->getDimension(dimension)) * _content->getScale(dimension);

	// prevent division by 0
	if (max == min)
	{
		return 0.0f;
	}
	
	// calculate the ratio
	return (_content->getPosition(dimension) - min) / (max - min);
}

/**
 * Gets the percent of the content that is visible in the window (as a decimal).
 *
 * @param dimension		The component of the dimension to get the percent for (ICS_WIDTH or ICS_HEIGHT).
 *
 * @returns				A value between 0.0f (no content is visible) and 1.0f (all content is visible).
 */
float
ICS_Window::getContentPercent(int dimension)
{
	return getDimension(dimension) / (_content->getDimension(dimension) * _content->getScale(dimension));
}


/**
 * Adds a child to the renderable.
 *
 * @param child		The child to add.
 */
void
ICS_Window::addChild(ICS_Renderable* child)
{
	_content->addChild(child);
}

/**
 * Removes a child from the renderable.
 *
 * @param child		The child to remove.
 */
void
ICS_Window::removeChild(ICS_Renderable* child)
{
	_content->removeChild(child);
}

/**
 * Removes all children from the renderable.
 */
void
ICS_Window::removeChildren()
{
	_content->removeChildren();
}

/**
 * Enables vertical scrolling of the content using the mouse wheel. This disables zooming (both cant happen at the same time).
 *
 * @param maxZoomScale		The maximum scale that can be applied to the content by zooming (should be greater than 1.0f).
 * @param zoomFactor		the factor to multiply / divide by when zomming in / out  (should be greater than 1.0f).
 */
void
ICS_Window::enableScrolling()
{
	// disable zooming (both can't be active at the same time)
	_zoomEnabled = false;

	// enable vertical scrolling
	_scrollEnabled = true;
}

/**
 * Disables vertical scrolling of the content using the mouse wheel.
 */
void
ICS_Window::disableScrolling()
{
	// disable scrolling
	_scrollEnabled = false;
}

/**
 * Enables zooming of the content using the mouse wheel.  This disables vertical scrolling (both cant happen at the same time).
 *
 * @param maxZoomScale		The maximum scale that can be applied to the content by zooming (should be greater than 1.0f).
 * @param zoomFactor		the factor to multiply / divide by when zomming in / out  (should be greater than 1.0f).
 */
void
ICS_Window::enableZoom(float maxZoomScale, float zoomFactor)
{
	// disable scrolling (both can't be active at the same time)
	_scrollEnabled = false;

	// enable zooming
	_zoomEnabled = true;
	_maxZoomScale = ICS_max(1.0f, maxZoomScale);
	_zoomFactor = ICS_max(1.0f, zoomFactor);
}

/**
 * Disables zooming of the content using the mouse wheel.
 */
void
ICS_Window::disableZoom()
{
	// disable zooming
	_zoomEnabled = false;
}

/**
 * Indicates whether or not the content can be zoomed in.
 *
 * @returns	true if the content can be zoomed in, false if the max zoom has been reached.
 */
bool
ICS_Window::canZoomIn()
{
	return _zoomEnabled and (_content->getScaleX() < _maxZoomScale) and (_content->getScaleY() < _maxZoomScale);
}

/**
 * Indicates whether or not the content can be zoomed out.
 *
 * @returns	true if the content can be zoomed out, false if the min zoom has been reached.
 */
bool
ICS_Window::canZoomOut()
{
	return _zoomEnabled and ((_content->getWidth() * _content->getScaleX()) > getWidth() or (_content->getHeight() * _content->getScaleY()) > getHeight());
}

/**
 * Zooms in on the content, if possible.
 */
void
ICS_Window::zoomIn()
{
	// zoom in using the coordinates in the center of the window
	zoomIn(getWidth() / 2.0f, getHeight() / 2.0f);
}

/**
 * Zooms out from the content, if possible.
 */
void
ICS_Window::zoomOut()
{
	// zoom out using the coordinates in the center of the window
	zoomOut(getWidth() / 2.0f, getHeight() / 2.0f);
}

/**
 * Enbles dragging of the content using the mouse.
 *
 * @param button		The button to use for dragging (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 */
void
ICS_Window::enableDrag(int button)
{
	// make sure the button is valid
	if (button == ICS_LEFT_MOUSE_BUTTON or button == ICS_RIGHT_MOUSE_BUTTON or button == ICS_MIDDLE_MOUSE_BUTTON)
	{
		_dragMouseButton = button;	// set the button to use for dragging		
		_dragging = false;			// indicate that dragging hasn't started yet
		_dragEnabled = true;		// enable dragging
	}
}

/**
 * Disables dragging of the content.
 */
void
ICS_Window::disableDrag()
{
	// disable dragging
	_dragEnabled = false;
}

/**
 * Removes all content and disables zooming, scrolling and dragging.
 */
void
ICS_Window::reset()
{
	// disable all manipulation
	disableScrolling();
	disableZoom();
	disableDrag();

	// remove all children from the content
	_content->removeChildren();

	// fit the content to the window
	_content->setDimensions(getDimensions());
	_content->setScale(1.0f);
	fitContent();
}

/**
 * This handles mouse movement over the renderable events.
 *
 * @param x		The x position of the mouse cursor.
 * @param y		The y position of the mouse cursor.
 */
void
ICS_Window::handleMouseMoveOver(float x, float y)
{
	// invoke the parent function
	ICS_Renderable::handleMouseMoveOver(x, y);

	// check if dragging is enabled
	if (_dragging)
	{
		// transform the mouse coordinates into local space
		inverseTransform(x, y);

		// determine how far the mouse has moved from its original position
		ICS_Pair<float> change = ICS_Pair<float>(x, y) - _dragMouseStart;

		// move the content to match the mouse change
		_content->setPosition(_dragContentOffset + change);

		// make sure the content still fits in the window
		fitContent();

		// notify the parent that the content has been modified
		triggerChildEvent(ICS_EVENT_CHANGE);
	}
}

/**
 * This handles mouse wheel events that occur when the cursor is over the renderable.
 *
 * @param x					The x position of the mouse cursor.
 * @param y					The y position of the mouse cursor.
 * @param wheelRotation		The wheel rotation value.  A positive value indicates that the wheel was rotated forward, away
 *							from the user; a negative value indicates that the wheel was rotated backward, toward the user.
 */
void
ICS_Window::handleMouseWheelOver(float x, float y, int wheelRotation)
{
	// invoke the parent function
	ICS_Renderable::handleMouseWheelOver(x, y, wheelRotation);

	// transform the mouse coordinates into local space
	inverseTransform(x, y);

	// make sure vertical scrolling is enabled and the contents has a greater height than the window
	if (_scrollEnabled and _content->getHeight() > getHeight())
	{
		// calculate the new y position within the constraints of the window
		float y = _content->getY() + wheelRotation / 12.0f;
		ICS_clamp(y, (float)(getHeight() - _content->getHeight()), 0.0f);
		_content->setY(y);

		// notify the parent that the content has been modified
		triggerChildEvent(ICS_EVENT_CHANGE);

		// invoke the "handleMouseMoveOver" function children will react to the cursor position
		ICS_Renderable::handleMouseMoveOver(x, y);
	}

	// zoom in?
	if (_zoomEnabled and wheelRotation > 0 and canZoomIn())
	{
		zoomIn(x, y);								// zoom in using the mouse coordinates
		triggerChildEvent(ICS_EVENT_CHANGE);		// notify the parent that the content has been modified
		ICS_Renderable::handleMouseMoveOver(x, y);	// invoke the "handleMouseMoveOver" function children will react to the cursor position
	}

	// zoom out?
	if (_zoomEnabled and wheelRotation < 0 and canZoomOut())
	{
		zoomOut(x, y);								// zoom out using the mouse coordinates
		triggerChildEvent(ICS_EVENT_CHANGE);		// notify the parent that the content has been modified
		ICS_Renderable::handleMouseMoveOver(x, y);	// invoke the "handleMouseMoveOver" function children will react to the cursor position
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
ICS_Window::handleMouseButtonEvent(int button, float x, float y, int eventType)
{
	// drag button released? done dragging
	if (button == _dragMouseButton and eventType == ICS_EVENT_RELEASE)
	{
		// stop dragging
		_dragging = false;

		// stop receiving mouse button events
		ICS_Game::getInstance().removeMouseButtonEventListener(this);
	}
}

/**
 * This is called when a mouse button is pressed over the renderable.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 * @param x			The x position of the mouse cursor.
 * @param y			The y position of the mouse cursor.
 */
void
ICS_Window::handleMousePressOver(int button, float x, float y)
{
	// invoke the parent function
	ICS_Renderable::handleMousePressOver(button, x, y);

	// if a drag mouse button triggered the event, start dragging
	if (_dragEnabled and button == _dragMouseButton)
	{
		// transform the mouse coordinates into local space
		inverseTransform(x, y);

		_dragging = true;											// start dragging
		_dragContentOffset = _content->getPosition();				// keep track of where the content was when dragging started
		_dragMouseStart = ICS_Pair<float>(x, y);					// keep track of where the mouse was when dragging started
		ICS_Game::getInstance().addMouseButtonEventListener(this);	// start receiving mouse button events
	}
}

/**
 * Sets the anchor point of the content to the specified window coordinates (transformed into the content's local space).
 *
 * @param x		The x position, in window space.
 * @param y		The y position, in window space.
 */
void
ICS_Window::setContentAnchorPoint(float x, float y)
{
	// copy the coordinates before they are transformed
	float positionX = x;
	float positionY = y;

	// transform the coordinates into the content's local space
	_content->inverseTransform(x, y);
	
	// set the content's position using the original coordinates
	_content->setPosition(positionX, positionY);

	// set the content's anchor point to the coordinates in local space
	_content->setAnchor(x / _content->getWidth(), y / _content->getHeight());
}

/**
 * Zooms in on the content, if possible.  Zooming will be centered around the specified window coordinates.
 *
 * @param x		The x position, in window space.
 * @param y		The y position, in window space.
 */
void
ICS_Window::zoomIn(float x, float y)
{
	// make sure that it is possible to zoom in
	if (canZoomIn())
	{
		// center the content around the specified coordinates
		setContentAnchorPoint(x, y);

		// increase the zoom level
		_zoomScale *= _zoomFactor;

		// set the content's scale to the zoom level
		_content->setScale(_zoomScale);

		// make sure the content still fits in the window
		fitContent();
	}
}

/**
 * Zooms out from the content, if possible.  Zooming will be centered around the specified window coordinates.
 *
 * @param x		The x position, in window space.
 * @param y		The y position, in window space.
 */
void
ICS_Window::zoomOut(float x, float y)
{
	// make sure that it is possible to zoom out
	if (canZoomOut())
	{
		// center the content around the specified coordinates
		setContentAnchorPoint(x, y);

		// decrease the zoom level
		_zoomScale /= _zoomFactor;

		// set the content's scale to the zoom level
		_content->setScale(_zoomScale);

		// make sure the content still fits in the window
		fitContent();
	}
}

/**
 * Auto-fits the content to the window by limiting the scale and position.
 */
void
ICS_Window::fitContent()
{
	// determine the minimum zoom level in each dimension to ensure the content fills the window
	float minScaleX = (float)getWidth() / _content->getWidth();
	float minScaleY = (float)getHeight() / _content->getHeight();

	// if the content is smaller than the window, increase the scale
	if (_content->getScaleX() < minScaleX and _content->getScaleY() < minScaleY)
	{
		_zoomScale = ICS_min(minScaleX, minScaleY);
		_content->setScale(_zoomScale);
	}

	// determine the minimum coordinates to center the content in the window
	float centerX = ICS_max(0.0f, (getWidth() - _content->getWidth() * _content->getScaleX()) / 2.0f);
	float centerY = ICS_max(0.0f, (getHeight() - _content->getHeight() * _content->getScaleY()) / 2.0f);

	// determine the max / min position values for the content in each direction
	float left = centerX + _content->getAnchorX() * _content->getWidth() * _content->getScaleX();
	float right = getWidth() - centerX - (_content->getWidth() - _content->getAnchorX() * _content->getWidth()) * _content->getScaleX();
	float top = centerY + _content->getAnchorY() * _content->getHeight() * _content->getScaleY();
	float bottom = getHeight() - centerY - (_content->getHeight() - _content->getAnchorY() * _content->getHeight()) * _content->getScaleY();

	// make sure the content isn't too far left
	if (_content->getX() < right)
	{
		_content->setX(right);
	}

	// make sure the content isn't too far up
	if (_content->getY() < bottom)
	{
		_content->setY(bottom);
	}

	// make sure the content isn't too far right
	if (_content->getX() > left)
	{
		_content->setX(left);
	}

	// make sure the content isn't too far down
	if (_content->getY() > top)
	{
		_content->setY(top);
	}
}