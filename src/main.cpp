#include <iostream>
#include <SDL.h>
#include <vector>
#include <math.h>
#include <string>
#include <memory>

#include "constants.hpp"
#include "input/SdlInputHandler.hpp"
#include "Game.hpp"

int main(int argc, char** argv) {
  int num_polygons, num_verts, poly_radius = 0;
  if( argc != 4 ) {
    std::cout << "Usage: game_sandbox <num polygons> <num_verts> <radius>" << std::endl;
    return 0;
  } else {
    num_polygons = std::stoi(argv[1]);
    num_verts = std::stoi(argv[2]);
    poly_radius = std::stoi(argv[3]);
  }

  Game game = Game(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    num_polygons,
    poly_radius,
    num_verts,
    std::make_unique<SdlInputHandler>()
  );
  if (!game.init()) {
    std::cout << "Failed to initialise." << std::endl;
  } else {
    game.runGameLoop();
  }
}
