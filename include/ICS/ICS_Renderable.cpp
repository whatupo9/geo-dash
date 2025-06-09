#include "ICS_Renderable.h"	// the definition of ICS_Renderable
#include "ICS_Game.h"		// the definition of ICS_Game

#include <algorithm>		// for sorting
#include <glut.h>			// the library for glut (OpenGL)

/**
 * The ICS_Renderable constructor for the ICS_Game root node.
 *
 * @param rootNode		Indicates that the renderable is the root node for the game.
 */
ICS_Renderable::ICS_Renderable(bool rootNode)
	:
	_visible(true),
	_ignoreMouse(false),
	_color(ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX),
	_windowMode(false),
	_anchor(0.5f, 0.5f),
	_position(0.0f, 0.0f),
	_dimensions(0, 0),
	_scale(1.0f, 1.0f),
	_rotation(0.0f),
	_childrenRelativeToAnchor(true),
	_parent(NULL),
	_priority(0),
	_mouseOverChild(NULL),
	_eventCallback(NULL),
	_renderableEventCallback(NULL)
{
	// initialize the pointers for handling mouse clicks
	_mouseDownChild[ICS_LEFT_MOUSE_BUTTON] = NULL;
	_mouseDownChild[ICS_RIGHT_MOUSE_BUTTON] = NULL;
	_mouseDownChild[ICS_MIDDLE_MOUSE_BUTTON] = NULL;
}

/**
 * The ICS_Renderable constructor.
 */
ICS_Renderable::ICS_Renderable()
	:
	_visible(true),
	_ignoreMouse(false),
	_color(ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX, ICS_COLOR_MAX),
	_windowMode(false),
	_anchor(0.5f, 0.5f),
	_position(0.0f, 0.0f),
	_dimensions(0, 0),
	_scale(1.0f, 1.0f),
	_rotation(0.0f),
	_childrenRelativeToAnchor(true),
	_parent(NULL),
	_priority(0),
	_mouseOverChild(NULL),
	_eventCallback(NULL),
	_renderableEventCallback(NULL)
{
	// initialize the pointers for handling mouse clicks
	_mouseDownChild[ICS_LEFT_MOUSE_BUTTON] = NULL;
	_mouseDownChild[ICS_RIGHT_MOUSE_BUTTON] = NULL;
	_mouseDownChild[ICS_MIDDLE_MOUSE_BUTTON] = NULL;

	// add the renderable to the root node
	ICS_Game::getInstance().add(this);
}

/**
 * The copy constructor.  Does not copy parents or children!  The renderable is added to the root node after construction.
 *
 * @param renderable	The instance to copy.
 */
ICS_Renderable::ICS_Renderable(const ICS_Renderable& renderable)
	:
	_visible(renderable._visible),
	_ignoreMouse(renderable._ignoreMouse),
	_color(renderable._color),
	_windowMode(renderable._windowMode),
	_anchor(renderable._anchor),
	_position(renderable._position),
	_dimensions(renderable._dimensions),
	_scale(renderable._scale),
	_rotation(renderable._rotation),
	_inverseTransform(renderable._inverseTransform),
	_anchorInverseTransform(renderable._anchorInverseTransform),
	_childrenRelativeToAnchor(renderable._childrenRelativeToAnchor),
	_parent(NULL),
	_priority(renderable._priority),
	_mouseOverChild(NULL),
	_eventCallback(renderable._eventCallback),
	_renderableEventCallback(renderable._renderableEventCallback)
{
	// initialize the pointers for handling mouse clicks
	_mouseDownChild[ICS_LEFT_MOUSE_BUTTON] = NULL;
	_mouseDownChild[ICS_RIGHT_MOUSE_BUTTON] = NULL;
	_mouseDownChild[ICS_MIDDLE_MOUSE_BUTTON] = NULL;

	// add the renderable to the root node
	ICS_Game::getInstance().add(this);
}

/**
 * The ICS_Renderable destructor.
 */
ICS_Renderable::~ICS_Renderable()
{
	// remove the parent if it has one
	if (_parent)
	{
		_parent->removeChild(this);
	}

	// the children are now orphans
	for (unsigned int i = 0; i < _children.size(); i++)
	{
		_children[i]->_parent = NULL;
	}
}

/**
 * The assignment operator.  Does not copy parents or children!  The renderable is added to the root node after construction.
 *
 * @param renderable	The renderable to copy.
 * 
 * @returns				A reference to this instance.
 */
ICS_Renderable&
ICS_Renderable::operator=(const ICS_Renderable& renderable)
{
	// copy the attributes
	_visible = true;
	_ignoreMouse = renderable._ignoreMouse;
	_color = renderable._color;
	_windowMode = renderable._windowMode;
	_anchor = renderable._anchor;
	_position = renderable._position;
	_dimensions = renderable._dimensions;
	_scale = renderable._scale;
	_rotation = renderable._rotation;
	_inverseTransform = renderable._inverseTransform;
	_anchorInverseTransform = renderable._anchorInverseTransform;
	_childrenRelativeToAnchor = renderable._childrenRelativeToAnchor;
	_priority = renderable._priority;
	_mouseOverChild = NULL;
	_eventCallback = renderable._eventCallback;
	_renderableEventCallback = renderable._renderableEventCallback;

	// initialize the pointers for handling mouse clicks
	_mouseDownChild[ICS_LEFT_MOUSE_BUTTON] = NULL;
	_mouseDownChild[ICS_RIGHT_MOUSE_BUTTON] = NULL;
	_mouseDownChild[ICS_MIDDLE_MOUSE_BUTTON] = NULL;

	// add the renderable to the root node
	ICS_Game::getInstance().add(this);

	// return a reference to the renderable
	return *this;
}

/**
 * Checks if the coordinates are over the renderable or not.
 *
 * @param x			The x component of the coordinates.
 * @param y			The y component of the coordinates.
 * @param global	Indicates that the coordinates are in global space (not parent space).
 *
 * @returns			true if the coordinates are over the renderable, false if not.
 */
bool
ICS_Renderable::isMouseOver(float x, float y, bool global) const
{
	// if the renderable is not visible or the mouse is being ignored, ignore the event
	if (not _visible or _ignoreMouse)
	{
		return false;
	}

	// transform the mouse coordinates into local space
	if (global)
	{
		globalToLocal(x, y);
	}
	else
	{
		inverseTransform(x, y);
	}

	// check if the coordinates are over a child
	bool overChild = isMouseOverChild(x, y);

	// apply the anchor transformation after checking mouse over children
	anchorInverseTransform(x, y);

	// check if the mouse is over the renderable
	int overThis = (x >= 0 and x < _dimensions[ICS_WIDTH] and y >= 0 and y <= _dimensions[ICS_HEIGHT]);

	// returns true if the mouse is over the renderable or over a visible child
	return (_windowMode and overThis) or (not _windowMode and (overThis or overChild));
}

/**
 * Sets the priority of the renderable.
 * Renderables with lower priority are rendered first, behind renderables with higher priority.
 *
 * @param priority	The value to assign to the renderable's priority.
 */
void
ICS_Renderable::setPriority(int priority)
{
	// make sure the priority changed
	if (_priority != priority)
	{
		// change the priority
		_priority = priority;

		// let the parent know the priority changed so the child will be placed in the correct order for rendering
		if (_parent)
		{
			_parent->reinsertChild(this);
		}
	}
}

/**
 * Sets the rotation of the renderable.
 *
 * @param rotation		The amount to rotate the sprite around its center (in degrees).
 */
void
ICS_Renderable::setRotation(float rotation)
{
	// keep the rotation value between 0 and 360
	while (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	while (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	// set the rotation
	_rotation = rotation;

	// calculate the inverse transformation matrix
	calculateInverseTransformation();
}

/**
 * This sets the color of the renderable.
 * Each component of the color should be between ICS_COLOR_MIN and ICS_COLOR_MAX
 *
 * @param red		The red component of the renderable's color.
 * @param green		The green component of the renderable's color.
 * @param blue		The blue component of the renderable's color.
 * @param alpha		The alpha transparency component of the renderable's color.
 */
void
ICS_Renderable::setColor(int red, int green, int blue, int alpha)
{
	// clamp the input values
	ICS_clamp(red, ICS_COLOR_MIN, ICS_COLOR_MAX);
	ICS_clamp(green, ICS_COLOR_MIN, ICS_COLOR_MAX);
	ICS_clamp(blue, ICS_COLOR_MIN, ICS_COLOR_MAX);
	ICS_clamp(alpha, ICS_COLOR_MIN, ICS_COLOR_MAX);

	// set the member attributes
	_color.red = red;
	_color.green = green;
	_color.blue = blue;
	_color.alpha = alpha;
}

/**
 * This sets the color of the renderable.
 *
 * @param color		The red, green, blue and alpha components of the color to assign to the renderable.
 */
void
ICS_Renderable::setColor(const ICS_Color& color)
{
	setColor(color.red, color.green, color.blue, color.alpha);
}

/**
 * This sets alpha transparency of the renderable.
 * The value should be between ICS_COLOR_MIN and ICS_COLOR_MAX
 *
 * @param alpha		The alpha transparency component of the renderable's color.
 */
void
ICS_Renderable::setAlpha(int alpha)
{
	// clamp the input value
	ICS_clamp(alpha, ICS_COLOR_MIN, ICS_COLOR_MAX);

	// set the member attribute
	_color.alpha = alpha;
}

/**
 * Adds a child to the renderable.
 *
 * @param child		The child to add.
 */
void
ICS_Renderable::addChild(ICS_Renderable* child)
{
	// only add the child if it isn't already added
	if (std::find(_children.begin(), _children.end(), child) == _children.end())
	{
		// remove the child from its parent if it already has one
		if (child->_parent)
		{
			child->_parent->removeChild(child);
		}

		// set the parent
		child->_parent = this;

		// insert the child in the correct rendering order
		insertChild(child);
	}
}

/**
 * Removes a child from the renderable.
 *
 * @param child		The child to remove.
 */
void
ICS_Renderable::removeChild(ICS_Renderable* child)
{
	// find the child in the set
	std::vector<ICS_Renderable*>::iterator it = std::find(_children.begin(), _children.end(), child);

	// if the child is found...
	if (it != _children.end())
	{
		// the child is now an orphan
		(*it)->_parent = NULL;

		// make sure it's not a "mouse" child
		if (*it == _mouseOverChild)
		{
			_mouseOverChild = NULL;
		}

		for (int i = 0; i < ICS_MOUSE_BUTTONS; i++)
		{
			if (*it == _mouseDownChild[i])
			{
				_mouseDownChild[i] = NULL;
			}
		}

		// remove the child from the set
		_children.erase(it);
	}
}

/**
 * Removes all children from the renderable.
 */
void
ICS_Renderable::removeChildren()
{
	// make each child an orphan
	for (unsigned int i = 0; i < _children.size(); i++)
	{
		_children[i]->_parent = NULL;
	}

	// no more children
	_children.clear();
}

/**
 * This renders 2D game elements.
 */
void
ICS_Renderable::onRender2D()
{
	// check if OpenGL is not initialized or the renderable is invisible
	if (not ICS_Game::getInstance().isInitialized() or not _visible)
	{
		return;
	}

	// push a matrix to preserve the current transformation
	glPushMatrix();

	// apply the transformation matrix
	glTranslatef(_position[ICS_X], _position[ICS_Y], 0);	// position
	glRotatef(_rotation, 0, 0, 1);							// rotation
	glScalef(_scale[ICS_X], _scale[ICS_Y], 0);				// scaling

	// use stencil test?
	if (_windowMode)
	{
		enableStencilTest(-_anchor[ICS_X] * _dimensions[ICS_X], (1 - _anchor[ICS_X]) * _dimensions[ICS_WIDTH], -_anchor[ICS_Y] * _dimensions[ICS_Y], (1 - _anchor[ICS_Y]) * _dimensions[ICS_HEIGHT]);
	}

	// adjust for the anchor point (if the anchor is applied to children)
	if (not _childrenRelativeToAnchor)
	{
		glTranslatef(-_anchor[ICS_X] * _dimensions[ICS_X], -_anchor[ICS_Y] * _dimensions[ICS_Y], 0);
	}

	// render them in reverse order (higher priority renders on top)
	for (int i = _children.size() - 1; i >= 0; i--)
	{
		_children[i]->onRender2D();
	}

	// adjust for the anchor point (if the anchor is not applied to children)
	if (_childrenRelativeToAnchor)
	{
		glTranslatef(-_anchor[ICS_X] * _dimensions[ICS_X], -_anchor[ICS_Y] * _dimensions[ICS_Y], 0);
	}

	// render
	render();

	// disable stencil test?
	if (_windowMode)
	{
		disableStencilTest();
	}

	// restore the previous transformation matrix
	glPopMatrix();
}

/**
 * This handles mouse movement events.
 *
 * @param x		The x position of the mouse cursor.
 * @param y		The y position of the mouse cursor.
 */
void
ICS_Renderable::handleMouseMoveOver(float x, float y)
{
	// transform the mouse coordinates into local space
	_inverseTransform.transform(x, y);

	// only trigger this event for one child
	bool eventConsumed = false;

	// notify each child
	for (unsigned int i = 0; not eventConsumed and i < _children.size(); i++)
	{
		// if the mouse is over this child...
		if (_children[i]->isMouseOver(x, y, false))
		{
			// trigger the event
			_children[i]->handleMouseMoveOver(x, y);
			eventConsumed = true;
		}
	}

	// check for mouse enter and mouse leave events for children
	updateMouseOverChild(x, y);
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
ICS_Renderable::handleMouseWheelOver(float x, float y, int wheelRotation)
{
	// transform the mouse coordinates into local space
	_inverseTransform.transform(x, y);

	// only trigger this event for one child
	bool eventConsumed = false;

	// go through each child to determine if the mouse is over one
	for (unsigned int i = 0; not eventConsumed and i < _children.size(); i++)
	{
		// if the mouse is over this child...
		if (_children[i]->isMouseOver(x, y, false))
		{
			// trigger the event
			_children[i]->handleMouseWheelOver(x, y, wheelRotation);
			eventConsumed = true;
		}
	}
}

/**
 * This is called when the mouse moves over the renderable.
 *
 * @param x		The x position of the mouse cursor.
 * @param y		The y position of the mouse cursor.
 */
void
ICS_Renderable::handleMouseEnter(float x, float y)
{
	// transform the mouse coordinates into local space
	_inverseTransform.transform(x, y);

	// check for mouse enter and mouse leave events for children
	updateMouseOverChild(x, y);
}

/**
 * This is called when the mouse moves off the renderable.
 */
void
ICS_Renderable::handleMouseLeave()
{
	// generate a mouse leave event if the mouse was over a child
	if (_mouseOverChild)
	{
		_mouseOverChild->handleMouseLeave();
		_mouseOverChild = NULL;
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
ICS_Renderable::handleMousePressOver(int button, float x, float y)
{
	// transform the mouse coordinates into local space
	_inverseTransform.transform(x, y);

	// find the child that the mouse is over
	_mouseDownChild[button] = NULL;

	// go through each child
	for (unsigned int i = 0; i < _children.size() and not _mouseDownChild[button]; i++)
	{
		// if the mouse is over this child, trigger an event
		if (_children[i]->isMouseOver(x, y, false))
		{
			// keep track of the child that is "down"
			_mouseDownChild[button] = _children[i];

			// trigger the event
			_mouseDownChild[button]->handleMousePressOver(button, x, y);
		}
	}

	// go through each child
	for (unsigned int i = 0; i < _children.size(); i++)
	{
		// if the mouse is NOT over this child, trigger an event
		if (_children[i] != _mouseDownChild[button])
		{
			_children[i]->handleMousePressNotOver(button);
		}
	}
}

/**
 * This is called when a mouse button is pressed somewhere NOT over the renderable.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 */
void
ICS_Renderable::handleMousePressNotOver(int button)
{
	// notify each child
	for (unsigned int i = 0; i < _children.size(); i++)
	{
		_children[i]->handleMousePressNotOver(button);
	}
}

/**
 * This is called when a mouse button is pressed over the renderable but released off the renderable.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 */
void
ICS_Renderable::handleMouseReleaseNotOver(int button)
{
	// if a child is down, notify it
	if (_mouseDownChild[button])
	{
		_mouseDownChild[button]->handleMouseReleaseNotOver(button);
		_mouseDownChild[button] = NULL;
	}
}

/**
 * This is called when a mouse button is pressed and released over the renderable.
 *
 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
 * @param x			The x position of the mouse cursor.
 * @param y			The y position of the mouse cursor.
 */
void
ICS_Renderable::handleMouseClick(int button, float x, float y)
{
	// transform the mouse coordinates into local space
	_inverseTransform.transform(x, y);

	// this only needs to be handled if a child is down 
	if (_mouseDownChild[button])
	{
		// find the event handler that the mouse is over
		ICS_Renderable* child = NULL;

		// go through each event handler to find which one the mouse is over
		for (unsigned int i = 0; i < _children.size() and !child; i++)
		{
			if (_children[i]->isMouseOver(x, y, false))
			{
				child = _children[i];
			}
		}

		// if the mouse is over the event handler that was down, trigger a mouse click event
		if (child == _mouseDownChild[button])
		{
			_mouseDownChild[button]->handleMouseClick(button, x, y);
		}

		// if the mouse is not over the event handler that was down, trigger a mouse up event
		else
		{
			_mouseDownChild[button]->handleMouseReleaseNotOver(button);
		}
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
ICS_Renderable::_setAnchor(ICS_Pair<float> anchor)
{
	// validate input
	ICS_clamp(anchor[ICS_X], 0.0f, 1.0f);
	ICS_clamp(anchor[ICS_Y], 0.0f, 1.0f);

	// set the anchor
	_anchor = anchor;

	// calculate the inverse transformation matrix
	calculateInverseTransformation();
}

/**
 * Sets the position of the renderable.
 * This function is a catch-all for position changes.
 * If the renderable's position changes, this function will be called.
 *
 * @param position		The position to assign to the renderable.
 */
void
ICS_Renderable::_setPosition(ICS_Pair<float> position)
{
	// set the position
	_position = position;

	// calculate the inverse transformation matrix
	calculateInverseTransformation();
}

/**
 * Sets the dimensions of the renderable.
 * This function is a catch-all for dimension changes.
 * If the renderable's dimensions change, this function will be called.
 *
 * @param dimensions	The dimensions to assign to the renderable.
 */
void
ICS_Renderable::_setDimensions(const ICS_Pair<float>& dimensions)
{
	// set the dimensions
	_dimensions[ICS_WIDTH] = dimensions[ICS_WIDTH] < 0 ? 0 : dimensions[ICS_WIDTH];
	_dimensions[ICS_HEIGHT] = dimensions[ICS_HEIGHT] < 0 ? 0 : dimensions[ICS_HEIGHT];

	// calculate the inverse transformation matrix
	calculateInverseTransformation();
}

/**
 * Sets the scale of the renderable.
 * This function is a catch-all for scale changes.
 * If the renderable's scale changes, this function will be called.
 *
 * @param scale		The scale to assign to the renderable.
 */
void
ICS_Renderable::_setScale(ICS_Pair<float> scale)
{
	// set the scale
	_scale = scale;

	// calculate the inverse transformation matrix
	calculateInverseTransformation();
}

/**
 * Calculates the inverse transformation matrix for this renderable.
 */
void
ICS_Renderable::calculateInverseTransformation()
{
	// inverse transform
	_inverseTransform.identity();											// identity

	// anchor point (if anchor is applied to children)
	if (not _childrenRelativeToAnchor)
	{
		_inverseTransform.translate(_anchor[ICS_X] * _dimensions[ICS_X], _anchor[ICS_Y] * _dimensions[ICS_Y]);
	}

	_inverseTransform.scale(1.0f / _scale[ICS_X], 1.0f / _scale[ICS_Y]);	// scale
	_inverseTransform.rotate(-_rotation);									// rotate
	_inverseTransform.translate(-_position[ICS_X], -_position[ICS_Y]);		// position

	// anchor inverse transform
	_anchorInverseTransform.identity();

	// anchor point (if anchor is not applied to children)
	if (_childrenRelativeToAnchor)
	{
		_anchorInverseTransform.translate(_anchor[ICS_X] * _dimensions[ICS_X], _anchor[ICS_Y] * _dimensions[ICS_Y]);
	}
}

/**
 * Enables the stencil test so elements drawn outside the bounds of this renderable will be clipped.
 * Should be called immediately before render, which should be followed by a call to disableStencilTest.
 *
 * @param left		The x value for the left edge of the clipping area.
 * @param right		The x value for the right edge of the clipping area.
 * @param top		The y value for the top edge of the clipping area.
 * @param bottom	The y value for the bottom edge of the clipping area.
 */
void
ICS_Renderable::enableStencilTest(int left, int right, int top, int bottom)
{
	// remember all openGL attributes to restore later
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// enable the stencil test
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);						// set all stencil values to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);				// keep everything I am drawing to the stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT);							// clear stencil buffer (0 by default)
	glDepthMask(GL_FALSE);									// don't write to depth buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);	// disable all color channels so the quad won't be rendered

	// render the quad
	glBegin(GL_QUADS);
	{
		glVertex2i(left, top);
		glVertex2i(right, top);
		glVertex2i(right, bottom);
		glVertex2i(left, bottom);
	}
	glEnd();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	// enable all color channels
	glDepthMask(GL_TRUE);								// write to depth buffer again
	glStencilFunc(GL_EQUAL, 1, 0xFF);					// pass test if stencil value is 1 (if it's over the quad)
}

/**
 * Disables the stencil test.
 */
void
ICS_Renderable::disableStencilTest()
{
	// disable the stencil test
	glDisable(GL_STENCIL_TEST);

	// restore default settings
	glPopAttrib();
}

/**
 * Checks if the mouse coordinates are over a child or not.
 *
 * @param x		The x component of the mouse coordinates.
 * @param y		The y component of the mouse coordinates.
 *
 * @returns		true if the coordinates are over a child, false if not.
 */
bool
ICS_Renderable::isMouseOverChild(float x, float y) const
{
	// check each child to see if the mouse coordinates are over it
	for (unsigned int i = 0; i < _children.size(); i++)
	{
		if (_children[i]->isMouseOver(x, y, false))
		{
			return true;
		}
	}

	// if this is reached, the mouse is not over any of the children
	return false;
}

/**
 * Checks for mouse enter or leave events for children when the mouse moves over the event handler.
 *
 * @param x		The x component of the mouse coordinates.
 * @param y		The y component of the mouse coordinates.
 */
void
ICS_Renderable::updateMouseOverChild(float x, float y)
{
	// keep track of the child the mouse was over previously
	ICS_Renderable* oldChild = _mouseOverChild;
	_mouseOverChild = NULL;

	// go through each child to determine if the mouse is over one
	for (unsigned int i = 0; i < _children.size() and !_mouseOverChild; i++)
	{
		// if the mouse is over this child...
		if (_children[i]->isMouseOver(x, y, false))
		{
			// keep track of the child
			_mouseOverChild = _children[i];

			// if mouse previously over a different child, generate a mouse enter event
			if (_mouseOverChild != oldChild)
			{
				_mouseOverChild->handleMouseEnter(x, y);
			}
		}
	}

	// generate a mouse leave event if the mouse moved off of the previous child
	if (oldChild and oldChild != _mouseOverChild)
	{
		oldChild->handleMouseLeave();
	}
}

/**
 * Recursively performs a binary search for the index to insert the child at based on its rendering priority.
 *
 * @param start		The index to start searching at.
 * @param end		The index to stop searching at.
 * @param child		The child that needs to be inserted.
 */
int
ICS_Renderable::getChildIndex(int start, int end, ICS_Renderable* child)
{
	// base case... if start is greater than end the value is not in the array
	if (start > end)
	{
		return start;
	}

	// calculate the mid point
	int mid = (start + end) / 2;

	// the value is smaller than the target, discard the first half of the search
	if (_children[mid]->getPriority() > child->getPriority())
	{
		return getChildIndex(mid + 1, end, child);
	}

	// the value is larger than the target, discard the second half of the search
	else if (_children[mid]->getPriority() < child->getPriority())
	{
		return getChildIndex(start, mid - 1, child);
	}

	// if the mid point is not larger or smaller than the target, it must be the target
	return mid;
}

/**
 * Inserts the child into the set of children while maintaining correct rendering order.
 *
 * @param child		The child that needs to be inserted.
 */
void
ICS_Renderable::insertChild(ICS_Renderable* child)
{
	// determine the index to insert at
	int index = getChildIndex(0, _children.size() - 1, child);

	// insert the child
	_children.insert(_children.begin() + index, child);
}

/**
 * Removes and inserts the child from the set of children to maintain correct rendering order.
 *
 * @param child		The child that needs to be inserted.
 */
void
ICS_Renderable::reinsertChild(ICS_Renderable* child)
{
	// find the child in the set
	std::vector<ICS_Renderable*>::iterator it = std::find(_children.begin(), _children.end(), child);

	// if the child is found...
	if (it != _children.end())
	{
		// remove the child from the set
		_children.erase(it);

		// insert the child in the correct rendering order
		insertChild(child);
	}
}