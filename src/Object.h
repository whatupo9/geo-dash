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
  int _blockY = 0;

public:
  Object() = default;

  Object(const Vertex& pos, double width, double height, std::string imageFile);

  virtual ~Object() = default;

  virtual bool update(double elapsed, const Array<Object*>& objects);

  virtual double getX() const
  {
    return _image.getX();
  }

  virtual double getY() const
  {
    return _image.getY();
  }

  virtual double getWidth() const
  {
    return _width;
  }

  virtual double getHeight() const
  {
    return _height;
  }

  int getBlockY() const
  {
    return _blockY;
  }

  virtual bool isDeadly() const
  {
    return false;
  }
};

#endif //! OBJECT_H
