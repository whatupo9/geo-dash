#include "Spike.h"

Spike::Spike(const Vertex& pos) :
  Object(pos, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, SPIKE_FILE_NAME)
{
}

double Spike::getX() const
{
  return _image.getX() - SPIKE_HITBOX_OFFSET_X;
}

double Spike::getY() const
{
  return _image.getY() - SPIKE_HITBOX_OFFSET_Y;
}

double Spike::getWidth() const
{
  return SPIKE_HITBOX_WIDTH;
}

double Spike::getHeight() const
{
  return SPIKE_HITBOX_HEIGHT;
}
