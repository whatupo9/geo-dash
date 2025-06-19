#include "Spike.h"

/**
 * Spike Constructor
 *
 * @param pos: The position of the spike on the screen
 */
Spike::Spike(const Vertex& pos) :
  Object(pos, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, SPIKE_FILE_NAME)
{
}

/**
 * Gets the image y
 *
 * @returns The image y position
 */
double Spike::getY() const
{
  // Shift up
  return _image.getY() - SPIKE_HITBOX_OFFSET_Y;
}

/**
 * Gets the image width
 *
 * @returns The image width
 */
double Spike::getWidth() const
{
  return SPIKE_HITBOX_WIDTH;
}

/**
 * Gets the image height
 *
 * @returns The image height
 */
double Spike::getHeight() const
{
  return SPIKE_HITBOX_HEIGHT;
}
