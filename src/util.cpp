#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "util.hpp"

double util::random( double min, double max ) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis( min, max );
  return dis(gen);
}

std::vector<SDL_Point> util::toSdlPoints(const std::vector<Vector>& vertices) {
  std::vector<SDL_Point> sdlPoints;
  for( auto i : vertices ) {
    sdlPoints.push_back({ static_cast<int>(round(i.x())), static_cast<int>(round(i.y()))});
  }
  return sdlPoints;
}

std::string util::uuid() {
  return boost::uuids::to_string(boost::uuids::random_generator()());
}
