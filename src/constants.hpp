#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <boost/math/constants/constants.hpp>

const int SCREEN_ORIGIN_X = 0;
const int SCREEN_ORIGIN_Y = 0;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

const int GAMESPACE_ORIGIN_X = 100;
const int GAMESPACE_ORIGIN_Y = 100;
const int GAMESPACE_WIDTH = SCREEN_WIDTH - 100;
const int GAMESPACE_HEIGHT = SCREEN_HEIGHT - 100;

const double pi = boost::math::constants::pi<double>();

#endif

