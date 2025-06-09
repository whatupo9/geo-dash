#include "GeometryDash.h"
#include "Block.h"
#include "ICS_Game.h"
#include <sstream>

GeometryDash::GeometryDash(std::string fileName) :
  _levelFile(fileName)
{
  for (int i = 0; i <= SCREEN_BLOCKS_WIDTH + 1; ++i)
    _objects.pushBack(new Block(Vertex(PIXELS_PER_BLOCK * i, WINDOW_HEIGHT - PIXELS_PER_BLOCK)));
}

void GeometryDash::update(double elapsed)
{
  Array<int> toRemove;
  for (int i = 0; i < _objects.getSize(); ++i)
    if (_objects[i]->update(elapsed, _objects))
      toRemove.pushBack(i);

  for (auto i : toRemove)
  {
    delete _objects[i];
    _objects.remove(i);
  }

  if (_player.update(elapsed, _objects))
    restart();

  _elapsed += elapsed;

  if (_elapsed / SECONDS_PER_BLOCK > _blockCounter)
  {
    loadColumn();
    _blockCounter++;
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
  ICS_Game::getInstance().stop();
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
      _objects.pushBack(new Block(Vertex(WINDOW_WIDTH + _blockCounter * PIXELS_PER_BLOCK - _elapsed * PIXELS_PER_BLOCK * SCROLL_SPEED + PIXELS_PER_BLOCK, y)));
    else
      std::cout << "There was an invalid object type in level file.\n\n";
  }
}
