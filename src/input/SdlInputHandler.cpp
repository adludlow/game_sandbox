#include <map>
#include <SDL.h>

#include "SdlInputHandler.hpp"
#include "InputEvent.hpp"

void SdlInputHandler::addObserver(InputObserver* observer) {
  if (observer != nullptr) {
    observers[observer->id()] = observer;
  }
}

void SdlInputHandler::removeObserver(InputObserver* observer) {
  if (observer != nullptr) {
    observers.erase(observer->id());
  }
}

void SdlInputHandler::handleInput() {
  SDL_Event e;
  const Uint8 *keystate = nullptr;
  std::vector<InputEvent> events;
  while (SDL_PollEvent(&e) != 0) {
    keystate = SDL_GetKeyboardState(NULL);
    switch (e.type) {
      case SDL_KEYDOWN:
        if (keystate[SDL_SCANCODE_LEFT]) {
          events.push_back(InputEvent::RotateAntiClockwise);
        }
        if (keystate[SDL_SCANCODE_RIGHT]) {
          events.push_back(InputEvent::RotateClockwise);
        }
        if (keystate[SDL_SCANCODE_UP]) {
          events.push_back(InputEvent::MoveForwards);
        }
        if (keystate[SDL_SCANCODE_DOWN]) {
          events.push_back(InputEvent::MoveBackwards);
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
          events.push_back(InputEvent::Shoot);
        }
        break;
      case SDL_KEYUP:
        if (!keystate[SDL_SCANCODE_LEFT]) {
          events.push_back(InputEvent::StopRotating);
        }
        if (!keystate[SDL_SCANCODE_RIGHT]) {
          events.push_back(InputEvent::StopRotating);
        }
        if (!keystate[SDL_SCANCODE_UP]) {
          events.push_back(InputEvent::StopMoving);
        }
        if (!keystate[SDL_SCANCODE_UP]) {
          events.push_back(InputEvent::StopMoving);
        }
        if (!keystate[SDL_SCANCODE_SPACE]) {
          events.push_back(InputEvent::StopShoot);
        }
        break;
    }
  }

  for (auto observer: observers) {
    observer.second->onNotifyInput(events);
  }
}