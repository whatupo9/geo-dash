#include "Player.h"
#include "Array.h"
#include "Constants.h"

// Default Constructor
Player::Player() :
  Object(PLAYER_STARTING_POS, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, PLAYER_IMAGE_FILE)
{
}

/**
 * Updates the player
 *
 * @param elapsed: How much time since the last update call, in seconds
 * @param objects: The array of objects in the game
 */
bool Player::update(double elapsed, const Array<Object*>& objects)
{
  // Reset their ground state
  _onGround = false;

  // Get the position of the player
  double x = _image.getX();
  double y = _image.getY();

  // Loop through each object and check for collisions
  for (auto i : objects)
  {
    // Find the distance between the object and the player
    double xDiff = x - i->getX();
    double yDiff = y - i->getY();

    // Check if the distance is less than half of the dimensions
    bool xCollide = (abs(xDiff) < (_width / 2 + i->getWidth() / 2));
    bool yCollide = (abs(yDiff) < (_height / 2 + i->getHeight() / 2));

    // Since all blocks are the same size, objects will always collide on both axis
    if (xCollide and yCollide)
    {
      // Hit head
      if (yDiff > 0)
      {
        return true;
      }

      // Calculate the destination after moving player out of block

      double xDest = i->getX() - (_width / 2 + i->getWidth() / 2);
      double yDest = i->getY() - (_height / 2 + i->getHeight() / 2);

      // Find the distance they need to move to their new position

      double xToMove = abs(xDest - x);
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
      // Landed on block
      else
      {
        _onGround = true;

        // Move to new position
        _image.setY(yDest);

        // Stop moving
        _velocity = 0.0;
      }
    }
  }

  // Jump if on block and jump in buffer
  if (_jumpFrames and _onGround)
  {
    // Reset buffer
    _jumpFrames = 0;

    // Set jump velocity
    _velocity = JUMP_VELOCITY_PIXELS;

    // Update position based on velocity
    _image.setY(_image.getY() + _velocity * elapsed);

    // Return to avoid applying gravity on the same frame as a jump
    return false; // Player didn't die
  }

  // Apply gravity
  _velocity += GRAVITY_PIXELS * elapsed;

  // Update position based on velocity
  _image.setY(_image.getY() + _velocity * elapsed);

  // If they didn't jump, decrement the buffer size
  if (_jumpFrames)
    _jumpFrames--;

  // Check if the player fell off of the screen
  bool offScreen = (_image.getY() - _height / 2) > WINDOW_HEIGHT;
  return offScreen;
}

/**
 * Queues a jump
 */
void Player::jump()
{
  _jumpFrames = JUMP_FRAMES;
}
