/*

ICS_Game

	Created: ???

	Change log:

		2020-03-22
			- notify renderables of initialization events
			- notify renderables of mouse movement events
			- notify renderables of mouse button events
			- flip y coordinates... top left of the screen is (0, 0)
			- added getters for widow width and height

		2020-03-24
			- only auto-render if a render 2D callback has not been assigned

		2020-03-25
			- enabled maximize window button

		2020-04-02
			- added variables and logic to trigger mouse enter and mouse leave events for renderables

		2020-04-03
			- added logic to trigger mouse down, up and click events for renderables

		2020-04-04
			- capture mouse events outside of client window

		2020-04-15
			- manage updateables and renderables separately

		2022-12-03
			- added event callback for window resize

		2023-01-13
			- added the option to have a fixed sized game window

		2023-01-20
			- fonts are auto-managed by the game so they can be initialized at the right time

		2023-01-27
			- fonts are now ICS_GameObjects so the redundant auto-management system was removed

		2023-10-31
			- added full screen mode

		2024-01-18
			- added a callback for when the game is initialized

		2024-01-23
			- notify game objects like fonts and textures that the game is initialized first before renderables

		2024-02-10
			- improved rendering by applying common OpenGL settings to the root node instead of each time a sprite is rendered.

		2024-02-17
			- maintain sets of event listeners so events can be dispatched conservatively.

		2024-04-19
			- prevent the render context from being deleted when the window is closed so it will still exist if a new window is created (like full screen mode)

		2024-05-19
			- added error logging

		2024-06-04
			- mouse coordinates and screen dimensions are now float instead of int

*/

#pragma once

#include <winsock2.h>		// for Windows internet communication... this must come before Windows.h!
#include <Windows.h>		// for creating a window
#include <time.h>			// for getting the current time
#include <string>			// for string

#include "ICS_Color.h"		// the definition of ICS_Color
#include "ICS_Types.h"		// for callback function pointer definitions

class ICS_EventListener;	// forward declare ICS_EventListener as ICS_Game manages an array of them
class ICS_Renderable;		// forward declare ICS_Renderable to declare a root node for all graphics

/**
 * This class can be used as the basic framework for a game.  It takes care of creating a window,
 * handling keyboard and mouse input, rendering OpenGL graphics and managing an update loop.
 */
class ICS_Game
{

private:

	bool		_fullScreen;										// indicates the window is in full screen
	bool		_fixedSize;											// indicates the window is fixed sized (can't be resized)
	bool		_initialized;										// indicates that the game has been initialized
	bool		_active;											// indicates the window is active
	bool		_done;												// indicates the game loop is complete and the game will exit

	std::string	_windowTitle;										// the title to display on the game window				
	int			_windowedModeWindowWidth;							// the width of the window in pixels for windowed mode
	int			_windowedModeWindowHeight;							// the height of the window in pixels for windowed mode
	int			_windowWidth;										// the width of the OpenGL viewport in pixels
	int			_windowHeight;										// the height of the OpenGL viewport in pixels

	HDC			_deviceContextHandle;								// a handle for the device context ?
	HGLRC		_renderContextHandle;								// a handle for the render context ?
	HWND		_windowHandle;										// a handle for the window
	HINSTANCE	_moduleHandle;										// a handle for the instance of the application ?

	clock_t		_lastTime;											// the time of the last update for managing frame rate
	clock_t		_keys[256];											// keeps track of the state of each keyboard key

	ICS_Color	_backgroundColor;									// the background color

	std::vector<ICS_EventListener*> _gameInitializedEventListeners;	// all objects which listen for game initialized events
	std::vector<ICS_EventListener*> _updateEventListeners;			// all objects which listen for update events
	std::vector<ICS_EventListener*> _keyboardEventListeners;		// all objects which listen for keyboard events
	std::vector<ICS_EventListener*> _mouseMoveEventListeners;		// all objects which listen for mouse movement events
	std::vector<ICS_EventListener*> _mouseButtonEventListeners;		// all objects which listen for mouse button events

	ICS_Renderable* _rootNode;										// the root node for renderables

	ICS_UpdateEventFunction _updateEventCallback;					// the callback for updating the game
	ICS_Render2DEventFunction _render2DEventCallback;				// the callback for rendering 2D graphics
	ICS_Render3DEventFunction _render3DEventCallback;				// the callback for rendering 3D graphics
	ICS_KeyboardEventFunction _keyboardEventCallback;				// the callback for keyboard events
	ICS_MouseMoveEventFunction _mouseMoveEventCallback;				// the callback for mouse move events
	ICS_MouseButtonEventFunction _mouseButtonEventCallback;			// the callback for mouse button events
	ICS_MouseWheelEventFunction _mouseWheelEventCallback;			// the callback for mouse wheel events
	ICS_ResizeWindowEventFunction _resizeWindowEventCallback;		// the callback for resizing the game window
	ICS_InitializeEventFunction _initializeEventCallback;			// the callback for initializing the game
	ICS_ExitEventFunction _exitEventCallback;						// the callback for closing the game window

public:

// singleton

	/**
	 * This class use a singleton.  Only one instance can exist.
	 * This function returns a reference to the instance.
	 *
	 * @returns		The singleton instance of ICS_Game
	 */
	static ICS_Game& getInstance();

private:

// constructor / destructor

	/**
	 * ICS_Game constructor.
	 */
	ICS_Game();

	/**
	 * Copy constructor (not implemented to prevent copying)
	 */
	ICS_Game(ICS_Game const&);

	/**
	 * Assignment operator (not implemented to prevent copying)
	 */
	void operator=(ICS_Game const&) = delete;

public:

// setters

	/**
	 * This sets the background color of the window.
	 *
	 * @param red		The red component of the background color (0 is the min, 255 is the max).
	 * @param green		The green component of the background color (0 is the min, 255 is the max).
	 * @param blue		The blue component of the background color (0 is the min, 255 is the max).
	 */
	void setBackgroundColor(int red, int green, int blue);

// getters

	/**
	 * Gets the width of the game window.
	 * 
	 * @returns		The width of the game window (in pixels).
	 */
	int getWindowWidth()
	{
		return _windowWidth;
	}

	/**
	 * Gets the height of the game window.
	 *
	 * @returns		The height of the game window (in pixels).
	 */
	int getWindowHeight()
	{
		return _windowHeight;
	}

// inquiry

	/**
	 * Checks if the game has been initialized.
	 *
	 * @returns		true if the game has been initialized, false otherwise. 
	 */
	bool isInitialized()
	{
		return _initialized;
	}

// callback management

	/**
	 * Sets the callback for updating game logic.
	 *
	 * @param callback	The function to call when game logic should be updated.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(float elapsed)
	 */
	void setUpdateEventCallback(ICS_UpdateEventFunction callback)
	{
		_updateEventCallback = callback;
	}

	/**
	 * Sets the callback for rendering 2D graphics.
	 *
	 * @param callback	The function to call when 2D graphics should be rendered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback()
	 */
	void setRender2DEventCallback(ICS_Render2DEventFunction callback)
	{
		_render2DEventCallback = callback;
	}

	/**
	 * Sets the callback for rendering 3D graphics.
	 *
	 * @param callback	The function to call when 3D graphics should be rendered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback()
	 */
	void setRender3DEventCallback(ICS_Render3DEventFunction callback)
	{
		_render3DEventCallback = callback;
	}

	/**
	 * Sets the callback for keyboard events.
	 *
	 * @param callback	The function to call when a keyboard event is triggered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(int key, int eventType)
	 */
	void setKeyboardEventCallback(ICS_KeyboardEventFunction callback)
	{
		_keyboardEventCallback = callback;
	}

	/**
	 * Sets the callback for mouse move events.
	 *
	 * @param callback	The function to call when a mouse move event is triggered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(float mouseX, float mouseY)
	 */
	void setMouseMoveEventCallback(ICS_MouseMoveEventFunction callback)
	{
		_mouseMoveEventCallback = callback;
	}

	/**
	 * Sets the callback for mouse button events.
	 *
	 * @param callback	The function to call when a mouse button event is triggered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(int mouseButton, float mouseX, float mouseY, int eventType)
	 */
	void setMouseButtonEventCallback(ICS_MouseButtonEventFunction callback)
	{
		_mouseButtonEventCallback = callback;
	}

	/**
	 * Sets the callback for mouse wheel events.
	 *
	 * @param callback	The function to call when a mouse wheel event is triggered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(float mouseX, float mouseY, int wheelRotation)
	 */
	void setMouseWheelEventCallback(ICS_MouseWheelEventFunction callback)
	{
		_mouseWheelEventCallback = callback;
	}

	/**
	 * Sets the callback for window resizing events.
	 *
	 * @param callback	The function to call the game window is rezized.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback(float width, float height)
	 */
	void setWindowResizeEventCallback(ICS_ResizeWindowEventFunction callback)
	{
		_resizeWindowEventCallback = callback;
	}

	/**
	 * Sets the callback for game initialization events.
	 *
	 * @param callback	The function to call when the game is intialized.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback()
	 */
	void seInitializeEventCallback(ICS_InitializeEventFunction callback)
	{
		_initializeEventCallback = callback;
	}

	/**
	 * Sets the callback for exit events.
	 *
	 * @param callback	The function to call when an exit event is triggered.
	 *					The function must match the return type and parameter list of this prototype:
	 *
	 *					void callback()
	 */
	void setExitEventCallback(ICS_ExitEventFunction callback)
	{
		_exitEventCallback = callback;
	}

// event listener management

	/**
	 * Adds an ICS_EventListener to the game.
	 * The ICS_EventListener will be notified when the game is initialized.
	 *
	 * @param listener		The ICS_EventListener to add.
	 */
	void addGameInitializedEventListener(ICS_EventListener* listener);

	/**
	 * Removes an ICS_EventListener from the game.
	 * The ICS_EventListener will no longer be notified when the game is initialized.
	 *
	 * @param listener		The ICS_EventListener to remove.
	 */
	void removeGameInitializedEventListener(ICS_EventListener* listener);

	/**
	 * Adds an ICS_EventListener to the game.
	 * The ICS_EventListener will be notified each time an update event is triggered.
	 * 
	 * @param listener		The ICS_EventListener to add.
	 */
	void addUpdateEventListener(ICS_EventListener* listener);

	/**
	 * Removes an ICS_EventListener from the game.
	 * The ICS_EventListener will stop recieving notifications for update events.
	 *
	 * @param listener		The ICS_EventListener to remove.
	 */
	void removeUpdateEventListener(ICS_EventListener* listener);

	/**
	 * Adds an ICS_EventListener to the game.
	 * The ICS_EventListener will be notified each time a keyboard event is triggered.
	 *
	 * @param listener		The ICS_EventListener to add.
	 */
	void addKeyboardEventListener(ICS_EventListener* listener);

	/**
	 * Removes an ICS_EventListener from the game.
	 * The ICS_EventListener will stop recieving notifications for keyboard events.
	 *
	 * @param listener		The ICS_EventListener to remove.
	 */
	void removeKeyboardEventListener(ICS_EventListener* listener);

	/**
	 * Adds an ICS_EventListener to the game.
	 * The ICS_EventListener will be notified each time a mouse movement event is triggered.
	 *
	 * @param listener		The ICS_EventListener to add.
	 */
	void addMouseMoveEventListener(ICS_EventListener* listener);

	/**
	 * Removes an ICS_EventListener from the game.
	 * The ICS_EventListener will stop recieving notifications for mouse movement events.
	 *
	 * @param listener		The ICS_EventListener to remove.
	 */
	void removeMouseMoveEventListener(ICS_EventListener* listener);

	/**
	 * Adds an ICS_EventListener to the game.
	 * The ICS_EventListener will be notified each time a mouse button event is triggered.
	 *
	 * @param listener		The ICS_EventListener to add.
	 */
	void addMouseButtonEventListener(ICS_EventListener* listener);

	/**
	 * Removes an ICS_EventListener from the game.
	 * The ICS_EventListener will stop recieving notifications for mouse button events.
	 *
	 * @param listener		The ICS_EventListener to remove.
	 */
	void removeMouseButtonEventListener(ICS_EventListener* listener);

// renderable management

	/**
	 * Adds a renderable to the game.
	 *
	 * @param renderable	The renderable to add.
	 */
	void add(ICS_Renderable* renderable);

	/**
	 * This removes a renderable from the game.
	 *
	 * @param renderable	The renderable to remove.
	 */
	void remove(ICS_Renderable* renderable);

// game loop

	/**
	 * This creates a window, starts the game loop, handles updating and rendering and finally destroys the window before returning.
	 *
	 * @param title			The title to appear at the top of the window.
	 * @param width			The width of the window in pixels.
	 * @param height		The height of the window in pixels.
	 * @param fixedSize		Indicates whether or not the game window should be fixed size (not resizable).
	 * @param fullScreen	Indicates the game should start in full screen mode.
	 *
	 * @returns				A flag... 0 for success, non-zero in the case of an error.
	 */
	int go(std::string title, int width, int height, bool fixedSize = false, bool fullScreen = false);

	/**
	 * Ends the game loop.
	 */
	void stop()
	{
		_done = true;
	}

	/**
	 * Toggles fullscreen mode.
	 *
	 * @returns	true on success, false on failure.
	 */
	bool toggleFullScreenMode();

// helpers

private:

	/**
	 * This updates the game and all renderables at a constant framerate.
	 */
	void update();

	/**
	 * This renders the game by invoking the render callback.
	 */
	void render();

// message handling

	/**
	 * This function receives all input directed at the game window.
	 *
	 * @param hWnd		A handle to the window.
	 * @param uMsg		The message.
	 * @param wParam	Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
	 * @param lParam	Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
	 *
	 * @returns			The result of the message processing and depends on the message sent.
	 */
	static LRESULT CALLBACK windowMessageCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	 * This function processes all input directed at the game window.
	 *
	 * @param hWnd		A handle to the window.
	 * @param uMsg		The message.
	 * @param wParam	Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
	 * @param lParam	Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
	 *
	 * @returns			The result of the message processing and depends on the message sent.
	 */
	LRESULT CALLBACK processWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// window management

	/**
	 * This creates the game window.
	 *
	 * @param title			The title to appear at the top of the window.
	 * @param width			The width of the window in pixels.
	 * @param height		The height of the window in pixels.
	 * @param fixedSize		Indicates whether or not the game window should be fixed size (not resizable).
	 *
	 * @returns				true for success, false for failure.
	 */
	bool createWindow(std::string title, int width, int height, bool fixedSize);

	/**
	 * This updates the viewport resolution.  It is called each time the game window is resized.
	 *
	 * @param width		The width of the window in pixels.
	 * @param height	The height of the window in pixels.
	 */
	void updateViewport(int width, int height);

	/**
	 * This properly destroys the game window.
	 *
	 * @param destroyRenderingContext	Indicates the OpenGL rendering context should be destroyed as well.
	 */
	void destroyWindow(bool destroyRenderingContext);
};
