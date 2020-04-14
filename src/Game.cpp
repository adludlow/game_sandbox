#include <iostream>
#include <algorithm>
#include <SDL.h>

#include "Game.hpp"
#include "util.hpp"
#include "Timer.hpp"

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

std::vector<GameEntity*> getRawGEPointers(const std::vector<std::unique_ptr<GameEntity>>& unique_ptrs) {
  std::vector<GameEntity*> ptrs;
  for (auto p = unique_ptrs.begin(); p != unique_ptrs.end(); p++) {
    ptrs.push_back(p->get());
  }
  return ptrs;
}

void Game::initialiseAsteroids(
  unsigned int count,
  int radius,
  int num_verts
) {
  while(asteroids_.size() < count) {
    Vector center = Vector(
        random(0, screenWidth_),
        random(0, screenHeight_),
        0
    );

    asteroids_.push_back(std::make_unique<GameEntity>(Polygon(center, radius, num_verts), Vector(), "asteroid"));
    std::vector<GameEntity*> asteroids = getRawGEPointers(asteroids_);
    if (detectCollisions(asteroids).size() > 0) {
      asteroids_.pop_back();
    }
  }
}

template <typename C, typename Oper> 
void Game::renderFrame(SDL_Renderer* renderer, C& container, Oper op) {
  SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
  SDL_RenderClear( renderer );
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, SDL_ALPHA_OPAQUE );

  for(auto& i : container)
    op(i);

  SDL_RenderPresent( renderer );
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

int Game::runGameLoop() {
  bool quit = false;
  SDL_Event e;
  const Uint8 *keystate = nullptr;
  float angle = 0.0;
  float angleDelta = -0.1;
  bool rotate = false;
  bool move_forward = false;
  bool move_reverse = false;
  bool shoot = false;

  Timer fpsTimer;
  Timer capTimer;
  int countedFrames = 0;
  fpsTimer.start();

  while (!quit) {
    capTimer.start();
    while (SDL_PollEvent(&e) != 0) {
      keystate = SDL_GetKeyboardState(NULL);
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_KEYDOWN:
          if (keystate[SDL_SCANCODE_LEFT]) {
            angle = angleDelta;
            rotate = true;
          }
          if (keystate[SDL_SCANCODE_RIGHT]) {
            angle = -angleDelta;
            rotate = true;
          }
          if (keystate[SDL_SCANCODE_UP]) {
            move_forward = true;
          }
          if (keystate[SDL_SCANCODE_DOWN]) {
            move_reverse = true;
          }
          if (keystate[SDL_SCANCODE_SPACE]) {
            shoot = true;
          }
          break;
        case SDL_KEYUP:
          if( !keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] ) {
            rotate = false;
          }
          if( !keystate[SDL_SCANCODE_UP] ) {
            move_forward = false;
            move_reverse = false;
          }
          if (!keystate[SDL_SCANCODE_SPACE]) {
            shoot = false;
          }
          break;
      }
    }

    float avgFps = countedFrames / ( fpsTimer.getTicks() / 1000.f );
    if( avgFps > 2000000 ) {
      avgFps = 0;
    }
  
    // Update Object Phase
    if (rotate) {
      ship_->rotate(angle);

      if (!inBounds(ship_.get())) {
        ship_->rotate(-angle);
      }
    }

    if (move_forward) {
      ship_->move();

      if (!inBounds(ship_.get())) {
        ship_->reverse();
      }
    }

    if (move_reverse) {
      ship_->reverse();

      if (!inBounds(ship_.get())) {
        ship_->move();
      }
    }

    if (shoot) {
      bullets_.push_back(initBullet());
    }
    for (auto bullet = bullets_.begin(); bullet != bullets_.end(); bullet++) {
      (*bullet)->move();
      if (!inBounds((*bullet).get())) {
        bullets_.erase(bullet--);
      }
    }

    std::vector<GameEntity*> gameObjects = getRawGEPointers(asteroids_);
    gameObjects.push_back(ship_.get());
    std::vector<GameEntity*> bullets_tmp = getRawGEPointers(bullets_);
    gameObjects.insert(gameObjects.end(), bullets_tmp.begin(), bullets_tmp.end());

    std::vector<Collision> collisions;
    collisions = detectCollisions(gameObjects);
    for (auto c : collisions) {
      if ((c.entity1->type() == "player_ship" && c.entity2->type() == "asteroid") ||
          (c.entity1->type() == "asteroid" && c.entity2->type() == "player_ship")) {
        // TODO Ship explodes
        Vector mtv = c.min_normal * -c.min_interval;
        ship_->move(mtv);
      }
      if ((c.entity1->type() == "bullet" && c.entity2->type() == "asteroid") ||
          (c.entity1->type() == "asteroid" && c.entity2->type() == "bullet")) {
        // Asteroid explodes
        std::string asteroidId = c.entity1->type() == "asteroid" ? c.entity1->id() : c.entity2->id();
        asteroids_.erase(std::find_if(asteroids_.begin(), asteroids_.end(), [asteroidId] (std::unique_ptr<GameEntity>& ge) { return ge->id() == asteroidId; }));
      }
    }

    renderFrame(renderer_, gameObjects, [this](GameEntity* e) { e->render(this->renderer_, false); });

    countedFrames++;
    int frameTicks = capTimer.getTicks();
    if( frameTicks < SCREEN_TICKS_PER_FRAME ) {
      SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
    }
  }
  return 0;
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

std::unique_ptr<GameEntity> Game::initBullet() {
  Vector origin = ship_->polygon().calculateCentroid();
  
  std::vector<Vector> vertices = {
    Vector(origin.x()-1, origin.y()-3),
    Vector(origin.x()+1, origin.y()-3),
    Vector(origin.x()+1, origin.y()),
    Vector(origin.x()-1, origin.y()),
    Vector(origin.x()-1, origin.y()-3)
  };
  return std::make_unique<GameEntity>(Polygon(vertices), ship_->heading(), "bullet");
}

void Game::initialiseShip() {
  Vector center = Vector(
      random(0, screenWidth_),
      random(0, screenHeight_),
      0
  );

  std::vector<Vector> vertices = {
    Vector(center.x()-25, center.y()+25),
    Vector(center.x(), center.y() - 25),
    Vector(center.x()+25, center.y()+25),
    Vector(center.x()-25, center.y()+25)
  };
  ship_ = std::make_unique<GameEntity>(Polygon(vertices), Vector(0, -6, 1, 1), "player_ship");
}
