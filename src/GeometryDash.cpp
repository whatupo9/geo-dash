#include "GeometryDash.h"

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

GeometryDash::~GeometryDash()
{
  if (_level)
    delete _level;
}

void GeometryDash::update(double elapsed)
{
  if (_pauseTimer > elapsed)
  {
    _pauseTimer -= elapsed;
    return;
  }
  else if (_pauseTimer > 0)
  {
    elapsed -= _pauseTimer;

    restart();
  }

  if (_level->update(elapsed))
    _pauseTimer = DEATH_PAUSE_LENGTH;
}

void GeometryDash::handleKeyEvent(int key, int eventType)
{
  _level->handleKeyPress(key, eventType);
}

void GeometryDash::restart()
{
  _pauseTimer = 0.0;
  _attempts++;
  if (_level)
    delete _level;
  _level = new Level(_levelName, _attempts, _lines);
}
