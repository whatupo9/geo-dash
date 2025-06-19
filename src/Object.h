#ifndef OBJECT_H
#define OBJECT_H

#include "Array.h"      // For Array<> class
#include "Constants.h"  // For Vertex macro
#include "ICS_Sprite.h" // For ICS_Sprite
#include <string>       // For std::string

// Represents an object in the game
class Object
{
protected:
  ICS_Sprite _image; // The sprite rendered on the screen
  double _width;     // Width of the sprite
  double _height;    // Height of the sprite

public:
  // Default Constructor
  Object() = default;

  /**
   * Parameterized Constructor
   *
   * @param pos:        The position of the object on the screen
   * @param width:      The width of the image
   * @param height:     The height of the image
   * @param imageFile:  The image to load
   *                    If left blank, defaults to no image
   */
  Object(const Vertex& pos, double width, double height, std::string imageFile = "blank");

  // Virtual Destructor
  virtual ~Object() = default;

  /**
   * Updates the object
   *
   * @param elapsed: How much time since the last update call, in seconds
   * @param objects: The array of objects in the game
   */
  virtual bool update(double elapsed, const Array<Object*>& objects);

  /**
   * Gets the image x
   *
   * @returns The image x position
   */
  virtual double getX() const
  {
    return _image.getX();
  }

  /**
   * Gets the image y
   *
   * @returns The image y position
   */
  virtual double getY() const
  {
    return _image.getY();
  }

  /**
   * Gets the image width
   *
   * @returns The image width
   */
  virtual double getWidth() const
  {
    return _width;
  }

  /**
   * Gets the image height
   *
   * @returns The image height
   */
  virtual double getHeight() const
  {
    return _height;
  }

  /**
   * Checks if the object kills the player on contact
   *
   * @returns True if the object is deadly
   */
  virtual bool isDeadly() const
  {
    return false;
  }
};

#endif //! OBJECT_H
