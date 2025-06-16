#include "Level.h"
#include "Block.h"
#include "Platform.h"
#include "Spike.h"
#include "itos.h"
#include <sstream>

Level::Level(std::string name, int attempts) :
  _name(name),
  _file(name + ".lvl"),
  _background(name + ".png", WINDOW_WIDTH * 6, WINDOW_HEIGHT * 2),
  _attemptText("data/PUSAB___.otf", 44)
{
  _attemptText.setText("Attempt " + itos(attempts));
  _attemptText.setPriority(1000);
  _attemptText.setColor(255, 255, 255);
  _attemptText.setPosition(WINDOW_WIDTH / 2.5, WINDOW_HEIGHT / 4.0);

  for (int i = 0; i <= SCREEN_BLOCKS_WIDTH + 1; ++i)
    _objects.pushBack(new Block(Vertex(PIXELS_PER_BLOCK * i, WINDOW_HEIGHT - PIXELS_PER_BLOCK / 2)));
  _background.setPriority(-999);
}

Level::~Level()
{
  _file.close();
  for (auto i : _objects)
    delete i;
}

bool Level::update(double elapsed)
{
  _attemptText.setX(_attemptText.getX() - SCROLL_SPEED * PIXELS_PER_BLOCK * elapsed);
  _background.setX(_background.getX() - SCROLL_SPEED * elapsed);
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
    return true;

  _elapsed += elapsed;

  if (_elapsed / SECONDS_PER_BLOCK > _blockCounter)
  {
    _blockCounter++;
    loadColumn();
  }

  if (_jumping)
    _player.jump();

  return false;
}

void Level::handleKeyPress(int key, int eventType)
{
  switch (key)
  {
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

void Level::loadColumn()
{
  std::string line = "";
  std::getline(_file, line);
  std::stringstream ss;
  ss << line;

  while (std::getline(ss, line, '|'))
  {
    std::stringstream ss2;
    ss2 << line;
    std::getline(ss2, line, ' ');
    double y = atof(line.c_str()) * PIXELS_PER_BLOCK + PIXELS_PER_BLOCK / 2;
    std::getline(ss2, line, ' ');
    if (line == "block")
      _objects.pushBack(new Block(Vertex(WINDOW_WIDTH + _blockCounter * PIXELS_PER_BLOCK - _elapsed * PIXELS_PER_BLOCK * SCROLL_SPEED + PIXELS_PER_BLOCK, y)));
    else if (line == "spike")
      _objects.pushBack(new Spike(Vertex(WINDOW_WIDTH + _blockCounter * PIXELS_PER_BLOCK - _elapsed * PIXELS_PER_BLOCK * SCROLL_SPEED + PIXELS_PER_BLOCK, y)));
    else if (line == "platform")
      _objects.pushBack(new Platform(Vertex(WINDOW_WIDTH + _blockCounter * PIXELS_PER_BLOCK - _elapsed * PIXELS_PER_BLOCK * SCROLL_SPEED + PIXELS_PER_BLOCK, y)));
    else
      std::cout << "There was an invalid object type in level file.\n\n";
  }
}
