#include "Platform.h"

/**
 * Platform Constructor
 *
 * @param pos: The position of the object on the screen
 */
Platform::Platform(const Vertex& pos) :
  Block(pos, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK / 2, PLATFORM_FILE_NAME)
{
  // Shift the platform a quarter block up
  // This way it will be in the top half of the square
  // Make sure it lines up with full blocks next to it
  _image.setY(pos.second - PIXELS_PER_BLOCK / 4);
}
