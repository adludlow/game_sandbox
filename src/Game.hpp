#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>

#include "GameEntity.hpp"
#include "Vector.hpp"
#include "Collision.hpp"

typedef float coord_t;

class Game {
  public:
    const int SCREEN_FPS = 60;
    const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

    Game(int screenWidth, int screenHeight) : 
      screenWidth_(screenWidth),
      screenHeight_(screenHeight),
      window_(nullptr),
      renderer_(nullptr) {};

    void initialiseAsteroids(
      unsigned int count,
      int radius,
      int num_verts
    );

    void initialiseShip();

    template <typename C, typename Oper> 
    void renderFrame(SDL_Renderer* renderer, C& container, Oper op);

    bool init();

    int runGameLoop();

    Collision colliding(GameEntity* e1, GameEntity* e2);
    std::vector<Collision> detectCollisions(const std::vector<GameEntity*>& objects);
    bool inBounds(GameEntity* e);
  private:
    std::vector<std::unique_ptr<GameEntity>> asteroids_;
    std::unique_ptr<GameEntity> ship_;
    int screenWidth_;
    int screenHeight_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
};

#endif
