#ifndef BLOCK_H
#define BLOCK_H

#include "Object.h"

class Block : public Object
{
public:
  Block(const Vertex& pos);
protected:
  Block(const Vertex& pos, double width, double height, std::string file);
};

#endif //! BLOCK_H
