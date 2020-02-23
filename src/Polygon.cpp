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
    coord_t x = center.x() + cos( angles.at(i) ) * ( radius );// + random( -2, 2 );
    coord_t y = center.y() + sin( angles.at(i) ) * ( radius );// + random( -2, 2 );
    vertices_.push_back( Vector(x, y, 0) );
  }
  vertices_.push_back( vertices_.at(0) );
}

std::vector<Vector> Polygon::vertices() const {
  return vertices_;
}

void Polygon::setVertices(const std::vector<Vector>& vertices) {
  vertices_ = vertices;
}

void Polygon::calculateNormals() {
  normals_.empty();
  for ( int i = 0; i < vertices_.size()-1; i++ ) {
    Vector normal = Vector::rhNormal(vertices_[i], vertices_[i+1]);
    normals_.push_back(normal);
  }
}

std::vector<Vector> Polygon::normals() const {
  return normals_;
}