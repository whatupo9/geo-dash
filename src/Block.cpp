#include "Block.h"

Block::Block(const Vertex& pos) :
  Object(pos, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, BLOCK_FILE_NAME)
{
}
