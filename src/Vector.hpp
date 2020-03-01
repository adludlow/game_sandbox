#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>

typedef float coord_t;

class Vector {
  public:
    Vector(coord_t x, coord_t y, coord_t z, int w, float mag) : x_(x), y_(y), z_(z), w_(w), magnitude_(mag) {};
    Vector(coord_t x, coord_t y, coord_t z = 1, coord_t w = 1) : Vector(x, y, z, w, 1) {};
    Vector() : Vector(0, 0, 1, 0, 1) {};

    int w() const;
    coord_t x() const;
    coord_t y() const;
    coord_t z() const;

    float magnitude() const;
    void setMagnitude(float mag);

    static Vector lhNormal(const Vector& v1, const Vector& v2);
    static Vector rhNormal(const Vector& v1, const Vector& v2);

    Vector unit() const;

    std::string toString() const;

    Vector operator - (const Vector& v) const;
    Vector operator + (const Vector& v) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector& v);

  private:
    int w_;
    coord_t x_;
    coord_t y_;
    coord_t z_;
    float magnitude_;
};


#endif
