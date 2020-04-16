#include "util.hpp"

double random( double min, double max ) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis( min, max );
  return dis(gen);
}

std::vector<SDL_Point> toSdlPoints(const std::vector<Vector>& vertices) {
  std::vector<SDL_Point> sdlPoints;
  for( auto i : vertices ) {
    sdlPoints.push_back({ static_cast<int>(round(i.x())), static_cast<int>(round(i.y()))});
  }
  return sdlPoints;
}
