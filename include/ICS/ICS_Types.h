/*

ICS_Types

	Created: 2020-03-26

	Change log:

		2020-03-26
			- created ICS_Types to store the various types
			- added function pointer typedefs for event callbacks

		2022-12-03
			- added event callback for window resize

		2024-01-18
			- added event callback for game initialization

		2024-01-25
			- added event callback for text fields
			- added event callback for text prompts

		2024-02-06
			- merged several "event" callback functions into one

		2024-06-04
			- mouse coordinates and screen dimensions are now float instead of int

*/

#pragma once

class ICS_Renderable;

// function pointer typedefs
typedef void(*ICS_UpdateEventFunction)(float);
typedef void(*ICS_Render2DEventFunction)();
typedef void(*ICS_Render3DEventFunction)();
typedef void(*ICS_KeyboardEventFunction)(int, int);
typedef void(*ICS_MouseMoveEventFunction)(float, float);
typedef void(*ICS_MouseButtonEventFunction)(int, float, float, int);
typedef void(*ICS_MouseWheelEventFunction)(float, float, int);
typedef void(*ICS_InitializeEventFunction)();
typedef void(*ICS_ExitEventFunction)();
typedef void(*ICS_ResizeWindowEventFunction)(float, float);
typedef void(*ICS_TextPromptEventFunction)(std::string);
typedef void(*ICS_EventFunction)(int);
typedef void(*ICS_RenderableEventFunction)(ICS_Renderable*, int);