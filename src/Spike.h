#ifndef SPIKE_H
#define SPIKE_H

#include "Object.h"

class Spike : public Object
{
public:
  Spike(const Vertex& pos);
  bool isDeadly() const override
  {
    return true;
  }
};

#endif //! SPIKE_H
