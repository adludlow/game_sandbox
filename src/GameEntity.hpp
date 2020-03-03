#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL.h>

#include "Polygon.hpp"
#include "Vector.hpp"

class GameEntity {
  public:
    GameEntity() : polygon_(Polygon()) {};
    GameEntity(const Polygon& polygon) : polygon_(polygon) {};

    void render(SDL_Renderer* renderer, bool normals = false);
    void move(const Vector& trans_vec);
    void rotate(double angle);

    const Polygon& polygon() const;

    virtual ~GameEntity() {};

  private:
    Polygon polygon_;
};

#endif
