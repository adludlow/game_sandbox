#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>
#include <SDL.h>

#include "Vector.hpp"

double random(double min, double max);

std::vector<SDL_Point> toSdlPoints(const std::vector<Vector>& vertices);

#endif
