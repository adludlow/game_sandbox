#include <iostream>

#include "GameEntity.hpp"
#include "Transform.hpp"
#include "util.hpp"
#include "constants.hpp"

Polygon GameEntity::polygon() const {
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
  std::vector<SDL_Point> sdlPoints = util::toSdlPoints(polygon_.vertices());
  std::vector<SDL_Point> edgeMidpoints;
  for ( auto i = 0lu; i < polygon_.vertices().size()-1; i++ ) {
    int x = (polygon_.vertices()[i].x() + polygon_.vertices()[i+1].x()) / 2;
    int y = (polygon_.vertices()[i].y() + polygon_.vertices()[i+1].y()) / 2;
    edgeMidpoints.push_back({ round(x), round(y) });
  }
  SDL_RenderDrawLines(renderer, sdlPoints.data(), sdlPoints.size());

  if (normals) {
    std::vector<SDL_Point> normalPoints = util::toSdlPoints(polygon_.normals());
    for( auto i = 0lu; i < edgeMidpoints.size(); i++ ) {
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

void GameEntity::onNotifyInput(const std::vector<InputEvent>& events) {
  for (auto event: events) {
    inputPipeline_.push(event);
  }
}

bool GameEntity::inScreenBounds(const World& world) const {
  for (auto v : polygon_.vertices()) {
    if (v.x() < 0 || v.x() > world.screenWidth ||
        v.y() < 0 || v.y() > world.screenHeight) {
      return false;
    }
  }
  return true;
}

GePtr GameEntity::initBullet() {
  Vector origin = polygon().calculateCentroid();
  
  std::vector<Vector> vertices = {
    Vector(origin.x()-1, origin.y()-3),
    Vector(origin.x()+1, origin.y()-3),
    Vector(origin.x()+1, origin.y()),
    Vector(origin.x()-1, origin.y()),
    Vector(origin.x()-1, origin.y()-3)
  };
  return std::make_unique<GameEntity>(Polygon(vertices), heading()*3, BULLET_GE_TYPE);
}

std::vector<GePtr> GameEntity::update(World& world) {
  std::vector<GePtr> childEntities;
  while (!inputPipeline_.empty()) {
    auto event = inputPipeline_.front();
    switch (event) {
      case InputEvent::MoveForwards:
        move();
        if (!inScreenBounds(world)) {
          reverse();
        }
        break;
      case InputEvent::MoveBackwards:
        reverse();
        if (!inScreenBounds(world)) {
          move();
        }
        break;
      case InputEvent::RotateClockwise:
        rotate(rotateAngleDelta_);
        if (!inScreenBounds(world)) {
          rotate(-rotateAngleDelta_);
        }
        break;
      case InputEvent::RotateAntiClockwise:
        rotate(-rotateAngleDelta_);
        if (!inScreenBounds(world)) {
          rotate(rotateAngleDelta_);
        }
        break;
      case InputEvent::Shoot:
        childEntities.push_back(initBullet());
        break;
      default:
        break;
    }
    inputPipeline_.pop();
  }
  return childEntities;
}