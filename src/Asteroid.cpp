#include <iostream>

#include "Asteroid.hpp"
#include "Transform.hpp"
#include "util.hpp"

Asteroid::Asteroid(const Vector& center, double radius, int num_verts) {
  polygon_ = Polygon(center, radius, num_verts);
  polygon_.calculateNormals();
}

void Asteroid::render(SDL_Renderer* renderer) const {
  std::vector<SDL_Point> sdlPoints = toSdlPoints(polygon_.vertices());
  std::vector<SDL_Point> edgeMidpoints;
  for ( int i = 0; i < polygon_.vertices().size()-1; i++ ) {
    int x = (polygon_.vertices()[i].x() + polygon_.vertices()[i+1].x()) / 2;
    int y = (polygon_.vertices()[i].y() + polygon_.vertices()[i+1].y()) / 2;
    edgeMidpoints.push_back({ round(x), round(y) });
  }
  SDL_RenderDrawLines(renderer, sdlPoints.data(), sdlPoints.size());

  std::vector<SDL_Point> normalPoints = toSdlPoints(polygon_.normals());
  for( int i = 0; i < edgeMidpoints.size(); i++ ) {
    // Translate normals to sides of poly
    Transform trans = Transform();
    trans.translate(Vector(edgeMidpoints[i].x, edgeMidpoints[i].y));
    trans.scale(Vector(50.0, 50.0));
    Vector norm = trans * polygon_.normals()[i].unit();
    SDL_RenderDrawLine(renderer, edgeMidpoints[i].x, edgeMidpoints[i].y, norm.x(), norm.y());
  }
}

void Asteroid::move(const Vector& trans_vec) {

}
