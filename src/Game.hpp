#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <map>
#include <string>

#include "GameEntity.hpp"
#include "Vector.hpp"
#include "Collision.hpp"
#include "input/InputObserver.hpp"
#include "input/ObservableInputHandler.hpp"
#include "util.hpp"
#include "World.hpp"

typedef float coord_t;
typedef std::pair<std::string, std::string> GoMapKey;

class Game: public InputObserver {
  public:
    const int SCREEN_FPS = 60;
    const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

    Game(
      World world,
      unsigned int maxAsteroids,
      unsigned int asteroidRadius,
      unsigned int asteroidVertCount,
      std::unique_ptr<ObservableInputHandler> inputHandler) : 
      world_(world),
      maxAsteroids_(maxAsteroids),
      asteroidRadius_(asteroidRadius),
      asteroidVertCount_(asteroidVertCount),
      window_(nullptr),
      renderer_(nullptr),
      inputHandler_(std::move(inputHandler)
    ) {
      id_ = util::uuid();
      running_ = false;
      inputHandler_->addObserver(this);
    };

    GePtr generateAsteroid(int radius, int numVerts);

    void replenishAsteroids();

    GameEntity* initShip();
    void initBullet();

    template <typename C, typename Oper> 
    void renderFrame(SDL_Renderer* renderer, C& container, Oper op);
    void renderFrame();

    bool init();

    int runGameLoop();

    Collision colliding(GameEntity* e1, GameEntity* e2);
    std::vector<Collision> detectCollisions(const std::vector<GameEntity*>& objects);
    std::vector<Collision> detectCollisions(GameEntity* subject, const std::vector<GameEntity*>& objects);
    bool inBounds(GameEntity* e);

    void onNotifyInput(const std::vector<InputEvent>& events);
    std::string id();
  private:
    World world_;
    bool running_;
    std::string id_;
    unsigned int maxAsteroids_;
    unsigned int asteroidRadius_;
    unsigned int asteroidVertCount_;

    std::map<GoMapKey, GePtr> gameObjects_;

    GoMapKey shipKey_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    std::unique_ptr<ObservableInputHandler> inputHandler_;

    std::vector<GameEntity*> objectsOfType(const char* const type);
};

#endif
