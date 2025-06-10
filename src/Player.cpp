#include "Player.h"
#include "Array.h"
#include "Constants.h"

Player::Player() :
  Object(PLAYER_STARTING_POS, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, PLAYER_IMAGE_FILE)
{
}

bool Player::update(double elapsed, const Array<Object*>& objects)
{
  _blockY = (_image.getY() - PIXELS_PER_BLOCK / 2) / PIXELS_PER_BLOCK;
  _onGround = isOnBlock(objects);

  // Jump if on block
  if (_jumpFrames and _onGround)
  {
    _jumpFrames = 0;
    _velocity = JUMP_VELOCITY_PIXELS;
    _image.setY(_image.getY() + _velocity * elapsed);
    return false; // Player didn't die
  }

  _velocity += GRAVITY_PIXELS * elapsed;
  _image.setY(_image.getY() + _velocity * elapsed);

  for (auto i : objects)
  {
    double y = _image.getY();
    double x = _image.getX();

    bool collision = abs(x - i->getX()) < (_width / 2 + i->getWidth() / 2);
    bool beside = _blockY == i->getBlockY();

    if (collision and beside)
      return true;
  }

  bool alive = true;
  if (not _onGround)
  {
    for (auto i : objects)
    {
      double y = _image.getY();
      double x = _image.getX();

      bool above = abs(x - i->getX()) < (_width / 2 + i->getWidth() / 2);
      bool inside = abs(y - i->getY()) < (_height / 2 + i->getHeight() / 2);

      if (above and inside)
      {
        double diff = y - i->getY();
        double shift = _height / 2 + i->getHeight() / 2;

        if (diff < 0)
        {
          shift *= -1;
          alive = false; // They hit their head
        }

        double newY = i->getY() + shift;
        _image.setY(newY);

        _velocity = 0.0;
      }
    }
  }

  if (_jumpFrames)
    _jumpFrames--;

  bool onScreen = (_image.getY() - _height / 2) > WINDOW_HEIGHT;

  return onScreen and alive;
}

void Player::jump()
{
  _jumpFrames = JUMP_FRAMES;
}

bool Player::isOnBlock(const Array<Object*>& objects)
{
  for (auto i : objects)
  {
    double y = _image.getY();
    double x = _image.getX();

    bool above = abs(x - i->getX()) < (_width / 2 + i->getWidth() / 2);
    bool onTop = abs(y - i->getY()) == (_height / 2 + i->getHeight() / 2);
    if (above and onTop)
      return true;
  }
  return false;
}
