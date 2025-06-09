#ifndef OBJECT_H
#define OBJECT_H

#include "Array.h"
#include "Constants.h"
#include "ICS_Sprite.h"
#include <string>

class Object
{
protected:
  ICS_Sprite _image;
  double _width;
  double _height;

public:
  Object() = default;

  Object(const Vertex& pos, double width, double height, std::string imageFile);

  virtual ~Object() = default;

  virtual bool update(double elapsed, const Array<Object*>& objects);

  double getX() const
  {
    return _image.getX();
  }

  double getY() const
  {
    return _image.getY();
  }

  double getWidth() const
  {
    return _width;
  }

  double getHeight() const
  {
    return _height;
  }
};

#endif //! OBJECT_H
