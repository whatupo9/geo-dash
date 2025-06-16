#include "Block.h"

Block::Block(const Vertex& pos) :
  Object(pos, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, BLOCK_FILE_NAME)
{
}

Block::Block(const Vertex& pos, double width, double height, std::string file) :
  Object(pos, width, height, file)
{
}
