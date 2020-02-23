#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>

#include "Vector.hpp"

class Polygon {
  public:
    Polygon() : vertices_(std::vector<Vector>()) {};
    Polygon(const std::vector<Vector>& vertices) : vertices_(vertices) {};
    Polygon(const Vector& center, double radius, int num_verts);

    std::vector<Vector> vertices() const;
    void setVertices(const std::vector<Vector>& vertices);
    void calculateNormals();
    std::vector<Vector> normals() const;

  private:
    std::vector<Vector> vertices_;
    std::vector<Vector> normals_;
};

#endif
