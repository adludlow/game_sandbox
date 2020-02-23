#include "Transform.hpp"
#include "glm/gtx/string_cast.hpp"

void Transform::translate(const Vector& v) {
  transMatrix_ = glm::translate(transMatrix_, glm::vec3(v.x(), v.y(), v.z()));
}

void Transform::scale(const Vector& v) {
  transMatrix_ = glm::scale(transMatrix_, glm::vec3(v.x(), v.y(), v.z()));
}

void Transform::rotate(float angle, const Vector& v) {

}

Vector Transform::operator * (const Vector& v) const {
  glm::vec4 transResult = transMatrix_ * glm::vec4(v.x(), v.y(), v.z(), v.w());
  return Vector(transResult.x, transResult.y, transResult.z);
}

std::string Transform::toString() {
  return glm::to_string(transMatrix_);
}
