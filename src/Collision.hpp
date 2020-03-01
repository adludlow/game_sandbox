#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <ostream>

#include "Vector.hpp"

struct Collision {
  bool colliding = false;
  double min_interval = 0;
  double max_interval = 0;
  Vector min_normal = Vector();
  Vector max_normal = Vector();

  GameEntity* entity1;
  GameEntity* entity2;

  Collision(
    bool coll,
    double min_i
  ) {
    colliding = coll;
    min_interval = min_i;
  }

  Collision(
    bool coll,
    double min_i,
    double max_i,
    Vector min_n,
    Vector max_n,
    GameEntity* e1,
    GameEntity* e2) {
    colliding = coll;
    min_interval = min_i;
    max_interval = max_i;
    min_normal = min_n;
    max_normal = max_n;
    entity1 = e1;
    entity2 = e2;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Collision& c) {
  os << c.colliding << " "
     << c.min_interval << " "
     << c.max_interval << " "
     << c.min_normal << " "
     << c.max_normal;
  return os;
}

#endif
