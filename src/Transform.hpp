#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "glm/gtc/matrix_transform.hpp"

#include "Vector.hpp"

class Transform {
  public:
    Transform() : transMatrix_(glm::mat4(1.0)) {};

    void scale(const Vector& v);
    void translate(const Vector& v);
    void rotate(float angle, const Vector& v);

    static float dotProduct(const Vector& v1, const Vector& v2);
    static float project(const Vector& v, const Vector& plane);

    std::string toString();

    Vector operator * (const Vector& v) const;

  private:
    glm::mat4 transMatrix_;
};

#endif
