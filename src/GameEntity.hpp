#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL.h>

#include "Polygon.hpp"
#include "Vector.hpp"

class GameEntity {
  public:
    GameEntity() : polygon_(Polygon()), heading_(Vector()) {};
    GameEntity(const Polygon& polygon, const Vector& heading = Vector()) : polygon_(polygon), heading_(heading) {};

    void render(SDL_Renderer* renderer, bool normals = false);
    void move(const Vector& trans_vec);
    void move();
    void reverse();
    void rotate(double angle);

    Polygon polygon();
    Vector heading() const;

    virtual ~GameEntity() {};

  private:
    Polygon polygon_;
    Vector heading_;
};

#endif
