#ifndef OBSERVABLEINPUTHANDLER_HPP
#define OBSERVABLEINPUTHANDLER_HPP

#include "InputHandler.hpp"
#include "InputObserver.hpp"

class ObservableInputHandler: public InputHandler {
  public:
    virtual ~ObservableInputHandler() {}

    virtual void handleInput() = 0;

    virtual void addObserver(InputObserver* observer) = 0;
    virtual void removeObserver(InputObserver* observer) = 0;
};

#endif
