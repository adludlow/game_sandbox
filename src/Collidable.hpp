#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

#include "Polygon.hpp"
#include "Collision.hpp"

class Collidable {
  public:
    virtual const Polygon& polygon() const = 0;
};

#endif
