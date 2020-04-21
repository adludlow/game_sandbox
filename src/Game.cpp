#include <iostream>
#include <algorithm>
#include <SDL.h>

#include "Game.hpp"
#include "Timer.hpp"
#include "constants.hpp"

Collision Game::colliding(GameEntity* e1, GameEntity* e2) {
  std::vector<Vector> normals;
  std::vector<Vector> e1Normals = e1->polygon().normals();
  std::vector<Vector> e2Normals = e2->polygon().normals();
  normals.insert(normals.begin(), e1Normals.begin(), e1Normals.end());
  normals.insert(normals.end(), e2Normals.begin(), e2Normals.end());
  double interval = -1000000, min_interval = -1000000, max_interval = 0;
  Vector min_normal, max_normal = Vector();
  for ( auto n : normals ) {
    PolygonProjectionResult p1_res = e1->polygon().project(n);
    PolygonProjectionResult p2_res = e2->polygon().project(n);

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
  return Collision(true, min_interval, max_interval, min_normal.unit(), max_normal.unit(), e1, e2);
}

std::vector<Collision> Game::detectCollisions(const std::vector<GameEntity*>& objects) {
  std::vector<Collision> collisions;
  for( unsigned int i = 0; i < objects.size(); i++ ) {
    for( unsigned int j = i + 1; j < objects.size(); j++ ) {
      Collision collision = colliding(objects[i], objects[j]);
      if( collision.colliding ) {
        collisions.push_back(collision);
      }
    }
  }
  return collisions;
}

GePtr Game::generateAsteroid(int radius, int numVerts) {
  Vector center = Vector(
      util::random(0, screenWidth_),
      util::random(0, screenHeight_),
      0
  );

  return std::make_unique<GameEntity>(Polygon(center, radius, numVerts), Vector(), ASTEROID_GE_TYPE);
}

std::vector<GameEntity*> Game::objectsOfType(const char* const type) {
  std::vector<GameEntity*> objects;
  for (auto it = gameObjects_.cbegin(); it != gameObjects_.cend(); it++) {
    if (type == std::string("ALL") || it->first.first == type) {
      objects.push_back(it->second.get());
    }
  }
  return objects;
}

void Game::replenishAsteroids() {
  std::vector<GameEntity*> asteroids = objectsOfType(ASTEROID_GE_TYPE);

  int spawnCount = maxAsteroids_ - asteroids.size();
  for (int i = 0; i < spawnCount; i++) {
    GePtr asteroid = generateAsteroid(asteroidRadius_, asteroidVertCount_);
    asteroids.push_back(asteroid.get());
    if (detectCollisions(asteroids).size() > 0) {
      asteroids.pop_back();
      i--;
    } else {
      gameObjects_[GoMapKey(ASTEROID_GE_TYPE, asteroid->id())] = std::move(asteroid);
    }
  }
}

void Game::renderFrame() {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer_);
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);

  for (auto& i : gameObjects_) {
    i.second->render(renderer_);
  }

  SDL_RenderPresent(renderer_);
}

bool Game::init() {
  bool success = true;

  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "SDL could not initialise. SDL_Error: %s\n", SDL_GetError() );
    success = false;
  } else {
    window_ = SDL_CreateWindow(
      "SDL Sandbox",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      screenWidth_,
      screenHeight_,
      SDL_WINDOW_SHOWN
    );
    if (window_ == nullptr) {
      printf( "Window could not be created. SDL_Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      renderer_ = SDL_CreateRenderer( window_, -1, SDL_RENDERER_ACCELERATED );
      if (renderer_ == nullptr) {
        printf( "Renderer could not be created. SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
    }
  }

  return success;
}

GameEntity* Game::initShip() {
  Vector center = Vector(
      util::random(0, screenWidth_),
      util::random(0, screenHeight_),
      0
  );

  std::vector<Vector> vertices = {
    Vector(center.x()-25, center.y()+25),
    Vector(center.x(), center.y() - 25),
    Vector(center.x()+25, center.y()+25),
    Vector(center.x()-25, center.y()+25)
  };
  GePtr ship = std::make_unique<GameEntity>(Polygon(vertices), Vector(0, -6, 1, 1), PLAYER_SHIP_GE_TYPE);
  shipKey_ = GoMapKey(PLAYER_SHIP_GE_TYPE, ship->id());
  gameObjects_[shipKey_] = std::move(ship);
  inputHandler_->addObserver(gameObjects_[shipKey_].get());

  return gameObjects_[shipKey_].get();
}

void Game::initBullet() {
  GameEntity* ship = gameObjects_[shipKey_].get();
  Vector origin = ship->polygon().calculateCentroid();
  
  std::vector<Vector> vertices = {
    Vector(origin.x()-1, origin.y()-3),
    Vector(origin.x()+1, origin.y()-3),
    Vector(origin.x()+1, origin.y()),
    Vector(origin.x()-1, origin.y()),
    Vector(origin.x()-1, origin.y()-3)
  };
  GePtr bullet = std::make_unique<GameEntity>(Polygon(vertices), ship->heading(), BULLET_GE_TYPE);
  gameObjects_[GoMapKey(BULLET_GE_TYPE, bullet->id())] = std::move(bullet);
}

bool Game::inBounds(GameEntity* e) {
  for (auto v : e->polygon().vertices()) {
    if (v.x() < 0 || v.x() > screenWidth_ ||
        v.y() < 0 || v.y() > screenHeight_) {
      return false;
    }
  }
  return true;
}

int Game::runGameLoop() {
  running_ = true;
  Timer fpsTimer;
  Timer capTimer;
  int countedFrames = 0;
  fpsTimer.start();
  replenishAsteroids();
  GameEntity* ship = initShip();
  while (running_) {
    capTimer.start();
    float avgFps = countedFrames / ( fpsTimer.getTicks() / 1000.f );
    if( avgFps > 2000000 ) {
      avgFps = 0;
    }

    /*if (shoot) {
      initBullet();
    }

    auto bullets = objectsOfType(BULLET_GE_TYPE);
    for (auto bullet: bullets) {
      bullet->move();
      if (!inBounds(bullet)) {
        gameObjects_.erase(std::make_pair(BULLET_GE_TYPE, bullet->id()));
      }
    }*/

    inputHandler_->handleInput();
    // Add Observer for game

    for (auto& gameObject: gameObjects_) {
      gameObject.second->update();
    }

    std::vector<GameEntity*> allObjects = objectsOfType("ALL");

    std::vector<Collision> collisions;
    collisions = detectCollisions(allObjects);
    for (auto c : collisions) {
      if ((c.entity1->type() == "player_ship" && c.entity2->type() == "asteroid") ||
          (c.entity1->type() == "asteroid" && c.entity2->type() == "player_ship")) {
        // TODO Ship explodes
        Vector mtv = c.min_normal * -c.min_interval;
        ship->move(mtv);
      }
      if ((c.entity1->type() == "bullet" && c.entity2->type() == "asteroid") ||
          (c.entity1->type() == "asteroid" && c.entity2->type() == "bullet")) {
        // Asteroid explodes
        std::string asteroidId = c.entity1->type() == "asteroid" ? c.entity1->id() : c.entity2->id();
        gameObjects_.erase(std::make_pair(ASTEROID_GE_TYPE, asteroidId));
      }
    }

    renderFrame();

    countedFrames++;
    int frameTicks = capTimer.getTicks();
    if( frameTicks < SCREEN_TICKS_PER_FRAME ) {
      SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
    }
  }
  return 0;
}

std::string Game::id() {
  return id_;
}

void Game::onNotifyInput(const std::vector<InputEvent>& events) {
  for (auto event: events) {
    switch (event) {
      case InputEvent::QuitToDesktop:
        running_ = false; 
        break;
      default:
        break;
    }
  }
}
