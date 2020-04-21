#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <memory>
#include <SDL.h>

#include "Polygon.hpp"
#include "Vector.hpp"
#include "input/InputObserver.hpp"
#include "util.hpp"
#include "World.hpp"

enum class MovementDirection { Forwards, Backwards, Stationary };
enum class RotateDirection { Clockwise, AntiClockwise, None };

class GameEntity: public InputObserver {
  public:
    GameEntity() : 
      polygon_(Polygon()),
      heading_(Vector()),
      type_(""),
      id_(util::uuid()) {};

    GameEntity(
      const Polygon& polygon,
      const Vector& heading = Vector(),
      const std::string& type = ""
    ) : 
    polygon_(polygon),
    heading_(heading),
    type_(type),
    id_(util::uuid()) {
      moveDirection_ = MovementDirection::Stationary;
      rotateDirection_ = RotateDirection::None;
      shooting_ = false;
    };

    void update(World& world);

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
    Polygon polygon_;
    Vector heading_;
    std::string type_;
    std::string id_;

    MovementDirection moveDirection_;
    RotateDirection rotateDirection_;
    bool shooting_;

    float rotateAngleDelta = 0.1;
};

typedef std::unique_ptr<GameEntity> GePtr;

#endif
