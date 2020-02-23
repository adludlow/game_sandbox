#ifndef POLYGONENTITY_HPP
#define POLYGONENTITY_HPP

#include "Entity.hpp"
#include "Collision.hpp"
#include "geometry.hpp"

struct PolygonProjectionResult {
  double min = 0;
  double max = 0;
};

class PolygonEntity : public Entity {
  public:
    PolygonEntity() {}
    PolygonEntity(const point_t& center, double radius, int num_verts);

    ~PolygonEntity() {}

    void render(SDL_Renderer* sdlRenderer) const;
    void move(const point_t& trans_vec);
    Collision colliding(PolygonEntity& poly);
    std::vector<point_t> normals() const;
    PolygonProjectionResult projectPolygon(const point_t& plane) const;


  private:
    std::vector<point_t> points_;
};

#endif
