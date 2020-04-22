#include <iostream>
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
  std::vector<InputEvent> events;
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
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
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
      case SDL_QUIT:
        events.push_back(InputEvent::QuitToDesktop);
        break;
    }
  }

  for (auto observer: observers) {
    observer.second->onNotifyInput(events);
  }
}