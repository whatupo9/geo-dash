#include "Object.h"

Object::Object(const Vertex& pos, double width, double height, std::string imageFile) :
  _width(width),
  _height(height),
  _image(imageFile, width, height)
{
  _image.setX(pos.first);
  _image.setY(pos.second);
}

bool Object::update(double elapsed, const Array<Object*>& objects)
{
  _image.setX(_image.getX() - SCROLL_SPEED_PIXELS * elapsed);
  return _image.getX() + _width / 2 < 0;
}
