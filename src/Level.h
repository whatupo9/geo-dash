#ifndef LEVEL_H
#define LEVEL_H

#include "ICS_Text.h" // For ICS_Text class
#include "LevelEnd.h" // For LevelEnd class
#include "Object.h"   // For Object class
#include "Player.h"   // For Player class
#include <fstream>    // For ifstream

class Level
{
  Array<Object*> _objects;   // The objects in the Level
  Player _player = Player(); // The player in the Level
  LevelEnd* _end = nullptr;  // The end of the Level

  std::string _name;   // Name of the Level
  std::ifstream _file; // File with the Level layout

  // Text objects

  ICS_Text _attemptText; // Attempt count at the start of Level
  ICS_Text _endText;     // Instructions at end of Level
  ICS_Text _endText2;    // Attempt count at end of Level

  // Images

  ICS_Sprite _background; // Background of Level
  ICS_Sprite _endMenu;    // Menu at the end of Level

  double _elapsed = 0.0; // How much time, in second, has elapsed since the start
  int _blockCounter = 0; // How many blocks have been rendered, not including the beginning platform

  bool _jumping = false; // Is the player jumping
  bool _atEnd = false;   // It the player at the end
  bool _restart = false; // Did the player choose to restart

public:
  /**
   * Level Constructor
   *
   * @param name:     The name of the Level
   * @param attempts: Which attempt is this
   * @param length:   How many blocks is the level
   */
  Level(std::string name, int attempts, int length);

  // Delete copy constructor
  Level(const Level&) = delete;

  // Delete assignment operator
  Level& operator=(const Level&) = delete;

  // Destructor
  ~Level();

  /**
   * Handles any key presses by the user
   *
   * @param key:        The key id, from ICS_Constants.h
   * @param eventType:  Whether it was a press or release
   */
  void handleKeyPress(int key, int eventType);

  /**
   * Updates the Level
   *
   * @param elapsed: The time since the last update
   *
   * @returns: True if the player died
   */
  bool update(double elapsed);

  /**
   * Loads a columnn from the file
   */
  void loadColumn();
};

#endif //! LEVEL_H
