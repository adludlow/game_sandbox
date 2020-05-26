#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>

typedef float coord_t;

class Vector {
  public:
    Vector(coord_t x, coord_t y, coord_t z, int w, float mag) : x_(x), y_(y), z_(z), w_(w), magnitude_(mag) {};
    Vector(coord_t x, coord_t y, coord_t z = 0, coord_t w = 1) : Vector(x, y, z, w, 1) {};
    Vector() : Vector(0, 0, 1, 0, 1) {};

    int w() const;
    coord_t x() const;
    coord_t y() const;
    coord_t z() const;

    float magnitude() const;
    void setMagnitude(float mag);

    static Vector lhNormal(const Vector& v1, const Vector& v2);
    static Vector rhNormal(const Vector& v1, const Vector& v2);

    float calculateMagnitude() const;
    float dot(const Vector& v) const;

    Vector unit() const;

    std::string toString() const;

    double cosineSimilarity(Vector& v) const;

    Vector operator - () const;
    Vector operator - (const Vector& v) const;
    Vector operator + (const Vector& v) const;
    Vector operator * (float mag) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector& v);

  private:
    coord_t x_;
    coord_t y_;
    coord_t z_;
    int w_;
    float magnitude_;
};


#endif
