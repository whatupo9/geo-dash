#include "Constants.h"
#include "GeometryDash.h"
#include "ICS_Game.h"

GeometryDash gd;

void update(float elapsed)
{
  gd.update(elapsed);
}

void handleKeyboardEvent(int key, int eventType)
{
  gd.handleKeyEvent(key, eventType);
}

/**
 * This is the entry point to the program.
 *
 * @param hInstance			A handle to the current instance of the application.
 * @param hPrevInstance		A handle to the previous instance of the application.
 * @param lpCmdLine			The command line for the application, excluding the program name.
 * @param nCmdShow			Controls how the window will be shown.
 *
 *@returns					If the function succeeds, terminating when it receives a WM_QUIT message, it should return the exit value contained
 *							in that message's wParam parameter.  If the function terminates before entering the message loop, it should return zero.
 */
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
  // create the game and set the callbacks
  // ICS_Game::getInstance().setMouseMoveEventCallback(handleMouseMove);          // set the callback for handling mouse move events
  // ICS_Game::getInstance().setMouseWheelEventCallback(handleMouseWheel);        // set the callback for handling mouse wheel events
  // ICS_Game::getInstance().setMouseButtonEventCallback(handleMouseButtonEvent); // set callback for mouse button events
  ICS_Game::getInstance().setKeyboardEventCallback(handleKeyboardEvent);
  ICS_Game::getInstance().setUpdateEventCallback(update);

  // start the game... the program ends when this function returns (when the game loop ends)
  return ICS_Game::getInstance().go("Cube Simulator", WINDOW_WIDTH, WINDOW_HEIGHT);
}
