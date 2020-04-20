#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <memory>
#include <SDL.h>

#include "Polygon.hpp"
#include "Vector.hpp"
#include "input/InputObserver.hpp"

enum class MovementDirection { Forwards, Backwards, Stationary };
enum class RotateDirection { Clockwise, AntiClockwise, None };

class GameEntity: public InputObserver {
  public:
    GameEntity() : polygon_(Polygon()), heading_(Vector()), type_("") {
      init();
    };

    GameEntity(const Polygon& polygon, const Vector& heading = Vector(), const std::string& type = "") : polygon_(polygon), heading_(heading), type_(type) {
      init();
    };

    void update();

    void render(SDL_Renderer* renderer, bool normals = false);
    void move(const Vector& trans_vec);
    void move();
    void reverse();
    void rotate(double angle);

    virtual void onNotifyInput(const std::vector<InputEvent>& events);

    Polygon polygon();
    Vector heading() const;
    std::string type();
    std::string id();

    virtual ~GameEntity() {};

  private:
    std::string id_;
    Polygon polygon_;
    Vector heading_;
    std::string type_;

    MovementDirection moveDirection_;
    RotateDirection rotateDirection_;
    bool shooting_;

    float rotateAngleDelta = 0.1;

    void init();
};

typedef std::unique_ptr<GameEntity> GePtr;

#endif
