#ifndef LEVEL_END_H
#define LEVEL_END_H

#include "Object.h" // For Object class

// Represent the end of a Level
class LevelEnd : public Object
{
public:
  /**
   * LevelEnd Constructor
   *
   * @param pos: The position of the spike on the screen
   */
  LevelEnd(const Vertex& pos);
};

#endif //! LEVEL_END_H
