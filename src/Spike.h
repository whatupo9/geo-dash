#ifndef SPIKE_H
#define SPIKE_H

#include "Object.h"

class Spike : public Object
{
public:
  Spike(const Vertex& pos);

  double getX() const override;
  double getY() const override;
  double getWidth() const override;
  double getHeight() const override;

  bool isDeadly() const override
  {
    return true;
  }
};

#endif //! SPIKE_H
