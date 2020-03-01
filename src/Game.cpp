#include <SDL.h>

#include "Game.hpp"
#include "util.hpp"

Collision Game::colliding(GameEntity* e1, GameEntity* e2) {
  std::vector<Vector> normals;
  normals.insert(normals.begin(), e1->polygon().normals().begin(), e1->polygon().normals().end());
  normals.insert(normals.end(), e2->polygon().normals().begin(), e2->polygon().normals().end());
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

std::vector<Collision> Game::detectCollisions(const std::vector<std::unique_ptr<GameEntity>>& objects) {
  std::vector<Collision> collisions;
  for( unsigned int i = 0; i < objects.size(); i++ ) {
    for( unsigned int j = i + 1; j < objects.size(); j++ ) {
      Collision collision = colliding(objects[i].get(), objects[j].get());
      if( collision.colliding ) {
        collisions.push_back(collision);
      }
    }
  }
  return collisions;
}

void Game::initialiseAsteroids(
  int count,
  int radius,
  int num_verts
) {
  for( auto i = 0; i < count; i++ ) {
    Vector center = Vector(
        random(0, screenWidth_),
        random(0, screenHeight_),
        0
    );

    asteroids_.push_back(std::make_unique<GameEntity>(Polygon(center, radius, num_verts)));
    if (detectCollisions(asteroids_).size() > 0) {
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
    op(*i);

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
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          break;
      }
    }
    keystate = SDL_GetKeyboardState(nullptr);
    std::vector<Collision> collisions;
    if (keystate[SDL_SCANCODE_C]) {
      collisions = detectCollisions(asteroids_);
    }
    renderFrame(renderer_, asteroids_, [this](GameEntity& e) { e.render(this->renderer_); });
  }
  return 0;
}
