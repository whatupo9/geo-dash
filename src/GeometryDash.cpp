#include "GeometryDash.h"

// Default Constructor
GeometryDash::GeometryDash()
{
  // Attempt to open the level file, and check if it opened
  std::ifstream inFile(_levelName + ".lvl");
  if (not inFile.is_open())
  {
    std::cout << "Could not open " << _levelName << ".lvl\n";
  }
  else
  {
    // Loop through the file and count the lines
    std::string line = "";
    for (; std::getline(inFile, line); _lines++)
      ;

    // Allocate the new level
    _level = new Level(_levelName, _attempts, _lines);
  }
}

// Destructor
GeometryDash::~GeometryDash()
{
  if (_level)
    delete _level;
}

/**
 * Updates the game
 *
 * @param elapsed: The time since the last update
 */
void GeometryDash::update(double elapsed)
{
  // Check if the game is paused after a death

  // If there is more time left on the timer than has passed
  if (_pauseTimer > elapsed)
  {
    // Subtract the time passed and skip update
    _pauseTimer -= elapsed;
    return;
  }
  // If more time has elapsed than time on the timer
  else if (_pauseTimer > 0)
  {
    // Update elapsed
    elapsed -= _pauseTimer;

    restart();
  }

  if (_level->update(elapsed))
    _pauseTimer = DEATH_PAUSE_LENGTH;
}

/**
 * Handles key presses from the user
 *
 * @param key: The key id, from ICS_Constants.h
 * @param eventType: The type of event, either press or release
 */
void GeometryDash::handleKeyEvent(int key, int eventType)
{
  // Send key presses to the level
  _level->handleKeyPress(key, eventType);
}

/**
 * Restarts the level
 */
void GeometryDash::restart()
{
  _pauseTimer = 0.0;
  _attempts++;

  // Deallocate and create a new level
  if (_level)
    delete _level;
  _level = new Level(_levelName, _attempts, _lines);
}
