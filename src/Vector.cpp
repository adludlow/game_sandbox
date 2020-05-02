#include <iostream>
#include <sstream>
#include <math.h>

#include "Vector.hpp"

int Vector::w() const {
  return w_;
}

coord_t Vector::x() const {
  return x_;
}

coord_t Vector::y() const {
  return y_;
}

coord_t Vector::z() const {
  return z_;
}

float Vector::magnitude() const {
  return magnitude_;
}

void Vector::setMagnitude(float mag) {
  magnitude_ = mag;
}

float Vector::calculateMagnitude() const {
  return sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
}

Vector Vector::unit() const {
  float len = sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
  return Vector(x_ / len, y_ / len, z_ / len);
}

Vector Vector::operator - () const {
  return Vector(-x_, -y_);
}

Vector Vector::operator - (const Vector& v) const {
  return Vector(x_ - v.x_, y_ - v.y_);
}

Vector Vector::operator + (const Vector& v) const {
  return Vector(x_ + v.x_, y_ + v.y_);
}

Vector Vector::operator * (float mag) const {
  return Vector(x_ * mag, y_ * mag, z_ * mag, w_);
}

Vector Vector::lhNormal(const Vector& v1, const Vector& v2) {
  Vector edge = v1 - v2;
  return Vector(edge.y_, -edge.x_);
}

Vector Vector::rhNormal(const Vector& v1, const Vector& v2) {
  Vector edge = v1 - v2;
  return Vector(-edge.y_, edge.x_);
}

float Vector::dot(const Vector& v) const {
  return x_ * v.x() + y_ * v.y() + z_ * v.z();
}

std::string Vector::toString() const {
  std::stringstream ss;
  ss << x_ << "," << y_ << "," << z_ << "->" << magnitude_;
  return ss.str();
}

double Vector::cosineSimilarity(Vector& v) const {
  float dp = dot(v);
  float denominator = calculateMagnitude() * v.calculateMagnitude();
  return dp / denominator;
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
  os << v.x_ << " "
     << v.y_ << " "
     << v.z_ << " "
     << v.w_ << " ";

  return os;
}