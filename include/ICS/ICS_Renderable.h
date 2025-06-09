/*

ICS_Renderable

	Created: ???

	Change log:

		2020-03-22
			- added event handler for initialization
			- added event handler for mouse movement
			- added event handler for mouse clicks
			- made position setters virtual for override by subclasses

		2020-03-23
			- added function "isMouseOver" for checking if mouse coordinates are over the renderable

		2020-03-24
			- added a render function independent of the "onRender2D" function
			- added getters and setters for width and height
			- used ICS_Pair<int> to represent dimensions instead of width and height
			- added overloaded function to set the renderable's position using ICS_Vector<float>

		2020-03-25
			- added event handler for scrolling
			- added getter for dimensions of the renderable which takes an index as input

		2020-03-26
			- added getter for the renderable's position which returns an ICS_Vector<float>

		2020-04-01
			- added getter for the renderable's dimensions which returns an ICS_Pair<int>

		2020-04-02
			- added function to check if mouse coordinates are over the renderable
			- added event handlers for mouse enter and mouse leave (when the mouse moves over or off of an renderable)

		2020-04-03
			- added event handlers for mouse up, mouse down and mouse click
			- added functionality to subscribe to or unsubscribe from events
			- added children

		2020-04-04
			- added private member attribute for priority and added getters and setters
			- added parents
			- only sort children when their priority changes
			- changed position from ICS_Vector to ICS_Pair

		2020-04-09
			- only trigger mouse wheel events if the mouse is over the event handler
			- added options for anchor points (top left or center)
			- added ability to ignore mouse events

		2020-04-15
			- calculate the inverse transformation matrix for the renderable and use it to detect mouse interaction in local coordinates
			- merged ICS_Renderable and ICS_EventHandler and renamed it ICS_Renderable

		2020-04-17
			- added functions to enable and disable stencil testing... this is to replace scissor testing which can't be transformed

		2023-01-17
			- added a function for changing the alpha transparency of the renderable
			- shortened names of getPositionX, getPositionY, setPositionX and setPositionY to getX, getY, setX and setY

		2023-01-19
			- made ICS_Game a friend class and added a private constructor for the root node

		2023-01-20
			- added a function to check if the renderable is initialized
			- fixed bug where choosing a centered anchor point was not applied to children

		2023-01-25
			- added a function to add to the renderable's rotational value
			- added more options for changing the anchor point of the renderable (from 2 options to 9)

		2023-01-27
			- renamed ICS_Resource to ICS_Renderable

		2023-01-19
			- fixed bug where anchor points were not considered in the inverse transformation matrix (messed with mouse over events)
			- added a greyscale attribute for rendering in greyscale
			- the greyscale property is passed down from parent to child at the time of rendering
			- rendering color is passed down from parent to child at the time of rendering

		2024-01-24
			- added an event for when the mouse is clicked somewhere not over the renderable (so text fields can lose focus)

		2024-01-26
			- handleChildEvent events are propagated to the parent, all the way to the root node
			- added a function to check if the renderable is visible

		2024-01-27
			- added a function to apply transformations so it could be reused by rendering and stencil test
			- added "widow mode" Boolean to tell render function whether or not to use the stencil test
			- made stencil test functions private; child classes should use the Boolean instead
			- fixed bug with rotate function not updating inverse transformation matrix

		2024-01-28
			- set each child's parent to NULL upon deletion
			- fixed a bug where isMouseOver reported incorrect result when the renderable was in window mode
			- added getters for the scale factors
			- added new mouse movement event that only triggers when the mouse is over the redenrable

		2024-01-29
			- prevent copy constructor and assignment operator from copying parent and children
			- added optional absolute anchor point
			- added functions for getting anchor points

		2024-01-30
			- added a function for getting a component of the scale factor
			- added a function for getting a component of the anchor point

		2024-01-31
			- added an event handler for mouse button presses / releases

		2024-02-01
			- modified all mouse button event handlers to work with all 3 mouse buttons (left, right, middle)

		2024-02-10
			- added generic callbacks for events (to replace the same functionality repeated in text fields, buttons, sliders, dropdown menus)
			- removed greyscale feature (cool but expensive)

		2024-02-14
			- added a function to remove all children

		2024-02-15
			- instead of sorting children each time one is added or changes priority, insert them in order with binary insertion

		2024-02-16
			- use float for renderable dimensions instead of int

		2024-02-17
			- removed several event handlers that not all renderables need (optimization)
			- added a function to convert global coordinates into local space

		2024-03-01
			- fixed bug in assignment operator that caused the renderable's parent to become NULL

		2024-03-08
			- made onRender2D public so renderables can be rendered manually

		2024-03-21
			- made applyTransformations protected so ICS_Sprite could define custom behavior

		2024-05-05
			- unified "relative" and "absolute" anchor points
			- anchor points are now specified as floats (0.0f means left, 1.0f means right, 0.5f means center)
			- made applyTransformation private again... ICS_Sprite does not need to define custom behavior after "set dimensions from texture" feature was removed
			- added "child inverse transform" and calculate it but it isn't used yet

		2024-05-07
			- fixed bug with isMouseOver function not correctly detecting when the mouse coordinates are over a child
			- isMouseOver now requires an extra parameter to indicate whether the coordinates are in parent space or global space
			- made some member functions protected that were public but didn't need to be (globalToLocal, window mode functions)
			- added getters and setters for anchor which use ICS_Pair<float>

		2024-05-09
			- anchor points have the option to be applied to children or not depending on the desired behavior

		2024-05-16
			- made child management functions virtual so they can be overwridden (ICS_Window needs this)

		2024-06-04
			- made globalToLocal function public

		2024-06-06
			- fixed a bug where anchor was not being copied correctly in copy constructor

*/

#pragma once

#include <vector>			// a vector container (a variable length array)

#include "ICS_Color.h"		// the definition of ICS_Color
#include "ICS_Pair.h"		// the definition of ICS_Pair (to represent the position and dimensions of the renderable)
#include "ICS_2DMatrix.h"	// the definition of ICS_2DMatrix (for inverse transformation matrices)
#include "ICS_Types.h"		// includes the definition of ICS_EventFunction

/**
 * Represents an object that can be rendered.
 */
class ICS_Renderable
{
	friend class ICS_Game;	// allow the game to access private members

// member attributes
protected:

	bool _visible;											// indicates the renderable should be drawn
	bool _ignoreMouse;										// indicates that mouse events should be ignored

	ICS_Color _color;										// the color of the renderable

private:

	bool _windowMode;										// indicates the whether or not the stencil test should be used

	ICS_Pair<float> _anchor;								// the anchor point for the renderable
	ICS_Pair<float> _position;								// the position for the renderable
	ICS_Pair<float> _dimensions;							// the width and height of the renderable (in pixels)
	ICS_Pair<float> _scale;									// the scaling factor
	float _rotation;										// the amount the renderable is rotated (in degrees)

	ICS_2DMatrix<float> _inverseTransform;					// the inverse transformation matrix for the renderable
	ICS_2DMatrix<float> _anchorInverseTransform;			// the inverse transformation matrix for the anchor point

	bool _childrenRelativeToAnchor;							// if true, children will be positioned relative to the renderable's anchor point
	std::vector<ICS_Renderable*> _children;					// children of this renderable
	ICS_Renderable* _parent;								// the parent of the event handler

	int _priority;											// renderables with higher priority with be notified of events sooner.

	ICS_Renderable* _mouseOverChild;						// keep track of the child that the mouse is over (for mouse enter and mouse leave events)
	ICS_Renderable* _mouseDownChild[ICS_MOUSE_BUTTONS];		// keep track of the child that the is down (for mouse down, up and click events)

	ICS_EventFunction _eventCallback;						// a callback for events triggered by this renderable.
	ICS_RenderableEventFunction _renderableEventCallback;	// a callback for events triggered by this renderable.

// constructor / destructor

private:

	/**
	 * The ICS_Renderable constructor for the ICS_Game root node.
	 *
	 * @param rootNode		Indicates that the renderable is the root node for the game.
	 */
	ICS_Renderable(bool rootNode);

public:

	/**
	 * The ICS_Renderable constructor.
	 */
	ICS_Renderable();

	/**
	 * The copy constructor.  Does not copy parents or children!  The renderable is added to the root node after construction.
	 *
	 * @param gameObject	The instance to copy.
	 */
	ICS_Renderable(const ICS_Renderable& gameObject);

	/**
	 * The ICS_Renderable destructor.
	 */
	virtual ~ICS_Renderable();

// assignment operator

	/**
	 * The assignment operator.  Does not copy parents or children!  The renderable is added to the root node after construction.
	 *
	 * @param renderable	The renderable to copy.
	 * 
	 * @returns				A reference to this instance.
	 */
	ICS_Renderable& operator=(const ICS_Renderable& renderable);

// inquiry

	/**
	 * For checking if the renderable is visible.
	 *
	 * @returns		true if the renderable is visible, false otherwise.
	*/
	bool isVisible()
	{
		return _visible;
	}

// getters

	/**
	 * Gets the renderable's anchor point.
	 *
	 * @returns		The renderable's anchor point.
	 */
	const ICS_Pair<float>& getAnchor() const
	{
		return _anchor;
	}

	/**
	 * Gets the component of the renderable's anchor point indicated by the index.
	 *
	 * @param index		The index of the anchor component (ICS_X or ICS_Y)
	 *
	 * @returns			The component of the renderable's anchor indicated by the index.
	 */
	float getAnchor(int index) const
	{
		if (index == ICS_Y)
		{
			return getAnchorY();
		}

		return getAnchorX();
	}

	/**
	 * Gets the x component of the anchor point.
	 *
	 * @returns		The x component of the anchor point.
	 */
	float getAnchorX() const
	{
		return _anchor[ICS_X];
	}

	/**
	 * Gets the y component of the anchor point.
	 *
	 * @returns		The y component of the anchor point.
	 */
	float getAnchorY() const
	{
		return _anchor[ICS_Y];
	}

	/**
	 * Gets the position of the renderable.
	 *
	 * @returns		The position of the renderable.
	 */
	ICS_Pair<float> getPosition() const
	{
		return _position;
	}

	/**
	 * Gets the component of the renderable's position indicated by the index.
	 *
	 * @param index		The index of the positional component (ICS_X or ICS_Y)
	 *
	 * @returns			The component of the renderable's position indicated by the index.
	 */
	float getPosition(int index) const
	{
		return _position[index == ICS_Y ? ICS_Y : ICS_X];
	}

	/**
	 * Gets the x component of the renderable's position.
	 *
	 * @returns	The x component of the renderable's position.
	 */
	float getX() const
	{
		return _position[ICS_X];
	}

	/**
	 * Gets the y component of the renderable's position.
	 *
	 * @returns	The y component of the renderable's position.
	 */
	float getY() const
	{
		return _position[ICS_Y];
	}

	/**
	 * Gets the dimensions of the renderable.
	 *
	 * @returns		The dimensions of the renderable.
	 */
	ICS_Pair<float> getDimensions() const
	{
		return _dimensions;
	}

	/**
	 * Gets a component of the dimensions of the renderable specified by the index.
	 * The renderable's dimensions are measured in pixels.
	 *
	 * @param index		The index of the dimension to get (ICS_WIDTH or ICS_HEIGHT).
	 *
	 * @returns			The indicated dimension of the renderable.
	 */
	float getDimension(int index) const
	{
		return _dimensions[index == ICS_Y ? ICS_Y : ICS_X];
	}

	/**
	 * Gets the width of the renderable in pixels
	 *
	 * @returns		The width of the renderable (in pixels).
	 */
	float getWidth() const
	{
		return _dimensions[ICS_WIDTH];
	}

	/**
	 * Gets the height of the renderable in pixels.
	 *
	 * @returns		The height of the renderable (in pixels).
	 */
	float getHeight() const
	{
		return _dimensions[ICS_HEIGHT];
	}

	/**
	 * Gets the component of the renderable's scale factor indicated by the index.
	 *
	 * @param index		The index of the scale component (ICS_X or ICS_Y)
	 *
	 * @returns			The component of the renderable's scale factor indicated by the index.
	 */
	float getScale(int index) const
	{
		return _scale[index == ICS_Y ? ICS_Y : ICS_X];
	}

	/**
	 * Gets the x scale factor.
	 *
	 * @returns		The x scale factor.
	 */
	float getScaleX() const
	{
		return _scale[ICS_X];
	}

	/**
	 * Gets the y scale factor.
	 *
	 * @returns		The y scale factor.
	 */
	float getScaleY() const
	{
		return _scale[ICS_Y];
	}

	/**
	 * Gets the rotation of the renderable in degrees
	 *
	 * @returns		The rotation of the renderable in degrees.
	 */
	float getRotation()
	{
		return _rotation;
	}

	/**
	 * Gets the rendering priority of the renderable.
	 *
	 * @returns		The priority of the renderable.  Higher priority renderables are rendered last (on top).
	 */
	int getPriority() const
	{
		return _priority;
	}

// inquiry

	/**
	 * Checks if the mouse coordinates are over the renderable or not.
	 *
	 * @param x			The x component of the mouse coordinates.
	 * @param y			The y component of the mouse coordinates.
	 * @param global	Indicates that the coordinates are in global space (not parent space).
	 *
	 * @returns		true if the coordinates are over the renderable, false if not.
	 */
	virtual bool isMouseOver(float x, float y, bool global = true) const;

// setters

	/**
	 * Sets the flag indicating if the renderable should be drawn or not.
	 *
	 * @param visible		true if the renderable should be drawn, false if not.
	 */
	void setVisible(bool visible)
	{
		_visible = visible;
	}

	/**
	 * Sets the flag that indicates mouse events should be ignored
	 *
	 * @param ignoreMouse	Indicates that mouse events should be ignored.
	 */
	void setIgnoreMouse(bool ignoreMouse)
	{
		_ignoreMouse = ignoreMouse;
	}

	/**
	 * Makes children render relative to the renderable's top left corner (rather than the anchor point).
	 */
	void positionChildrenRelativeToOrigin()
	{
		_childrenRelativeToAnchor = false;

		calculateInverseTransformation();
	}

	/**
	 * Sets the anchor point of the renderable.
	 *
	 * @param anchor	The anchor point to assign to the renderable.
	 */
	void setAnchor(const ICS_Pair<float>& anchor)
	{
		_setAnchor(anchor);
	}

	/**
	 * Sets the anchor point of the renderable.
	 *
	 * @param x		The x coordinate of the anchor for the renderable (0.0f is the left, 0.5f is the center, 1.0f is the right).
	 * @param y		The y coordinate of the anchor for the renderable (0.0f is the top, 0.5f is the center, 1.0f is the bottom).
	 */
	void setAnchor(float x, float y)
	{
		_setAnchor(ICS_Pair<float>(x, y));
	}

	/**
	 * Sets the x component of the anchor point of the renderable.
	 *
	 * @param x		The x coordinate of the anchor for the renderable (0.0f is the left, 0.5f is the center, 1.0f is the right).
	 */
	void setAnchorX(float x)
	{
		_setAnchor(ICS_Pair<float>(x, _anchor[ICS_Y]));
	}

	/**
	 * Sets the y component of the anchor point of the renderable.
	 *
	 * @param y		The y coordinate of the anchor for the renderable (0.0f is the top, 0.5f is the center, 1.0f is the bottom).
	 */
	void setAnchorY(float y)
	{
		_setAnchor(ICS_Pair<float>(_anchor[ICS_X], y));
	}

	/**
	 * Sets the position of the renderable.
	 *
	 * @param position		The position to assign to the renderable.
	 */
	void setPosition(const ICS_Pair<float>& position)
	{
		_setPosition(position);
	}

	/**
	 * Sets the renderable's position.
	 *
	 * @param x		The value to assign to the x component of the position.
	 * @param y		The value to assign to the y component of the position.
	 */
	void setPosition(float x, float y)
	{
		_setPosition(ICS_Pair<float>(x, y));
	}

	/**
	 * Sets a component of the renderable's position.
	 *
	 * @param index		The index of the positional component (ICS_X or ICS_Y).
	 * @param value		The value to assign to the positional component.
	 */
	void setPositionComponent(int index, float value)
	{
		ICS_Pair<float> position = _position;

		if (index == ICS_X || index == ICS_Y)
		{
			position[index] = value;

			_setPosition(position);
		}
	}

	/**
	 * Sets the x component of the renderable's position.
	 *
	 * @param x		The value to assign to the x component of the renderable's position.
	 */
	void setX(float x)
	{
		setPositionComponent(ICS_X, x);
	}

	/**
	 * Sets the y component of the renderable's position.
	 *
	 * @param y		The value to assign to the y component of the renderable's position.
	 */
	void setY(float y)
	{
		setPositionComponent(ICS_Y, y);
	}

	/**
	 * Sets the dimensions of the renderable.
	 *
	 * @param dimensions		The dimensions to assign to the renderable.
	 */
	void setDimensions(const ICS_Pair<float>& dimensions)
	{
		_setDimensions(dimensions);
	}

	/**
	 * Sets the dimensions of the renderable.
	 *
	 * @param width		The width of the renderable in pixels.
	 * @param height	The height of the renderable in pixels.
	 */
	void setDimensions(float width, float height)
	{
		_setDimensions(ICS_Pair<float>(width, height));
	}

	/**
	 * Sets one of the dimensions of the renderable.
	 *
	 * @param index		The index of the dimension to set.
	 * @param value		The value to assign to the dimension.
	 */
	void setDimension(int index, float value)
	{
		ICS_Pair<float> dimensions = _dimensions;

		if (index == ICS_WIDTH || index == ICS_HEIGHT)
		{
			dimensions[index] = value;

			_setDimensions(dimensions);
		}
	}

	/**
	 * Sets the width of the renderable.
	 *
	 * @param width		The width (in pixels).
	 */
	void setWidth(float width)
	{
		_setDimensions(ICS_Pair<float>(width, _dimensions[ICS_HEIGHT]));
	}

	/**
	 * Sets the height of the renderable.
	 *
	 * @param height		The height (in pixels).
	 */
	void setHeight(float height)
	{
		_setDimensions(ICS_Pair<float>(_dimensions[ICS_WIDTH], height));
	}

	/**
	 * Sets the horizontal scaling for the renderable.
	 *
	 * @param scaleX	The horizontal scaling factor (1.0 is 100%)
	 */
	void setScaleX(float scaleX)
	{
		_setScale(ICS_Pair<float>(scaleX, _scale[ICS_Y]));
	}

	/**
	 * Sets the vertical scaling for the renderable.
	 *
	 * @param scaleY	The vertical scaling factor (1.0 is 100%)
	 */
	void setScaleY(float scaleY)
	{
		_setScale(ICS_Pair<float>(_scale[ICS_X], scaleY));
	}

	/**
	 * Sets the scaling for the renderable.
	 *
	 * @param scaleX	The horizontal scaling factor (1.0 is 100%)
	 * @param scaleY	The vertical scaling factor (1.0 is 100%)
	 */
	void setScale(float scaleX, float scaleY)
	{
		_setScale(ICS_Pair<float>(scaleX, scaleY));
	}

	/**
	 * Sets the scaling for the renderable.
	 *
	 * @param scale		The horizontal and vertical scaling factor (1.0 is 100%)
	 */
	void setScale(float scale)
	{
		_setScale(ICS_Pair<float>(scale, scale));
	}

	/**
	 * Sets the rotation of the renderable.
	 *
	 * @param rotation		The amount to rotate the renderable around its center (in degrees).
	 */
	void setRotation(float rotation);

	/**
	 * This sets the color of the renderable.
	 *
	 * @param color		The red, green, blue and alpha components of the color to assign to the renderable.
	 */
	void setColor(const ICS_Color& color);

	/**
	 * This sets the color of the renderable.
	 * Each component of the color should be between ICS_COLOR_MIN and ICS_COLOR_MAX
	 *
	 * @param red		The red component of the renderable's color.
	 * @param green		The green component of the renderable's color.
	 * @param blue		The blue component of the renderable's color.
	 * @param alpha		The alpha transparency component of the renderable's color.
	 */
	void setColor(int red, int green, int blue, int alpha = ICS_COLOR_MAX);

	/**
	 * This sets alpha transparency of the renderable.
	 * The value should be between ICS_COLOR_MIN and ICS_COLOR_MAX
	 *
	 * @param alpha		The alpha transparency component of the renderable's color.
	 */
	void setAlpha(int alpha);

	/**
	 * Sets the priority of the renderable.
	 * Renderables with lower priority are rendered first, behind renderables with higher priority.
	 *
	 * @param priority	The value to assign to the renderable's priority.
	 */
	void setPriority(int priority);

	/**
	 * Sets the callback for events.
	 *
	 * @param callback	The function to call when an event is triggered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(int eventType)
	 */
	void setCallback(ICS_EventFunction callback)
	{
		_eventCallback = callback;
	}

	/**
	 * Sets the callback for events.
	 *
	 * @param callback	The function to call when an event is triggered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(ICS_Renderable* renderable, int eventType)
	 */
	void setCallback(ICS_RenderableEventFunction callback)
	{
		_renderableEventCallback = callback;
	}

// public helpers

	/**
	 * Transforms mouse coordinates into local space (applies inverse transformation).
	 *
	 * @param x	The x component of the coordinates.
	 * @param y	The y component of the coordinates.
	 */
	void inverseTransform(float& x, float& y) const
	{
		_inverseTransform.transform(x, y);
	}

	/**
	 * Transforms global coordinates into local space (applies inverse transformation).
	 *
	 * @param x		The x component of the coordinates.
	 * @param y		The y component of the coordinates.
	 */
	void globalToLocal(float& x, float& y) const
	{
		if (_parent)
		{
			_parent->globalToLocal(x, y);
		}

		_inverseTransform.transform(x, y);
	}

// child management

	/**
	 * Adds a child to the renderable.
	 *
	 * @param child		The child to add.
	 */
	virtual void addChild(ICS_Renderable* child);

	/**
	 * Removes a child from the renderable.
	 *
	 * @param child		The child to remove.
	 */
	virtual void removeChild(ICS_Renderable* child);

	/**
	 * Removes all children from the renderable.
	 */
	virtual void removeChildren();

// event handlers

	/**
	 * This renders 2D game elements.
	 */
	virtual void onRender2D();

protected:

	/**
	 * This handles mouse movement over the renderable events.
	 *
	 * @param x		The x position of the mouse cursor.
	 * @param y		The y position of the mouse cursor.
	 */
	virtual void handleMouseMoveOver(float x, float y);

	/**
	 * This handles mouse wheel events that occur when the cursor is over the renderable.
	 *
	 * @param x					The x position of the mouse cursor.
	 * @param y					The y position of the mouse cursor.
	 * @param wheelRotation		The wheel rotation value.  A positive value indicates that the wheel was rotated forward, away
	 *							from the user; a negative value indicates that the wheel was rotated backward, toward the user.
	 */
	virtual void handleMouseWheelOver(float x, float y, int wheelRotation);

	/**
	 * This is called when the mouse moves over the renderable.
	 *
	 * @param x		The x position of the mouse cursor.
	 * @param y		The y position of the mouse cursor.
	 */
	virtual void handleMouseEnter(float x, float y);

	/**
	 * This is called when the mouse moves off the renderable.
	 */
	virtual void handleMouseLeave();

	/**
	 * This is called when a mouse button is pressed over the renderable.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param x			The x position of the mouse cursor.
	 * @param y			The y position of the mouse cursor.
	 */
	virtual void handleMousePressOver(int button, float x, float y);

	/**
	 * This is called when a mouse button is pressed somewhere NOT over the renderable.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 */
	virtual void handleMousePressNotOver(int button);

	/**
	 * This is called when a mouse button is pressed over the renderable but released off the renderable.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 */
	virtual void handleMouseReleaseNotOver(int button);

	/**
	 * This is called when a mouse button is pressed and released over the renderable.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param x			The x position of the mouse cursor.
	 * @param y			The y position of the mouse cursor.
	 */
	virtual void handleMouseClick(int button, float x, float y);

	/**
	 * Triggers a generic child event for the parent.
	 *
	 * @param eventType		The type of event.
	 */
	void triggerChildEvent(int eventType)
	{
		// notify the parent
		if (_parent)
		{
			_parent->handleChildEvent(this, eventType);
		}

		// trigger the event callback
		if (_eventCallback)
		{
			_eventCallback(eventType);
		}

		// trigger the renderable event callback
		if (_renderableEventCallback)
		{
			_renderableEventCallback(this, eventType);
		}
	}

	/**
	 * This handles generic child events (for children to notify their parent).
	 *
	 * @param child			The child that triggered the event.
	 * @param eventType		The type of event.
	 */
	virtual void handleChildEvent(ICS_Renderable* child, int eventType)
	{
		// notify the parent
		if (_parent)
		{
			_parent->handleChildEvent(child, eventType);
		}
	}

// transformation helpers

	/**
	 * Sets the anchor of the renderable.
	 * This function is a catch-all for anchor changes.
	 * If the renderable's anchor changes, this function will be called.
	 *
	 * @param anchor	The anchor to assign to the renderable.
	 */
	virtual void _setAnchor(ICS_Pair<float> anchor);

	/**
	 * Sets the position of the renderable.
	 * This function is a catch-all for position changes.
	 * If the renderable's position changes, this function will be called.
	 *
	 * @param position		The position to assign to the renderable.
	 */
	virtual void _setPosition(ICS_Pair<float> position);

	/**
	 * Sets the dimensions of the renderable.
	 * This function is a catch-all for dimension changes.
	 * If the renderable's dimensions change, this function will be called.
	 *
	 * @param dimensions	The dimensions to assign to the renderable.
	 */
	virtual void _setDimensions(const ICS_Pair<float>& dimensions);

	/**
	 * Sets the scale of the renderable.
	 * This function is a catch-all for scale changes.
	 * If the renderable's scale changes, this function will be called.
	 *
	 * @param scale		The scale to assign to the renderable.
	 */
	virtual void _setScale(ICS_Pair<float> scale);

	/**
	 * Calculates the inverse transformation matrix for this renderable.
	 */
	void calculateInverseTransformation();

	/**
	 * Transforms coordinates by applying the renderable's anchor point transformation.
	 *
	 * @param x	The x component of the coordinates.
	 * @param y	The y component of the coordinates.
	 */
	void anchorInverseTransform(float& x, float& y) const
	{
		_anchorInverseTransform.transform(x, y);
	}

// rendering helpers

	/**
	 * This enables window mode which clips any elements outside the renderable when rendering.
	 */
	void enableWindowMode()
	{
		_windowMode = true;
	}

	/**
	 * This disables window mode so no elements are clipped when rendering.
	 */
	void disableWindowMode()
	{
		_windowMode = false;
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
	void enableStencilTest(int left, int right, int top, int bottom);

	/**
	 * Disables the stencil test.
	 */
	void disableStencilTest();

	/**
	 * Renders the renderable.
	 */
	virtual void render()
	{
	}

// child helper functions

	/**
	 * Checks if the mouse coordinates are over a child or not.
	 *
	 * @param x		The x component of the mouse coordinates.
	 * @param y		The y component of the mouse coordinates.
	 *
	 * @returns		true if the coordinates are over a child, false if not.
	 */
	bool isMouseOverChild(float x, float y) const;

private:

	/**
	 * Checks for mouse enter or leave events for children when the mouse moves over the event handler.
	 *
	 * @param x		The x component of the mouse coordinates.
	 * @param y		The y component of the mouse coordinates.
	 */
	void updateMouseOverChild(float x, float y);

	/**
	 * Recursively performs a binary search for the index to insert the child at based on its rendering priority.
	 *
	 * @param start		The index to start searching at.
	 * @param end		The index to stop searching at.
	 * @param child		The child that needs to be inserted.
	 */
	int getChildIndex(int start, int end, ICS_Renderable* child);

	/**
	 * Inserts the child into the set of children while maintaining correct rendering order.
	 *
	 * @param child		The child that needs to be inserted.
	 */
	void insertChild(ICS_Renderable* child);

	/**
	 * Removes and inserts the child from the set of children to maintain correct rendering order.
	 *
	 * @param child		The child that needs to be inserted.
	 */
	void reinsertChild(ICS_Renderable* child);

};