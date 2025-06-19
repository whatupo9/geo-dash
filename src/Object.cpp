#include "Object.h"

/**
 * Parameterized Constructor
 *
 * @param pos:        The position of the object on the screen
 * @param width:      The width of the image
 * @param height:     The height of the image
 * @param imageFile:  The image to load
 *                    If left blank, defaults to no image
 */
Object::Object(const Vertex& pos, double width, double height, std::string imageFile) :
  _width(width),
  _height(height),
  _image(imageFile, width, height)
{
  // Hide the image if the user didnt give an image file
  if (imageFile == "blank")
  {
    _image.setVisible(false);
  }

  // Set the image position
  _image.setX(pos.first);
  _image.setY(pos.second);
}

/**
 * Updates the object
 *
 * @param elapsed: How much time since the last update call, in seconds
 * @param objects: The array of objects in the game
 */
bool Object::update(double elapsed, const Array<Object*>& objects)
{
  // Move the image to the left based on delta time
  _image.setX(_image.getX() - SCROLL_SPEED_PIXELS * elapsed);

  // Return true if the image is off of the screen
  return _image.getX() + _width / 2 < 0;
}
