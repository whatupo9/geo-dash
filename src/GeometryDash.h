#ifndef GEOMETRY_DASH_H
#define GEOMETRY_DASH_H

#include "Array.h"
#include "Object.h"
#include "Player.h"
#include <fstream>

class GeometryDash
{
  Array<Object*> _objects;
  Player _player = Player();
  int _blockCounter = 0;
  double _elapsed = 0.0;
  std::ifstream _levelFile;
  bool _jumping = false;
  double _pauseTimer = 0.0;

public:
  GeometryDash(std::string fileName);
  void update(double elapsed);
  void handleKeyEvent(int key, int eventType);

private:
  void restart();
  void loadColumn();
};

#endif //! GEOMETRY_DASH_H
