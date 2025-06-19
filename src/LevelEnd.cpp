#include "LevelEnd.h"

/**
 * LevelEnd Constructor
 *
 * @param pos: The position of the spike on the screen
 */
LevelEnd::LevelEnd(const Vertex& pos) :
  Object(pos, PIXELS_PER_BLOCK, WINDOW_HEIGHT)
{
}
