#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>
#include <random>
#include <string>

#include <SDL.h>

#include "Vector.hpp"

namespace util {
  double random(double min, double max);

  std::vector<SDL_Point> toSdlPoints(const std::vector<Vector>& vertices);

  std::string uuid();
}

#endif
