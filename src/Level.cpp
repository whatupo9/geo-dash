#include "Level.h"
#include "Block.h"
#include "ICS_Game.h"
#include "LevelEnd.h"
#include "Platform.h"
#include "Spike.h"
#include "itos.h"
#include <sstream>

Level::Level(std::string name, int attempts, int length) :
  _name(name),
  _file(name + ".lvl"),
  _background(name + ".png", WINDOW_WIDTH * 6.0, WINDOW_HEIGHT * 2.0),
  _attemptText("data/PUSAB___.otf", 44),
  _endMenu(LEVEL_COMPLETE_FILE_NAME, END_MENU_WIDTH_PIXELS, END_MENU_HEIGHT_PIXELS),
  _endText("data/PUSAB___.otf", 34),
  _endText2("data/PUSAB___.otf", 44),
  _sound()
{
  _track = _sound.streamSound(name + ".mp3");

  _endMenu.setVisible(false);
  _endMenu.setPosition(ICS_Pair<float>(WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0));
  _endMenu.setPriority(1000);

  _endText.setVisible(false);
  _endText.setColor(END_MENU_TEXT_COLOUR);
  _endText.setPosition(ICS_Pair<float>(WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0 + 34));
  _endText.setText("Space-Restart    Escape-Exit");
  _endText.setAnchor(0.5, 0.5);
  _endText.setPriority(1001);

  _endText2.setVisible(false);
  _endText2.setColor(END_MENU_TEXT_COLOUR);
  _endText2.setPosition(ICS_Pair<float>(WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0 - 44));
  _endText2.setText("Attemps " + itos(attempts));
  _endText2.setAnchor(0.5, 0.5);
  _endText2.setPriority(1001);

  _attemptText.setText("Attempt " + itos(attempts));
  _attemptText.setPriority(1000);
  _attemptText.setColor(255, 255, 255);
  _attemptText.setPosition(WINDOW_WIDTH / 2.5, WINDOW_HEIGHT / 4.0);

  for (int i = 0; i <= SCREEN_BLOCKS_WIDTH + 1; ++i)
    _objects.pushBack(new Block(Vertex(PIXELS_PER_BLOCK * i, WINDOW_HEIGHT - PIXELS_PER_BLOCK / 2)));
  _background.setPriority(-999);

  if (not _file.is_open())
  {
    std::cout << "Could not open " << name << ".lvl\n";
  }
  else
  {
    std::string input = "";
    getline(_file, input);
    double x = atof(input.c_str());
    _end = new LevelEnd(Vertex(WINDOW_WIDTH + x * PIXELS_PER_BLOCK + PIXELS_PER_BLOCK, WINDOW_HEIGHT / 2));
  }
}

Level::~Level()
{
  _sound.stopSound(_track);

  _file.close();
  for (auto i : _objects)
    if (i)
      delete i;

  if (_end)
    delete _end;
}

bool Level::update(double elapsed)
{
  if (_restart)
    return true;

  if (_atEnd)
    return false;

  _end->update(elapsed, _objects);
  _attemptText.setX(_attemptText.getX() - SCROLL_SPEED * PIXELS_PER_BLOCK * elapsed);
  _background.setX(_background.getX() - BACKGROUND_SCROLL_SPEED_PIXELS * elapsed);
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

  double distToEnd = _end->getX() - _player.getX();
  if (distToEnd < _end->getWidth() / 2 + _player.getWidth() / 2)
  {
    _atEnd = true;
    _endText.setVisible(true);
    _endText2.setVisible(true);
    _endMenu.setVisible(true);
    return false;
  }

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
  case ICS_KEY_ESC:
    ICS_Game::getInstance().stop();
    break;
  case ICS_KEY_W:
  case ICS_KEY_UP:
    if (eventType == ICS_EVENT_PRESS)
      _jumping = true;
    else
      _jumping = false;
    break;
  case ICS_KEY_SPACE:
    if (_atEnd)
    {
      _restart = true;
    }
    else
    {
      if (eventType == ICS_EVENT_PRESS)
        _jumping = true;
      else
        _jumping = false;
      break;
    }
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
