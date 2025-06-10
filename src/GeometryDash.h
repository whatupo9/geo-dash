#ifndef GEOMETRY_DASH_H
#define GEOMETRY_DASH_H

#include "Level.h"

class GeometryDash
{
  double _pauseTimer = 0.0;
  int _attempts = 0;
  Level* _level = nullptr;
  std::string _currentLevelFile = "data/stereo_madness.lvl";

public:
  GeometryDash();
  void update(double elapsed);
  void handleKeyEvent(int key, int eventType);

private:
  void restart();
  void loadColumn();
};

#endif //! GEOMETRY_DASH_H
