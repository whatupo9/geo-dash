#ifndef LEVEL_H
#define LEVEL_H

#include "ICS_Text.h"
#include "LevelEnd.h"
#include "Object.h"
#include "Player.h"
#include <fstream>

class Level
{
  Array<Object*> _objects;
  Player _player = Player();
  LevelEnd* _end = nullptr;

  std::string _name;
  std::ifstream _file;

  ICS_Text _attemptText;
  ICS_Text _endText;
  ICS_Text _endText2;

  ICS_Sprite _background;
  ICS_Sprite _endMenu;

  double _elapsed = 0.0;
  int _blockCounter = 0;

  bool _jumping = false;
  bool _atEnd = false;
  bool _restart = false;

public:
  Level(std::string name, int attempts);
  Level(const Level&) = delete;
  Level& operator=(const Level&) = delete;
  ~Level();

  void handleKeyPress(int key, int eventType);
  bool update(double elapsed);
  void loadColumn();
};

#endif //! LEVEL_H
