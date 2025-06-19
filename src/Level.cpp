#include "Level.h"
#include "Block.h"
#include "ICS_Game.h"
#include "LevelEnd.h"
#include "Platform.h"
#include "Spike.h"
#include "itos.h"
#include <sstream>

// Delete copy constructor
Level::Level(std::string name, int attempts, int length) :
  _name(name),
  _file(name + ".lvl"),
  _background(name + ".png", WINDOW_WIDTH * 6.0, WINDOW_HEIGHT * 2.0),
  _attemptText("data/PUSAB___.otf", 44),
  _endMenu(LEVEL_COMPLETE_FILE_NAME, END_MENU_WIDTH_PIXELS, END_MENU_HEIGHT_PIXELS),
  _endText("data/PUSAB___.otf", 34),
  _endText2("data/PUSAB___.otf", 44)
{
  // Set up all of the UI

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

  _background.setPriority(-999);

  // Add enough objects to make a starting platform for the player
  for (int i = 0; i <= SCREEN_BLOCKS_WIDTH + 1; ++i)
    _objects.pushBack(new Block(Vertex(PIXELS_PER_BLOCK * i, WINDOW_HEIGHT - PIXELS_PER_BLOCK / 2)));

  // Print error message if the level file couldn't be opened
  if (not _file.is_open())
  {
    std::cout << "Could not open " << name << ".lvl\n";
  }
  // Add the end to the level
  else
  {
    _end = new LevelEnd(Vertex(WINDOW_WIDTH + length * PIXELS_PER_BLOCK + PIXELS_PER_BLOCK, WINDOW_HEIGHT / 2));
  }
}

// Destructor
Level::~Level()
{
  // Close the level file
  _file.close();

  // Deallocate all of the objects
  for (auto i : _objects)
    if (i)
      delete i;

  // Delete the LevelEnd
  if (_end)
    delete _end;
}

/**
 * Handles any key presses by the user
 *
 * @param key:        The key id, from ICS_Constants.h
 * @param eventType:  Whether it was a press or release
 */
void Level::handleKeyPress(int key, int eventType)
{
  switch (key)
  {
    // At any point, if they press esc, stop the game
  case ICS_KEY_ESC:
    ICS_Game::getInstance().stop();
    break;

  // If key is W or the up arrow
  case ICS_KEY_W:
  case ICS_KEY_UP:
    // If they pressed it, then they are jumping
    if (eventType == ICS_EVENT_PRESS)
      _jumping = true;
    // Otherwise they released it, and stopped bouncing
    else
      _jumping = false;
    break;
  case ICS_KEY_SPACE:
    // If they press space at the end of the level, then restart
    if (_atEnd)
    {
      _restart = true;
    }
    // Otherwise, apply jump logic as normal
    else
    {
      if (eventType == ICS_EVENT_PRESS)
        _jumping = true;
      else
        _jumping = false;
      break;
    }
  };

  // If the player is jumping, then queue a jump
  if (_jumping)
    _player.jump();
}

/**
 * Updates the Level
 *
 * @param elapsed: The time since the last update
 *
 * @returns: True if the player died
 */
bool Level::update(double elapsed)
{
  // If the player wants to restart, then pretend that they died
  // The game will then create a new Level
  if (_restart)
    return true;

  // Skip updates if they are waiting at the end
  if (_atEnd)
    return false;

  // Update the end
  _end->update(elapsed, _objects);

  // Move the attempt text and background
  _attemptText.setX(_attemptText.getX() - SCROLL_SPEED * PIXELS_PER_BLOCK * elapsed);
  _background.setX(_background.getX() - BACKGROUND_SCROLL_SPEED_PIXELS * elapsed);

  // Update each object, and remove them if they are off of the screen
  for (int i = 0; i < _objects.getSize(); ++i)
  {
    if (_objects[i]->update(elapsed, _objects))
    {
      delete _objects[i];
      _objects.remove(i);
      i--;
    }
  }

  // If they player died, then return true
  if (_player.update(elapsed, _objects))
    return true;

  // Calculate how far the player is from the end
  double distToEnd = _end->getX() - _player.getX();

  // If they are at the end
  if (distToEnd < _end->getWidth() / 2 + _player.getWidth() / 2)
  {
    _atEnd = true;

    // Show the menu
    _endText.setVisible(true);
    _endText2.setVisible(true);
    _endMenu.setVisible(true);

    // Return false, because the player didn't die
    return false;
  }

  // Track time since level start
  _elapsed += elapsed;

  // If enough time has passed, load another block
  if (_elapsed / SECONDS_PER_BLOCK > _blockCounter)
  {
    _blockCounter++;
    loadColumn();
  }

  // If the player is jumping, queue a jump
  if (_jumping)
    _player.jump();

  return false;
}

/**
 * Loads a columnn from the file
 */
void Level::loadColumn()
{
  // Get the line from the file
  std::string line = "";
  std::getline(_file, line);
  std::stringstream ss;
  ss << line;

  // Get each object from the line
  while (std::getline(ss, line, '|'))
  {
    std::stringstream ss2;
    ss2 << line;
    // Get the position
    std::getline(ss2, line, ' ');
    double x = WINDOW_WIDTH + _blockCounter * PIXELS_PER_BLOCK - _elapsed * PIXELS_PER_BLOCK * SCROLL_SPEED + PIXELS_PER_BLOCK;
    double y = atof(line.c_str()) * PIXELS_PER_BLOCK + PIXELS_PER_BLOCK / 2;
    Vertex pos(x, y);

    // Get the type
    std::getline(ss2, line, ' ');

    // Allocate a new object based on type
    if (line == "block")
      _objects.pushBack(new Block(pos));
    else if (line == "spike")
      _objects.pushBack(new Spike(pos));
    else if (line == "platform")
      _objects.pushBack(new Platform(pos));
    else
      std::cout << "There was an invalid object type in level file.\n\n";
  }
}
