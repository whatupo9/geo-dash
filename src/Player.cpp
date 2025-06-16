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

  _onGround = false;
  for (auto i : objects)
  {
    double x = _image.getX();
    double y = _image.getY();

    double xDiff = x - i->getX();
    double yDiff = y - i->getY();

    bool xCollide = (abs(xDiff) < (_width / 2 + i->getWidth() / 2));
    bool yCollide = (abs(yDiff) < (_height / 2 + i->getHeight() / 2));

    if (xCollide and yCollide)
    {
      // Hit head
      if (yDiff > 0)
      {
        return true;
      }

      double xDest = i->getX() - (_width / 2 + i->getWidth() / 2);
      double xToMove = abs(xDest - x);

      double yDest = i->getY() - (_height / 2 + i->getHeight() / 2);
      double yToMove = abs(yDest - y);

      // Hit spike
      if (i->isDeadly())
      {
        return true;
      }
      // Hit wall
      if (xToMove < yToMove)
      {
        return true;
      }
      else
      {
        _onGround = true;
        _image.setY(yDest);
        _velocity = 0.0;
      }
    }
  }

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

  if (_jumpFrames)
    _jumpFrames--;

  bool onScreen = (_image.getY() - _height / 2) > WINDOW_HEIGHT;

  return onScreen;
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

void Player::reset()
{
  _image.setX(PLAYER_STARTING_POS.first);
  _image.setY(PLAYER_STARTING_POS.second);
  _velocity = 0.0;
  _jumpFrames = 0;
  _onGround = false;
}
