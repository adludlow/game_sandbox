#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "GameEntity.hpp"
#include "Transform.hpp"
#include "util.hpp"

void GameEntity::init() {
  id_ = boost::uuids::to_string(boost::uuids::random_generator()());
}

Polygon GameEntity::polygon() {
  return polygon_;
}

Vector GameEntity::heading() const {
  return heading_;
}

std::string GameEntity::type() {
  return type_;
}

std::string GameEntity::id() {
  return id_;
}

void GameEntity::render(SDL_Renderer* renderer, bool normals) {
  std::vector<SDL_Point> sdlPoints = toSdlPoints(polygon_.vertices());
  std::vector<SDL_Point> edgeMidpoints;
  for ( int i = 0; i < polygon_.vertices().size()-1; i++ ) {
    int x = (polygon_.vertices()[i].x() + polygon_.vertices()[i+1].x()) / 2;
    int y = (polygon_.vertices()[i].y() + polygon_.vertices()[i+1].y()) / 2;
    edgeMidpoints.push_back({ round(x), round(y) });
  }
  SDL_RenderDrawLines(renderer, sdlPoints.data(), sdlPoints.size());

  if (normals) {
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
}

void GameEntity::move(const Vector& trans_vec) {
  Transform trans;
  trans.translate(trans_vec);

  polygon_ = polygon_ * trans;

  polygon_.calculateNormals();
}

void GameEntity::move() {
  Transform trans;
  trans.translate(heading_);

  polygon_ = polygon_ * trans;
}

void GameEntity::reverse() {
  Transform trans;
  trans.translate(-heading_);

  polygon_ = polygon_ * trans;
}

void GameEntity::rotate(double angle) {
  // Translate to origin, rotate, translate back.
  Vector centroid = polygon_.calculateCentroid();
  Transform trans;
  trans.translate(Vector(-centroid.x(), -centroid.y()));

  Transform rotTrans;
  rotTrans.rotate(angle, Vector(0, 0, 1));

  Transform transBack;
  transBack.translate(centroid);
  polygon_ = polygon_ * trans * rotTrans * transBack;

  heading_ = rotTrans * heading_;
}
