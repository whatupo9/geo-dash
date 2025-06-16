#include "GeometryDash.h"

GeometryDash::GeometryDash()
{
  _level = new Level(_currentLevelName, _attempts);
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
    _pauseTimer = 0.0;
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
  _attempts++;
  if (_level)
    delete _level;
  _level = new Level(_currentLevelName, _attempts);
}
