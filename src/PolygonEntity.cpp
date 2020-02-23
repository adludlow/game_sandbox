#include <iostream>

#include "PolygonEntity.hpp"
#include "geometry.hpp"
#include "util.hpp"
#include "constants.hpp"

PolygonEntity::PolygonEntity(const point_t& center, double radius, int num_verts) {
  std::vector<double> angles;
  for( int i = 0; i < num_verts; i++ ) {
    double angle = random( 0, 1 ) * pi*2;
    angles.push_back(angle);
  }
  std::sort( angles.begin(), angles.end() );

  for ( int i = 0; i < num_verts; i++ ) {
    double x = center.get<0>() + cos( angles.at(i) ) * ( radius );// + random( -2, 2 );
    double y = center.get<1>() + sin( angles.at(i) ) * ( radius );// + random( -2, 2 );
    points_.push_back( { x, y } );
  }
  points_.push_back( points_.at(0) );
}

void PolygonEntity::render(SDL_Renderer* renderer) const {
  std::vector<SDL_Point> sdlPoints = toSdlPoints(points_);
  SDL_RenderDrawLines( renderer, sdlPoints.data(), sdlPoints.size() );
}

std::vector<point_t> PolygonEntity::normals() const {
  std::vector<point_t> normals;
  for ( int i = 0; i < points_.size()-1; i++ ) {
    point_t normal = rhNormal(points_[i], points_[i+1]);
    normals.push_back(normal);
  }
  return normals;
}

PolygonProjectionResult PolygonEntity::projectPolygon(const point_t& plane) const {
  double init = project( points_[0], plane );
  PolygonProjectionResult proj_res = { init, init };
  for ( auto p : points_ ) {
    double projection = project( p, plane );
    if( projection < proj_res.min )
      proj_res.min = projection;
    if( projection > proj_res.max )
      proj_res.max = projection;
  }
  return proj_res;
}

Collision PolygonEntity::colliding(PolygonEntity& poly) {
  std::vector<point_t> normals = this->normals();
  std::vector<point_t> theirNormals = poly.normals();
  normals.insert(normals.end(), theirNormals.begin(), theirNormals.end());
  double interval = -1000000, min_interval = -1000000, max_interval = 0;
  point_t min_normal, max_normal = point_t();
  for ( auto n : normals ) {
    PolygonProjectionResult p1_res = this->projectPolygon(n);
    PolygonProjectionResult p2_res = poly.projectPolygon(n);

    if( p1_res.min < p2_res.min ) {
      interval = p2_res.min - p1_res.max;
    } else {
      interval = p1_res.min - p2_res.max;
    }

    if( interval > 0 ) {
      return { false, interval };
    }

    if( interval > min_interval ) {
      min_interval = interval;
      min_normal = n;
    }
    if( interval < max_interval ) {
      max_interval = interval;
      max_normal = n;
    }
  }
  return Collision(true, min_interval, max_interval, unit(min_normal), unit(max_normal), this, &poly);
}

void PolygonEntity::move(const point_t& trans_vec) {
  std::vector<point_t> new_points = std::vector<point_t>();
  for(auto p : points_){
    new_points.push_back(
      point_t(
        p.get<0>() + trans_vec.get<0>(),
        p.get<1>() + trans_vec.get<1>()
      )
    );
  }
  points_ = new_points;
}
