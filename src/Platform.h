#ifndef PLATFORM_H
#define PLATFORM_H

#include "Block.h"

class Platform : public Block
{
public:
  /**
   * Platform Constructor
   *
   * @param pos: The position of the object on the screen
   */
  Platform(const Vertex& pos);
};

#endif //! PLATFORM_H
