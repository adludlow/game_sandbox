#ifndef SDLINPUTHANDLER_HPP
#define SDLINPUTHANDLER_HPP

#include <memory>
#include <vector>
#include <map>

#include "ObservableInputHandler.hpp"

class SdlInputHandler: public ObservableInputHandler {
  public:
    SdlInputHandler() {};

    void handleInput();

    virtual void addObserver(InputObserver* observer);
    virtual void removeObserver(InputObserver* observer);

  private:
    std::map<std::string, InputObserver*> observers;
};

#endif
