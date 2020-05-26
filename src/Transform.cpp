#include "Transform.hpp"
#include "glm/gtx/string_cast.hpp"

void Transform::translate(const Vector& v) {
  transMatrix_ = glm::translate(transMatrix_, glm::vec3(v.x(), v.y(), v.z()));
}

void Transform::scale(const Vector& v) {
  transMatrix_ = glm::scale(transMatrix_, glm::vec3(v.x(), v.y(), v.z()));
}

void Transform::rotate(float angle, const Vector& v) {
  transMatrix_ = glm::rotate(transMatrix_, angle, glm::vec3(v.x(), v.y(), v.z()));
}

Vector Transform::operator * (const Vector& v) const {
  glm::vec4 transResult = transMatrix_ * glm::vec4(v.x(), v.y(), v.z(), v.w());
  Vector result = Vector(transResult.x, transResult.y, transResult.z);
  return result;
}

std::string Transform::toString() {
  return glm::to_string(transMatrix_);
}

float Transform::dotProduct(const Vector& v1, const Vector& v2) {
  return glm::dot(
    glm::vec4(v1.x(), v1.y(), v1.z(), v1.w()),
    glm::vec4(v2.x(), v2.y(), v2.z(), v2.w())
  );
}

float Transform::project(const Vector& v, const Vector& plane) {
  Vector plane_unit = plane.unit();
  float result = dotProduct(plane_unit, v);
  return result;
}
