#include "Platform.h"

Platform::Platform(const Vertex& pos) :
  Block(pos, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK / 2, PLATFORM_FILE_NAME)
{
  _image.setY(pos.second - PIXELS_PER_BLOCK / 4);
}
