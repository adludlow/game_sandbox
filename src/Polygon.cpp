#include <iostream>

#include "Polygon.hpp"
#include "constants.hpp"
#include "util.hpp"

Polygon::Polygon(const Vector& center, double radius, int num_verts) {
  std::vector<double> angles;
  for( int i = 0; i < num_verts; i++ ) {
    double angle = random( 0, 1 ) * pi*2;
    angles.push_back(angle);
  }
  std::sort( angles.begin(), angles.end() );

  for ( int i = 0; i < num_verts; i++ ) {
    coord_t x = center.x() + cos( angles.at(i) ) * ( radius );
    coord_t y = center.y() + sin( angles.at(i) ) * ( radius );
    vertices_.push_back( Vector(x, y, 0) );
  }
  vertices_.push_back( vertices_.at(0) );

  calculateNormals();
}

std::vector<Vector> Polygon::vertices() const {
  return vertices_;
}

void Polygon::setVertices(const std::vector<Vector>& vertices) {
  vertices_ = vertices;
}

void Polygon::calculateNormals() {
  normals_.empty();
  for ( size_t i = 0; i < vertices_.size()-1; i++ ) {
    Vector normal = Vector::rhNormal(vertices_[i], vertices_[i+1]);
    normals_.push_back(normal);
  }
}

const std::vector<Vector>& Polygon::normals() const {
  return normals_;
}

PolygonProjectionResult Polygon::project(const Vector& plane) const {
  double init = Transform::project( vertices_[0], plane );
  PolygonProjectionResult proj_res = { init, init };
  for (auto p : vertices_) {
    double projection = Transform::project( p, plane );
    if( projection < proj_res.min )
      proj_res.min = projection;
    if( projection > proj_res.max )
      proj_res.max = projection;
  }
  return proj_res;
}

Polygon Polygon::operator * (const Transform& trans) const {
  std::vector<Vector> newVerts;
  for (auto v : vertices_) {
    newVerts.push_back(trans * v);
  }

  return Polygon(newVerts);
}

std::ostream& operator<<(std::ostream& os, const Polygon& p) {
  os << "Vertices:" << std::endl;
  for (auto v : p.vertices_) {
    os << v << std::endl;
  }
  os << "Normals:" << std::endl;
  for (auto n : p.normals_) {
    os << n << std::endl;
  }
  return os;
}
