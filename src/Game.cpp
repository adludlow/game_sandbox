#include <SDL.h>

#include "Game.hpp"
#include "util.hpp"
#include "Timer.hpp"

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
  float angle = 0.0;
  float angleDelta = 0.1;
  bool rotate = false;
  bool move = false;

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
          if( keystate[SDL_SCANCODE_LEFT] ) {
            angle = angleDelta;
            rotate = true;
          }
          if( keystate[SDL_SCANCODE_RIGHT] ) {
            angle = -angleDelta;
            rotate = true;
          }
          if( keystate[SDL_SCANCODE_UP] ) {
            move = true;
          }
          break;
        case SDL_KEYUP:
          if( !keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] ) {
            rotate = false;
          }
          if( !keystate[SDL_SCANCODE_UP] ) {
            move = false;
          }
          break;
      }
    }
    float avgFps = countedFrames / ( fpsTimer.getTicks() / 1000.f );
    if( avgFps > 2000000 ) {
      avgFps = 0;
    }
  
    std::vector<Collision> collisions;
    if (keystate[SDL_SCANCODE_C]) {
      collisions = detectCollisions(asteroids_);
    }

    // Update Object Phase
    if (rotate) {
      ship_->rotate(angle);
    }

    std::vector<GameEntity*> objectsToRender;
    objectsToRender.push_back(ship_.get());
    renderFrame(renderer_, objectsToRender, [this](GameEntity& e) { e.render(this->renderer_); });

    countedFrames++;
    int frameTicks = capTimer.getTicks();
    if( frameTicks < SCREEN_TICKS_PER_FRAME ) {
      SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
    }
  }
  return 0;
}

void Game::initialiseShip() {
  Vector center = Vector(
      random(0, screenWidth_),
      random(0, screenHeight_),
      0
  );
  std::vector<Vector> vertices = {
    Vector(center.x()-5, center.y()-5),
    Vector(center.x(), center.y() + 5),
    Vector(center.x()+5, center.y()-5),
    Vector(center.x()-5, center.y()-5)
  };
  ship_ = std::make_unique<GameEntity>(Polygon(vertices));
}
