#ifndef PLAYER_H
#define PLAYER_H

#include "Array.h"
#include "Object.h"

class Player : public Object
{
  double _velocity = 0.0;
  int _jumpFrames = 0;
  bool _onGround = false;

public:
  Player();

  bool update(double elapsed, const Array<Object*>& objects) override;
  void jump();
  bool isOnBlock(const Array<Object*>& objects);
};

#endif //! PLAYER_H
