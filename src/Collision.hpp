#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "geometry.hpp"
#include "Entity.hpp"

struct Collision {
  bool colliding = false;
  double min_interval = 0;
  double max_interval = 0;
  point_t min_normal = point_t();
  point_t max_normal = point_t();
  Entity* e1 = nullptr;
  Entity* e2 = nullptr;

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
    point_t min_n,
    point_t max_n,
    Entity* e1,
    Entity* e2) {
    colliding = coll;
    min_interval = min_i;
    max_interval = max_i;
    min_normal = min_n;
    max_normal = max_n;
    this->e1 = e1;
    this->e2 = e2;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Collision& c)
{
    os << c.colliding << " "
       << c.min_interval << " "
       << c.max_interval << " "
       << c.min_normal << " "
       << c.max_normal << " "
       << c.e1 << " "
       << c.e2;
    return os;
}

#endif
