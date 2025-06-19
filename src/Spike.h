#ifndef SPIKE_H
#define SPIKE_H

#include "Object.h"

class Spike : public Object
{
public:
  /**
   * Spike Constructor
   *
   * @param pos: The position of the spike on the screen
   */
  Spike(const Vertex& pos);

  // Need to override the get position and dimension functions
  // The spike hitbox and image are not the same
  // So to calculate the right collisions, getters must apply translations
  // before returning values

  /**
   * Gets the image y
   *
   * @returns The image y position
   */
  double getY() const override;

  /**
   * Gets the image width
   *
   * @returns The image width
   */
  double getWidth() const override;

  /**
   * Gets the image height
   *
   * @returns The image height
   */
  double getHeight() const override;

  /**
   * Checks if the object kills the player on contact
   *
   * @returns True if the object is deadly
   */
  bool isDeadly() const override
  {
    return true;
  }
};

#endif //! SPIKE_H
