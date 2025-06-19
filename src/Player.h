#ifndef PLAYER_H
#define PLAYER_H

#include "Array.h"  // For Array class
#include "Object.h" // For Object class

// Represents a player in a Level
class Player : public Object
{
  double _velocity = 0.0; // Current y velocity, in pixels per second
  int _jumpFrames = 0;    // Current frames left in the jump buffer
  bool _onGround = false; // Is the Player on the ground

public:
  // Default Constructor
  Player();

  /**
   * Updates the player
   *
   * @param elapsed: How much time since the last update call, in seconds
   * @param objects: The array of objects in the game
   */
  bool update(double elapsed, const Array<Object*>& objects) override;

  /**
   * Queues a jump
   */
  void jump();
};

#endif //! PLAYER_H
