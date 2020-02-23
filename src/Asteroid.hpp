#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Polygon.hpp"
#include "Entity.hpp"

class Asteroid : public Entity {
  public:
    Asteroid() {};
    Asteroid(const Vector& center, double radius, int num_verts);

    void render(SDL_Renderer* renderer) const;
    void move(const Vector& trans_vec);

  private:

    Polygon polygon_;
};

#endif
