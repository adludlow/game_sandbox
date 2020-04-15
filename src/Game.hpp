#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <map>

#include "GameEntity.hpp"
#include "Vector.hpp"
#include "Collision.hpp"

typedef float coord_t;
typedef std::pair<std::string, std::string> GoMapKey;

class Game {
  public:
    const int SCREEN_FPS = 60;
    const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

    Game(
      int screenWidth,
      int screenHeight,
      unsigned int maxAsteroids,
      unsigned int asteroidRadius,
      unsigned int asteroidVertCount) : 
      screenWidth_(screenWidth),
      screenHeight_(screenHeight),
      maxAsteroids_(maxAsteroids),
      asteroidRadius_(asteroidRadius),
      asteroidVertCount_(asteroidVertCount),
      window_(nullptr),
      renderer_(nullptr) {};

    GEPtr generateAsteroid(int radius, int numVerts);

    void replenishAsteroids();

    GameEntity* initShip();
    void initBullet();

    template <typename C, typename Oper> 
    void renderFrame(SDL_Renderer* renderer, C& container, Oper op);

    bool init();

    int runGameLoop();

    Collision colliding(GameEntity* e1, GameEntity* e2);
    std::vector<Collision> detectCollisions(const std::vector<GameEntity*>& objects);
    bool inBounds(GameEntity* e);
  private:
    unsigned int maxAsteroids_;
    unsigned int asteroidRaduis_;
    unsigned int asteroidVertCount_;

    std::map<GoMapKey, GePtr> gameObjects_

    std::vector<GePtr> asteroids_;
    GoMapKey shipKey_;
    std::vector<GePtr> bullets_;
    int screenWidth_;
    int screenHeight_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
};

#endif
