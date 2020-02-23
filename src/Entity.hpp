#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL.h>

#include "Vector.hpp"

class Entity {
  public:
    virtual void render(SDL_Renderer* renderer) const = 0;
    virtual void move(const Vector& trans_vec) = 0;
    //virtual void rotate() = 0;

    virtual ~Entity() {};
};

#endif
