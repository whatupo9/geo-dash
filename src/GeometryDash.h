#ifndef GEOMETRY_DASH_H
#define GEOMETRY_DASH_H

#include "Level.h" // For Level class

// Represents a simple game of Geometry Dash
class GeometryDash
{
  double _pauseTimer = 0.0;                       // How long has the level been paused after an attempt
  int _attempts = 1;                              // Total attemps for this level
  int _lines = 1;                                 // How many lines is the level file
  Level* _level = nullptr;                        // The level being rendered
  std::string _levelName = "data/stereo_madness"; // The name of the level

public:
  // Default Constructor
  GeometryDash();

  // Delete the copy constructor
  GeometryDash(const GeometryDash&) = delete;

  // Delete the assignment operator
  GeometryDash& operator=(const GeometryDash&) = delete;

  // Destructor
  ~GeometryDash();

  /**
   * Updates the game
   *
   * @param elapsed: The time since the last update
   */
  void update(double elapsed);

  /**
   * Handles key presses from the user
   *
   * @param key: The key id, from ICS_Constants.h
   * @param eventType: The type of event, either press or release
   */
  void handleKeyEvent(int key, int eventType);

private:
  /**
   * Restarts the level
   */
  void restart();
};

#endif //! GEOMETRY_DASH_H
