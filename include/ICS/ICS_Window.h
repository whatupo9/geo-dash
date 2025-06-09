/*

ICS_Window

	Created: 2020-04-04

	Change log:

		2020-04-04
			- created ICS_Window to render a set of ICS_Renderables in a spcific area using glScissor

		2020-04-17
			- use stencil test instead of scissor test so ICS_Window can be transformed

		2024-01-27
			- added a "content" renderable to hold all the content that can be scrolled or zoomed
			- added the ability to scroll through the content with the mouse wheel

		2024-01-28
			- added the ability to zoom the content with the mouse wheel

		2024-01-30
			- zooming using the mouse centers on mouse coordinates

		2024-01-30
			- added functions to enable / disable zooming
			- added functions to enable / disable scrolling

		2024-02-03
			- added ability to drag content using the mouse

		2024-02-13
			- allowed increased zooming and centered content in window when content is smaller than the window

		2024-02-14
			- added a reset function to remove all content and disable controls

		2024-02-17
			- inherits from ICS_EventListener so it can receive mouse button events

		2024-05-13
			- fixed bugs caused by mouse coordinates not being converted into local space (zooming, dragging)

		2024-05-16
			- overwrode child management functions so all "children" of a window are added to the window's content

		2024-06-04
			- added function to convert global coordinates to content space

		2024-06-07
			- added copy constructor and assignment operator

*/

#pragma once

#include "ICS_Renderable.h"		// ICS_Window inherits from ICS_Renderable
#include "ICS_EventListener.h"	// ICS_Window inherits from ICS_EventListener

/**
 * A set of ICS_Renderables rendered in a spcific area.
 */
class ICS_Window : public ICS_Renderable, public ICS_EventListener
{

private:

	ICS_Renderable* _content;			// holds the content of the window which can be scrolled or zoomed

	bool _scrollEnabled;				// indicates that vertical scrolling of the content is enabled using the mouse wheel

	bool _zoomEnabled;					// indicates that zooming of the content is enabled usising the mouse wheel 		
	float _zoomScale;					// the current level that the content has been zoomed
	float _maxZoomScale;				// the maximum scale that can be applied to the content by zooming
	float _zoomFactor;					// the factor to multiply / divide by when zomming in / out

	bool _dragEnabled;					// indicates that dragging the content using the mouse is enabled
	int _dragMouseButton;				// indicates which mouse button to use for dragging
	bool _dragging;						// indicates that the content is currently being dragged
	ICS_Pair<float> _dragContentOffset;	// the position of the content when dragging began
	ICS_Pair<float> _dragMouseStart;	// the position of the mouse when dragging began

public:

// constructors / destructor

	/**
	 * ICS_Window constructor.
	 *
	 * @param width		The width of the window in pixels. 
	 * @param height	The height of the window in pixels.
	 */
	ICS_Window(float width, float height);

	/**
	 * ICS_Window copy constructor.
	 *
	 * @param window	The window to copy.
	 */
	ICS_Window(const ICS_Window& window);

	/**
	 * ICS_Window destructor.
	 */
	~ICS_Window();

// assignment operator

	/**
	 * Assignment operator.
	 *
	 * @param window	The window to copy.
	 * 
	 * @returns			A reference to this instance.
	 */
	ICS_Window& operator=(const ICS_Window& window);

// content setters

	/**
	 * Sets the dimensions of the window's content. 
	 *
	 * @param width		The width to assign to the content (in pixels).
	 * @param height	The height to assign to the content (in pixels).
	 * @param autoFit	Indicates that the content should be automatically scaled to fill the window.
	 */
	void setContentDimensions(float width, float height, bool autoFit);

	/**
	 * Sets the position of the window's content relative to the window itself.
	 *
	 * @param dimension		The component of the position to set (ICS_X or ICS_Y).
	 * @param ratio			A value between 0.0f (left / top edge is visible) and 1.0f (right / bottom edge is visible).
	 */
	void setContentPosition(int dimension, float ratio);

// content getters

	/**
	 * Gets the position of the window's content relative to the window itself.
	 *
	 * @param dimension		The component of the position to get (ICS_X or ICS_Y).
	 *
	 * @returns				A value between 0.0f (left / top edge is visible) and 1.0f (right / bottom edge is visible).
	 */
	float getContentPosition(int dimension);

	/**
	 * Gets the percent of the content that is visible in the window (as a decimal).
	 *
	 * @param dimension		The component of the dimension to get the percent for (ICS_WIDTH or ICS_HEIGHT).
	 *
	 * @returns				A value between 0.0f (no content is visible) and 1.0f (all content is visible).
	 */
	float getContentPercent(int dimension);

// public helpers

	/**
	 * Transforms global mouse coordinates into content space.
	 *
	 * @param x	The x component of the coordinates.
	 * @param y	The y component of the coordinates.
	 */
	void globalToContentSpace(float& x, float& y) const
	{
		_content->globalToLocal(x, y);
	}

// child management

	/**
	 * Adds a child to the renderable.
	 *
	 * @param child		The child to add.
	 */
	void addChild(ICS_Renderable* child);

	/**
	 * Removes a child from the renderable.
	 *
	 * @param child		The child to remove.
	 */
	void removeChild(ICS_Renderable* child);

	/**
	 * Removes all children from the renderable.
	 */
	void removeChildren();

// scrolling functions

	/**
	 * Enables vertical scrolling of the content using the mouse wheel. This disables zooming (both cant happen at the same time).
	 *
	 * @param maxZoomScale		The maximum scale that can be applied to the content by zooming (should be greater than 1.0f).
	 * @param zoomFactor		the factor to multiply / divide by when zomming in / out  (should be greater than 1.0f).
	 */
	void enableScrolling();

	/**
	 * Disables vertical scrolling of the content using the mouse wheel.
	 */
	void disableScrolling();

// zoom functions

	/**
	 * Enables zooming of the content using the mouse wheel. This disables vertical scrolling (both cant happen at the same time).
	 *
	 * @param maxZoomScale		The maximum scale that can be applied to the content by zooming (should be greater than 1.0f).
	 * @param zoomFactor		the factor to multiply / divide by when zomming in / out  (should be greater than 1.0f).
	 */
	void enableZoom(float maxZoomScale, float zoomFactor);

	/**
	 * Disables zooming of the content using the mouse wheel.
	 */
	void disableZoom();

	/**
	 * Indicates whether or not the content can be zoomed in.
	 *
	 * @returns	true if the content can be zoomed in, false if the max zoom has been reached.
	 */
	bool canZoomIn();

	/**
	 * Indicates whether or not the content can be zoomed out.
	 *
	 * @returns	true if the content can be zoomed out, false if the min zoom has been reached.
	 */
	bool canZoomOut();

	/**
	 * Zooms in on the content, if possible.
	 */
	void zoomIn();

	/**
	 * Zooms out from the content, if possible.
	 */
	void zoomOut();

// drag functions

	/**
	 * Enbles dragging of the content using the mouse.
	 *
	 * @param button		The button to use for dragging (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 */
	void enableDrag(int button);

	/**
	 * Disables dragging of the content.
	 */
	void disableDrag();

// reset

	/**
	 * Removes all content and disables zooming, scrolling and dragging.
	 */
	void reset();

private:

// event handlers

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
	void handleMouseWheelOver(float x, float y, int wheelRotation);

	/**
	 * This is called when a mouse button is pressed or released.
	 *
	 * @param button		The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param x				The x component of the mouse cursor position.
	 * @param y				The y component of the mouse cursor position.
	 * @param eventType		The type of event (ICS_EVENT_PRESS or ICS_EVENT_RELEASE)
	 */
	void handleMouseButtonEvent(int button, float x, float y, int eventType);

	/**
	 * This is called when a mouse button is pressed over the renderable.
	 *
	 * @param button	The button that triggered the event (ICS_LEFT_MOUSE_BUTTON, ICS_RIGHT_MOUSE_BUTTON or ICS_MIDDLE_MOUSE_BUTTON)
	 * @param x			The x position of the mouse cursor.
	 * @param y			The y position of the mouse cursor.
	 */
	void handleMousePressOver(int button, float x, float y);

// helpers

	/**
	 * Sets the anchor point of the content to the specified window coordinates (transformed into the content's local space).
	 *
	 * @param x		The x position, in window space.
	 * @param y		The y position, in window space.
	 */
	void setContentAnchorPoint(float x, float y);

	/**
	 * Zooms in on the content, if possible.  Zooming will be centered around the specified window coordinates.
	 *
	 * @param x		The x position, in window space.
	 * @param y		The y position, in window space.
	 */
	void zoomIn(float x, float y);

	/**
	 * Zooms out from the content, if possible.  Zooming will be centered around the specified window coordinates.
	 *
	 * @param x		The x position, in window space.
	 * @param y		The y position, in window space.
	 */
	void zoomOut(float x, float y);

	/**
	 * Auto-fits the content to the window by limiting the scale and position.
	 */
	void fitContent();

};