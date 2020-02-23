#include "geometry.hpp"

std::ostream& operator<<(std::ostream& os, const point_t& p)
{
    os << p.get<0>() << "," << p.get<1>();
    return os;
}

point_t operator - (const point_t& p1, const point_t& p2) {
  return { p1.get<0>() - p2.get<0>(), p1.get<1>() - p2.get<1>() };
}

point_t operator + (const point_t& p1, const point_t& p2) {
  return { p1.get<0>() + p2.get<0>(), p1.get<1>() + p2.get<1>() };
}

/*
 * rhNormal: calculates the right hand normal for an edge spanning 2 points.
*/
point_t rhNormal(const point_t& p1, const point_t& p2) {
  point_t edge = p1 - p2;
  return { -edge.get<1>(), edge.get<0>() };
}

/*
 * lhNormal: calculates the left hand normal for an edge spanning 2 points.
*/
point_t lhNormal(const point_t& p1, const point_t& p2) {
  point_t edge = p1 - p2;
  return { edge.get<1>(), -edge.get<0>() };
}

double dotProduct(const point_t& v1, const point_t& v2) {
  return geom::dot_product( v1, v2 );
}

double magnitude(const point_t& vector) {
  return sqrt(dotProduct(vector, vector));
}

point_t unit(const point_t& vector) {
  double mag = magnitude(vector);
  return point_t( vector.get<0>()/mag, vector.get<1>()/mag );
}

/* projects vector onto plane. Converts plane into a unit vector. */
double project(const point_t& vector, const point_t& plane) {
  point_t plane_unit = unit(plane);
  return dotProduct( vector, plane_unit );
}
std::vector<SDL_Point> toSdlPoints( const std::vector<point_t>& points ) {
  std::vector<SDL_Point> sdlPoints;
  for( auto i : points ) {
    sdlPoints.push_back({ round(i.get<0>()), round(i.get<1>())});
  }
  return sdlPoints;
}
