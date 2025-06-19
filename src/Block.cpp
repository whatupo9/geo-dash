#include "Block.h"

/**
 * Public Constructor
 *
 * @param pos: The position of the object on the screen
 */
Block::Block(const Vertex& pos) :
  Object(pos, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, BLOCK_FILE_NAME)
{
}

/**
 * Protected Constructor
 *
 * @param pos:    The position of the object on the screen
 * @param width:  The width of the image
 * @param height: The height of the image
 * @param file:   The image to load
 */
Block::Block(const Vertex& pos, double width, double height, std::string file) :
  Object(pos, width, height, file)
{
}
