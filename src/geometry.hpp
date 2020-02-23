#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>
#include <boost/math/constants/constants.hpp>
#include <SDL.h>

namespace geom = boost::geometry;
namespace trans = geom::strategy::transform;

typedef geom::model::point<double, 2, boost::geometry::cs::cartesian> point_t;

std::ostream& operator<<(std::ostream& os, const point_t& p);

point_t operator - (const point_t& p1, const point_t& p2);

point_t operator + (const point_t& p1, const point_t& p2);

/*
 * rhNormal: calculates the right hand normal for an edge spanning 2 points.
*/
point_t rhNormal(const point_t& p1, const point_t& p2);

/*
 * lhNormal: calculates the left hand normal for an edge spanning 2 points.
*/
point_t lhNormal(const point_t& p1, const point_t& p2);

double dotProduct(const point_t& v1, const point_t& v2);

double magnitude(const point_t& vector);

point_t unit(const point_t& vector);

/* projects vector onto plane. Converts plane into a unit vector. */
double project(const point_t& vector, const point_t& plane);

std::vector<SDL_Point> toSdlPoints(const std::vector<point_t>& points);

#endif
