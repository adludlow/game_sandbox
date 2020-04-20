#ifndef INPUTOBSERVER_HPP
#define INPUTOBSERVER_HPP

#include <vector>

#include "InputEvent.hpp"

class InputObserver {
  public:
    virtual ~InputObserver() {}

    virtual void onNotifyInput(const std::vector<InputEvent>& events) = 0;
    virtual std::string id() = 0;
};

#endif
