#include "ICS_Game.h"							// the definition of ICS_Game
#include "ICS_EventListener.h"					// the definition of ICS_EventListener
#include "ICS_Resource.h"						// the definition of ICS_Resource
#include "ICS_Renderable.h"						// the definition of ICS_Renderable
#include "ICS_DebugLog.h"						// the definition of ICS_DebugLog

#include <glut.h>								// for OpenGL
#include <Windowsx.h>							// for getting mouse parameters when handling events
#include <map>									// for mapping window handles to ICS_Game instances
#include <algorithm>							// for find

#pragma comment (lib, "glut32.lib")				// for OpenGL

std::map<HWND, ICS_Game*> ICS_GameInstances;	// a map from window handles to ICS_Game instances (for message handling)

/**
 * This class use a singleton.  Only one instance can exist.
 * This function returns a reference to the instance.
 *
 * @returns		The singleton instance of ICS_Game
 */
ICS_Game&
ICS_Game::getInstance()
{
	static ICS_Game instance;

	return instance;
}

/**
 * The default constructor.
 */
ICS_Game::ICS_Game()
	:
	_fullScreen(false),
	_fixedSize(false),
	_initialized(false),
	_active(true),
	_done(false),
	_windowTitle("ICS Game"),
	_windowedModeWindowWidth(0),
	_windowedModeWindowHeight(0),
	_windowWidth(0),
	_windowHeight(0),
	_deviceContextHandle(NULL),
	_renderContextHandle(NULL),
	_windowHandle(NULL),
	_moduleHandle(NULL),
	_lastTime(0),
	_backgroundColor(0, 0, 0),
	_rootNode(NULL),
	_updateEventCallback(NULL),
	_render2DEventCallback(NULL),
	_render3DEventCallback(NULL),
	_keyboardEventCallback(NULL),
	_mouseMoveEventCallback(NULL),
	_mouseButtonEventCallback(NULL),
	_mouseWheelEventCallback(NULL),
	_resizeWindowEventCallback(NULL),
	_exitEventCallback(NULL)
{
	// initialize the state of each key
	for (int i = 0; i < 256; i++)
	{
		_keys[i] = -1;
	}

	// create the root node for renderables
	_rootNode = new ICS_Renderable(true);
}

/**
* This sets the background color of the window.
*
* @param red		The red component of the background color (0 is the min, 255 is the max).
* @param green		The green component of the background color (0 is the min, 255 is the max).
* @param blue		The blue component of the background color (0 is the min, 255 is the max).
*/
void
ICS_Game::setBackgroundColor(int red, int green, int blue)
{
	// clamp the input values
	ICS_clamp(red, ICS_COLOR_MIN, ICS_COLOR_MAX);
	ICS_clamp(green, ICS_COLOR_MIN, ICS_COLOR_MAX);
	ICS_clamp(blue, ICS_COLOR_MIN, ICS_COLOR_MAX);

	// set the member attributes
	_backgroundColor.red = red;
	_backgroundColor.green = green;
	_backgroundColor.blue = blue;

	// set the background color
	_backgroundColor.setClearColor();
}

/**
 * Adds an ICS_EventListener to the game.
 * The ICS_EventListener will be notified when the game is initialized.
 *
 * @param listener		The ICS_EventListener to add.
 */
void
ICS_Game::addGameInitializedEventListener(ICS_EventListener* listener)
{
	// if the game is already initialized, ignore this
	if (_initialized)
	{
		return;
	}

	// check if the listener is already in the set before adding it
	if (std::find(_gameInitializedEventListeners.begin(), _gameInitializedEventListeners.end(), listener) == _gameInitializedEventListeners.end())
	{
		_gameInitializedEventListeners.push_back(listener);
	}
}

/**
 * Removes an ICS_EventListener from the game.
 * The ICS_EventListener will no longer be notified when the game is initialized.
 *
 * @param listener		The ICS_EventListener to remove.
 */
void
ICS_Game::removeGameInitializedEventListener(ICS_EventListener* listener)
{
	// find the listener in the set
	std::vector<ICS_EventListener*>::iterator it = std::find(_gameInitializedEventListeners.begin(), _gameInitializedEventListeners.end(), listener);

	// if the gameObject is found then remove it from the set
	if (it != _gameInitializedEventListeners.end())
	{
		_gameInitializedEventListeners.erase(it);
	}
}

/**
 * Adds an ICS_EventListener to the game.
 * The ICS_EventListener will be notified each time an update event is triggered.
 *
 * @param listener		The ICS_EventListener to add.
 */
void
ICS_Game::addUpdateEventListener(ICS_EventListener* listener)
{
	// check if the listener is already in the set before adding it
	if (std::find(_updateEventListeners.begin(), _updateEventListeners.end(), listener) == _updateEventListeners.end())
	{
		_updateEventListeners.push_back(listener);
	}
}

/**
 * Removes an ICS_EventListener from the game.
 * The ICS_EventListener will stop recieving notifications for update events.
 *
 * @param listener		The ICS_EventListener to remove.
 */
void
ICS_Game::removeUpdateEventListener(ICS_EventListener* listener)
{
	// find the listener in the set
	std::vector<ICS_EventListener*>::iterator it = std::find(_updateEventListeners.begin(), _updateEventListeners.end(), listener);

	// if the gameObject is found then remove it from the set
	if (it != _updateEventListeners.end())
	{
		_updateEventListeners.erase(it);
	}
}

/**
 * Adds an ICS_EventListener to the game.
 * The ICS_EventListener will be notified each time a keyboard event is triggered.
 *
 * @param listener		The ICS_EventListener to add.
 */
void
ICS_Game::addKeyboardEventListener(ICS_EventListener* listener)
{
	// check if the listener is already in the set before adding it
	if (std::find(_keyboardEventListeners.begin(), _keyboardEventListeners.end(), listener) == _keyboardEventListeners.end())
	{
		_keyboardEventListeners.push_back(listener);
	}
}

/**
 * Removes an ICS_EventListener from the game.
 * The ICS_EventListener will stop recieving notifications for keyboard events.
 *
 * @param listener		The ICS_EventListener to remove.
 */
void
ICS_Game::removeKeyboardEventListener(ICS_EventListener* listener)
{
	// find the listener in the set
	std::vector<ICS_EventListener*>::iterator it = std::find(_keyboardEventListeners.begin(), _keyboardEventListeners.end(), listener);

	// if the gameObject is found then remove it from the set
	if (it != _keyboardEventListeners.end())
	{
		_keyboardEventListeners.erase(it);
	}
}

/**
 * Adds an ICS_EventListener to the game.
 * The ICS_EventListener will be notified each time a mouse movement event is triggered.
 *
 * @param listener		The ICS_EventListener to add.
 */
void
ICS_Game::addMouseMoveEventListener(ICS_EventListener* listener)
{
	// check if the listener is already in the set before adding it
	if (std::find(_mouseMoveEventListeners.begin(), _mouseMoveEventListeners.end(), listener) == _mouseMoveEventListeners.end())
	{
		_mouseMoveEventListeners.push_back(listener);
	}
}

/**
 * Removes an ICS_EventListener from the game.
 * The ICS_EventListener will stop recieving notifications for mouse movement events.
 *
 * @param listener		The ICS_EventListener to remove.
 */
void
ICS_Game::removeMouseMoveEventListener(ICS_EventListener* listener)
{
	// find the listener in the set
	std::vector<ICS_EventListener*>::iterator it = std::find(_mouseMoveEventListeners.begin(), _mouseMoveEventListeners.end(), listener);

	// if the gameObject is found then remove it from the set
	if (it != _mouseMoveEventListeners.end())
	{
		_mouseMoveEventListeners.erase(it);
	}
}

/**
 * Adds an ICS_EventListener to the game.
 * The ICS_EventListener will be notified each time a mouse button event is triggered.
 *
 * @param listener		The ICS_EventListener to add.
 */
void
ICS_Game::addMouseButtonEventListener(ICS_EventListener* listener)
{
	// check if the listener is already in the set before adding it
	if (std::find(_mouseButtonEventListeners.begin(), _mouseButtonEventListeners.end(), listener) == _mouseButtonEventListeners.end())
	{
		_mouseButtonEventListeners.push_back(listener);
	}
}

/**
 * Removes an ICS_EventListener from the game.
 * The ICS_EventListener will stop recieving notifications for mouse button events.
 *
 * @param listener		The ICS_EventListener to remove.
 */
void
ICS_Game::removeMouseButtonEventListener(ICS_EventListener* listener)
{
	// find the listener in the set
	std::vector<ICS_EventListener*>::iterator it = std::find(_mouseButtonEventListeners.begin(), _mouseButtonEventListeners.end(), listener);

	// if the gameObject is found then remove it from the set
	if (it != _mouseButtonEventListeners.end())
	{
		_mouseButtonEventListeners.erase(it);
	}
}

/**
 * Adds a renderable to the game.
 *
 * @param renderable	The renderable to add.
 */
void
ICS_Game::add(ICS_Renderable* renderable)
{
	if (_rootNode)
	{
		_rootNode->addChild(renderable);
	}
}

/**
 * This removes a renderable from the game.
 *
 * @param renderable	The renderable to remove.
 */
void
ICS_Game::remove(ICS_Renderable* renderable)
{
	if (_rootNode)
	{
		_rootNode->removeChild(renderable);
	}
}

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
int
ICS_Game::go(std::string title, int width, int height, bool fixedSize, bool fullScreen)
{
	// set the attributes of the game window
	_fullScreen = fullScreen;
	_fixedSize = fixedSize;
	_windowTitle = title;
	_windowedModeWindowWidth = width;
	_windowedModeWindowHeight = height;

	// going to fullscreen mode?  determine the dimensions for the viewport
	if (_fullScreen)
	{
		const HWND hDesktop = GetDesktopWindow();	// get a handle to the desktop window
		RECT desktop;								// to hold the dimensions of the desktop
		GetWindowRect(hDesktop, &desktop);			// get the size of screen to the variable desktop
		width = desktop.right;						// set the width of the viewport
		height = desktop.bottom;					// set the height of the viewport
	}

	// attempt to create the game window
	if (!createWindow(title, width, height, fixedSize))
	{
		return -1;
	}

	// declare a variable for handling messages from the window
	MSG msg;

	// start the clock
	_lastTime = clock();

	// the engine is initialized
	_initialized = true;

	// invoke the callback for game initialization
	if (_initializeEventCallback)
	{
		_initializeEventCallback();
	}

	// notify all event listeners of the event
	for (int i = _gameInitializedEventListeners.size() - 1; i >= 0; i--)
	{
		_gameInitializedEventListeners[i]->handleGameInitializedEvent();
	}

	// repeat the loop until a quit message is received
	_done = false;

	while (!_done)
	{
		// check if there is a message waiting
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// if the message is quit, set the done flag so the loop will complete
			if (msg.message == WM_QUIT)
			{
				_done = true;
			}
			// otherwise, translate and dispatch the message
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// if there are no messages, update and render the game
		else if (_active)
		{
			// update
			update();

			// render
			render();
		}
	}

	// trigger the exit callback
	if (_exitEventCallback)
	{
		_exitEventCallback();
	}

	// when the loop is complete, destroy the window and rendering context
	destroyWindow(true);

	// return the result from the last message received
	return msg.wParam;
}

/**
 * Toggles fullscreen mode.
 *
 * @returns	true on success, false on failure.
 */
bool
ICS_Game::toggleFullScreenMode()
{
	// kill the current window
	destroyWindow(false);

	// toggle the value of the fullscreen mode flag
	_fullScreen = (not _fullScreen);

	// state the viewport dimensions
	int width = _windowedModeWindowWidth;
	int height = _windowedModeWindowHeight;

	// going to fullscreen mode?  determine the dimensions for the viewport
	if (_fullScreen)
	{
		const HWND hDesktop = GetDesktopWindow();	// get a handle to the desktop window
		RECT desktop;								// to hold the dimensions of the desktop
		GetWindowRect(hDesktop, &desktop);			// get the size of screen to the variable desktop
		width = desktop.right;						// set the width of the viewport
		height = desktop.bottom;					// set the height of the viewport
	}

	// attempt to create the window
	return createWindow(_windowTitle, width, height, _fixedSize);
}

/**
 * This updates the game and all renderables at a constant framerate.
 */
void
ICS_Game::update()
{
	// determine how much time has elapsed since the last update
	clock_t currentTime = clock();
	float elapsed = currentTime - _lastTime;

	// limit the framerate to 60 FPS
	if (elapsed >= CLK_TCK / 60)
	{
		// update the time for the next call
		_lastTime = currentTime;

		// update the game logic
		if (_updateEventCallback)
		{
			_updateEventCallback(elapsed / 1000);
		}

		// notify all update event listeners of the event
		for (int i = _updateEventListeners.size() - 1; i >= 0; i--)
		{
			_updateEventListeners[i]->handleUpdateEvent(elapsed / 1000);
		}
	}
}

/**
 * This renders the game by invoking the render callback.
 */
void
ICS_Game::render()
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset transformations
	glLoadIdentity();

	// 3D rendering
	if (_render3DEventCallback)
	{
		glMatrixMode(GL_PROJECTION);															// Select The Projection Matrix
		glLoadIdentity();																		// Reset The Projection Matrix
		gluPerspective(45.0f, (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.1f, 10000.0f);	// Calculate The Aspect Ratio Of The Window
		glMatrixMode(GL_MODELVIEW);																// Select The Modelview Matrix
		glLoadIdentity();																		// Reset The Modelview Matrix

		_render3DEventCallback();
	}

	// 2D rendering
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluOrtho2D(0, _windowWidth, _windowHeight, 0);		// define the 2D orthographic projection matrix for sprites
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	glPushAttrib(GL_ALL_ATTRIB_BITS);					// remember all openGL attributes
	glDisable(GL_LIGHTING);								// disable lighting
	glEnable(GL_BLEND);									// enable blending
	glDisable(GL_DEPTH_TEST);							// disable depth testing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// apply blending so transparent parts of textures will not be drawn

	// if a custom draw 2D callback has been set, use it
	if (_render2DEventCallback)
	{
		_render2DEventCallback();
	}

	// otherwise, auto-draw all renderables
	else
	{
		_rootNode->onRender2D();
	}

	// restore default settings
	glPopAttrib();

	// swap buffers
	SwapBuffers(_deviceContextHandle);
}

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
LRESULT CALLBACK
ICS_Game::windowMessageCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ICS_GameInstances[hWnd])
	{
		return ICS_GameInstances[hWnd]->processWindowMessage(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

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
LRESULT CALLBACK
ICS_Game::processWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// activate messages
	if (uMsg == WM_ACTIVATE)
	{
		if (!HIWORD(wParam))					// Check Minimization State
		{
			_active = true;						// Program Is Active
		}
		else
		{
			_active = false;					// Program Is No Longer Active
		}
	}

	// prevent screensaver and monitor powersave mode
	else if (uMsg == WM_SYSCOMMAND and (wParam == SC_SCREENSAVE || SC_SCREENSAVE == SC_MONITORPOWER))
	{
		return 0;
	}

	// handle close messages (close the window)
	else if (uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}

	// key press
	else if (uMsg == WM_KEYDOWN)
	{
		// make sure the state of the key was up
		if (_keys[wParam] < 0)
		{
			// set the key state to down
			_keys[wParam] = clock();

			// invoke the callback
			if (_keyboardEventCallback)
			{
				_keyboardEventCallback(wParam, ICS_EVENT_PRESS);
			}

			// notify all event listeners of the event
			for (int i = _keyboardEventListeners.size() - 1; i >= 0; i--)
			{
				_keyboardEventListeners[i]->handleKeyboardEvent(wParam, ICS_EVENT_PRESS);
			}
		}
	}

	// key release
	else if (uMsg == WM_KEYUP)
	{
		// make sure the key state was down
		if (_keys[wParam] >= 0)
		{
			// set the key state to up
			_keys[wParam] = -1;

			// invoke the callback
			if (_keyboardEventCallback)
			{
				_keyboardEventCallback(wParam, ICS_EVENT_RELEASE);
			}

			// notify all event listeners of the event
			for (int i = _keyboardEventListeners.size() - 1; i >= 0; i--)
			{
				_keyboardEventListeners[i]->handleKeyboardEvent(wParam, ICS_EVENT_RELEASE);
			}
		}
	}

	// window resize
	else if (uMsg == WM_SIZE)
	{
		updateViewport(LOWORD(lParam), HIWORD(lParam));

		// record the new window dimensions when not in full screen mode
		if (not _fullScreen)
		{
			_windowedModeWindowWidth = LOWORD(lParam);
			_windowedModeWindowHeight = HIWORD(lParam);
		}
	}

	// mouse button
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP)
	{
		// determine which button triggered the event
		int mouseButton = ICS_LEFT_MOUSE_BUTTON;

		if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP)
		{
			mouseButton = ICS_RIGHT_MOUSE_BUTTON;
		}

		if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP)
		{
			mouseButton = ICS_MIDDLE_MOUSE_BUTTON;
		}

		// get the mouse coordinates
		int mouseX = GET_X_LPARAM(lParam);
		int mouseY = GET_Y_LPARAM(lParam);

		// determine the type of event (press or release)
		int eventType = (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN) ? ICS_EVENT_PRESS : ICS_EVENT_RELEASE;

		// capture all mouse events, even outside the client window
		if (eventType == ICS_EVENT_PRESS)
		{
			SetCapture(_windowHandle);
		}
		else
		{
			ReleaseCapture();
		}

		// invoke the callback
		if (_mouseButtonEventCallback)
		{
			_mouseButtonEventCallback(mouseButton, mouseX, mouseY, eventType);
		}

		// notify all event listeners of the event
		for (int i = _mouseButtonEventListeners.size() - 1; i >= 0; i--)
		{
			_mouseButtonEventListeners[i]->handleMouseButtonEvent(mouseButton, mouseX, mouseY, eventType);
		}

		// the mouse button went down?
		if (eventType == ICS_EVENT_PRESS)
		{
			// notify the root node of the event
			_rootNode->handleMousePressOver(mouseButton, mouseX, mouseY);
		}

		// the mouse button was released?
		if (mouseButton == ICS_LEFT_MOUSE_BUTTON and eventType == ICS_EVENT_RELEASE)
		{
			_rootNode->handleMouseClick(mouseButton, mouseX, mouseY);
		}
	}

	// mouse movement
	else if (uMsg == WM_MOUSEMOVE)
	{
		// get the mouse coordinates
		int mouseX = GET_X_LPARAM(lParam);
		int mouseY = GET_Y_LPARAM(lParam);

		// make sure the callback is set
		if (_mouseMoveEventCallback)
		{
			// invoke the callback
			_mouseMoveEventCallback(mouseX, mouseY);
		}

		// notify all event listeners of the event
		for (int i = _mouseMoveEventListeners.size() - 1; i >= 0; i--)
		{
			_mouseMoveEventListeners[i]->handleMouseMove(mouseX, mouseY);
		}

		// notify the root node of the event
		_rootNode->handleMouseMoveOver(mouseX, mouseY);
	}

	// vertical mouse wheel scroll
	else if (uMsg == WM_MOUSEWHEEL)
	{
		// convert the screen coordinates to client coordinates
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		ScreenToClient(hWnd, &pt);

		// get the mouse coordinates
		int x = pt.x;
		int y = pt.y;
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		// invoke the callback
		if (_mouseWheelEventCallback)
		{
			_mouseWheelEventCallback(x, y, zDelta);
		}

		// notify the root node of the event
		_rootNode->handleMouseWheelOver(x, y, zDelta);
	}

	// pass all unhandled messages to DefWindowProc
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

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
bool
ICS_Game::createWindow(std::string title, int width, int height, bool fixedSize)
{
	// reset the state of each key
	for (int i = 0; i < 256; i++)
	{
		_keys[i] = -1;
	}

	int			PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style

	RECT WindowRect;						// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left		=	0L;				// Set Left Value To 0
	WindowRect.right	=	(long)width;	// Set Right Value To Requested Width
	WindowRect.top		=	0L;				// Set Top Value To 0
	WindowRect.bottom	=	(long)height;	// Set Bottom Value To Requested Height

	_moduleHandle		= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC)windowMessageCallback;		// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= _moduleHandle;						// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	// attempt to register the window class
	if (not RegisterClass(&wc))
	{
		ICS_LOG_ERROR("Failed to register the window class.");
		return false;
	}
	
	// attempt full screen mode
	if (_fullScreen)
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			ICS_LOG_ERROR("Fullscreen mode is not supported by your video card.");
			_fullScreen = false;
		}
	}

	// check for full screen mode
	if (_fullScreen)
	{
		dwExStyle = WS_EX_APPWINDOW;	// Window Extended Style
		dwStyle = WS_POPUP;				// Windows Style
		ShowCursor(false);				// Hide Mouse Pointer
	}
	else
	{
		// Window Extended Style
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		// Windows Style
		if (fixedSize)
		{
			dwStyle = WS_OVERLAPPEDWINDOW - (WS_MAXIMIZEBOX | WS_THICKFRAME);
		}
		else
		{
			dwStyle = WS_OVERLAPPEDWINDOW;
		}
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);		// Adjust Window To True Requested Size

	std::wstring wTitle = std::wstring(title.begin(), title.end());

	// attempt to create the window
	if (!(_windowHandle = CreateWindowEx(	dwExStyle,							// Extended Style For The Window
											"OpenGL",							// Class Name
											title.c_str(),						// Window Title
											dwStyle |							// Defined Window Style
											WS_CLIPSIBLINGS |					// Required Window Style
											WS_CLIPCHILDREN,					// Required Window Style
											0, 0,								// Window Position
											WindowRect.right-WindowRect.left,	// Calculate Window Width
											WindowRect.bottom-WindowRect.top,	// Calculate Window Height
											NULL,								// No Parent Window
											NULL,								// No Menu
											_moduleHandle,						// Instance
											NULL)))								// Dont Pass Anything To WM_CREATE
	{
		destroyWindow(true);
		ICS_LOG_ERROR("Window creation error.");
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	// attempt to create a GL device context
	if (not (_deviceContextHandle = GetDC(_windowHandle)))
	{
		destroyWindow(true);
		ICS_LOG_ERROR("Failed to create a GL device context.");
		return false;
	}

	// attempt to find a suitable pixel format
	if (not (PixelFormat = ChoosePixelFormat(_deviceContextHandle, &pfd)))
	{
		destroyWindow(true);
		ICS_LOG_ERROR("Failed to find a suitable pixel format.");
		return false;
	}

	// attempt to set the pixel format
	if(not SetPixelFormat(_deviceContextHandle, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		destroyWindow(true);
		ICS_LOG_ERROR("Failed to set the pixel format.");
		return false;
	}

	// attempt to create a rendering context if one does not already exist
	if (not _renderContextHandle and not (_renderContextHandle = wglCreateContext(_deviceContextHandle)))
	{
		destroyWindow(true);
		ICS_LOG_ERROR("Failed to create a GL rendering context.");
		return false;
	}

	// attempt to activate the rendering context
	if (not wglMakeCurrent(_deviceContextHandle, _renderContextHandle))
	{
		destroyWindow(true);
		ICS_LOG_ERROR("Failed to activate the GL rendering context.");
		return false;
	}

	ShowWindow(_windowHandle, SW_SHOW);			// Show The Window
	SetForegroundWindow(_windowHandle);			// Slightly Higher Priority
	SetFocus(_windowHandle);					// Sets Keyboard Focus To The Window
	updateViewport(width, height);				// Set Up Our Perspective GL Screen

	ICS_GameInstances[_windowHandle] = this;	// map the window handle to this instance

	// OpenGL initialization
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	_backgroundColor.setClearColor();					// Background Color
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// success
	return true;
}

/**
 * This updates the viewport resolution.  It is called each time the game window is resized.
 *
 * @param width		The width of the window in pixels.
 * @param height	The height of the window in pixels.
 */
void
ICS_Game::updateViewport(int width, int height)
{
	// prevent divide by 0 for the perspective calculations
	if (height == 0)
	{
		height = 1;
	}

	// set the member attributes
	_windowWidth = width;
	_windowHeight = height;

	// update the viewport
	glViewport(0, 0, _windowWidth, _windowHeight);

	// invoke the callback
	if (_resizeWindowEventCallback)
	{
		_resizeWindowEventCallback(_windowWidth, _windowHeight);
	}
}

/**
 * This properly destroys the game window.
 *
 * @param destroyRenderingContext	Indicates the OpenGL rendering context should be destroyed as well.
 */
void
ICS_Game::destroyWindow(bool destroyRenderingContext)
{
	// stop this instance from receiving window messages
	if (_windowHandle)
	{
		ICS_GameInstances[_windowHandle] = NULL;
	}

	// check for full screen mode
	if (_fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);	// switch back to the desktop
		ShowCursor(TRUE);				// show mouse pointer
	}
	
	// check if the render context needs to be destroyed
	if (destroyRenderingContext and _renderContextHandle)
	{
		// attempt to release the device and render contexts
		if (not wglMakeCurrent(NULL, NULL))
		{
			ICS_LOG_ERROR("Failed to release the device and render contexts");
		}

		// attempt to delete the render context
		if (not wglDeleteContext(_renderContextHandle))
		{
			ICS_LOG_ERROR("Failed to delete the render context");
		}

		// set the render context to NULL
		_renderContextHandle = NULL;
	}

	// attempt to release the device context
	if (_deviceContextHandle and not ReleaseDC(_windowHandle, _deviceContextHandle))
	{
		ICS_LOG_ERROR("Failed to release the device context.");
		_deviceContextHandle = NULL;
	}

	// attempt to destroy the window
	if (_windowHandle and not DestroyWindow(_windowHandle))
	{
		ICS_LOG_ERROR("Failed to release the window handle");
		_windowHandle = NULL;
	}

	// attempt to unregister the OpenGL class
	if (!UnregisterClass("OpenGL", _moduleHandle))
	{
		ICS_LOG_ERROR("Failed to unregister the OpenGL class.");
		_moduleHandle = NULL;
	}
}
