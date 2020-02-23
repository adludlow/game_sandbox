#include <iostream>
#include <SDL.h>
#include <vector>
#include <math.h>
#include <string>

#include "constants.hpp"
#include "geometry.hpp"
#include "util.hpp"
#include "Polygon.hpp"
#include "Vector.hpp"
#include "Entity.hpp"
#include "Asteroid.hpp"

typedef geom::model::polygon<point_t> polygon_t;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init() {
  bool success = true;

  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "SDL could not initialise. SDL_Error: %s\n", SDL_GetError() );
    success = false;
  } else {
    gWindow = SDL_CreateWindow(
      "SDL Sandbox",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN
    );
    if( gWindow == NULL ) {
      printf( "Window could not be created. SDL_Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL ) {
        printf( "Renderer could not be created. SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
    }
  }

  return success;
}

template <typename C, typename Oper> 
void renderFrame(SDL_Renderer* renderer, C& container, Oper op) {
  SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
  SDL_RenderClear( renderer );
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, SDL_ALPHA_OPAQUE );

  for(auto& i : container)
    op(*i);

  SDL_RenderPresent( renderer );
}

/*std::vector<Collision> detectCollisions(const std::vector<std::unique_ptr<PolygonEntity>>& polygons) {
  std::vector<Collision> collisions;
  for( int i = 0; i < polygons.size(); i++ ) {
    for( int j = i + 1; j < polygons.size(); j++ ) {
      Collision collision = polygons[i]->colliding(*polygons[j].get());
      if( collision.colliding ) {
        collisions.push_back(collision);
      }
    }
  }
  return collisions;
}*/

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

  if( !init() ) {
    printf( "Failed to initialise.\n" );
  } else {
    SDL_Event e;
    const Uint8 *keystate = NULL;
    bool quit = false;

    std::vector<std::unique_ptr<Asteroid>> asteroids;
    for( auto i = 0; i < num_polygons; i++ ) {
      Vector center = Vector(
          random(SCREEN_ORIGIN_X, SCREEN_WIDTH),
          random(SCREEN_ORIGIN_Y, SCREEN_HEIGHT),
          0
      );
      asteroids.push_back(std::make_unique<Asteroid>(Asteroid(center, poly_radius, num_verts)));  
    }

    bool collisionDetection = false;
    while(!quit) {
      while( SDL_PollEvent( &e ) != 0 ) {
        switch( e.type ) {
          case SDL_QUIT:
            quit = true;
            break;
        }
      }
      keystate = SDL_GetKeyboardState(NULL);
      if(keystate[SDL_SCANCODE_C]) {
        collisionDetection = !collisionDetection;
      }

      renderFrame( gRenderer, asteroids, [](Entity& e) { e.render(gRenderer); });

      /*if(collisionDetection) {
        std::vector<Collision> collisions = detectCollisions(polygons);
        for( auto c : collisions ) {
          point_t trans_vec = unit(c.min_normal);
          c.e1->move(point_t(trans_vec.get<0>() * c.min_interval, trans_vec.get<1>() * c.min_interval));
        }
      }*/
    }
  }
}
