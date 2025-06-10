#include "GeometryDash.h"
#include "Block.h"
#include "ICS_Game.h"
#include <sstream>

GeometryDash::GeometryDash(std::string fileName) :
  _levelFile(fileName)
{
  for (int i = 0; i <= SCREEN_BLOCKS_WIDTH + 1; ++i)
    _objects.pushBack(new Block(Vertex(PIXELS_PER_BLOCK * i, WINDOW_HEIGHT - PIXELS_PER_BLOCK / 2)));
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

  for (int i = 0; i < _objects.getSize(); ++i)
  {
    if (_objects[i]->update(elapsed, _objects))
    {
      delete _objects[i];
      _objects.remove(i);
      i--;
    }
  }

  if (_player.update(elapsed, _objects))
    _pauseTimer = DEATH_PAUSE_LENGTH;

  _elapsed += elapsed;

  if (_elapsed / SECONDS_PER_BLOCK > _blockCounter)
  {
    _blockCounter++;
    loadColumn();
  }

  if (_jumping)
    _player.jump();
}

void GeometryDash::handleKeyEvent(int key, int eventType)
{
  switch (key)
  {
  case ICS_KEY_ESC:
    ICS_Game::getInstance().stop();
    break;
  case ICS_KEY_W:
  case ICS_KEY_UP:
  case ICS_KEY_SPACE:
    if (eventType == ICS_EVENT_PRESS)
      _jumping = true;
    else
      _jumping = false;
    break;
  };

  if (_jumping)
    _player.jump();
}

void GeometryDash::restart()
{
}

void GeometryDash::loadColumn()
{
  std::string line = "";
  std::getline(_levelFile, line);
  std::stringstream ss;
  ss << line;

  while (std::getline(ss, line, '|'))
  {
    std::stringstream ss2;
    ss2 << line;
    std::getline(ss2, line, ' ');
    double y = atof(line.c_str()) * PIXELS_PER_BLOCK;
    std::getline(ss2, line, ' ');
    if (line == "block")
      _objects.pushBack(new Block(Vertex(WINDOW_WIDTH + _blockCounter * PIXELS_PER_BLOCK - _elapsed * PIXELS_PER_BLOCK * SCROLL_SPEED + PIXELS_PER_BLOCK, y + PIXELS_PER_BLOCK / 2)));
    else
      std::cout << "There was an invalid object type in level file.\n\n";
  }
}
