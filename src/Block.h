#ifndef BLOCK_H
#define BLOCK_H

#include "Object.h"

class Block : public Object
{
public:
  /**
   * Public Constructor
   *
   * @param pos: The position of the object on the screen
   */
  Block(const Vertex& pos);

protected:
  /**
   * Protected Constructor
   * Allow children to explicitly define width, height, and image
   *
   * @param pos:    The position of the object on the screen
   * @param width:  The width of the image
   * @param height: The height of the image
   * @param file:   The image to load
   */
  Block(const Vertex& pos, double width, double height, std::string file);
};

#endif //! BLOCK_H
