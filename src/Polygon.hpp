#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>

#include "Transform.hpp"
#include "Vector.hpp"

struct PolygonProjectionResult {
  double min = 0;
  double max = 0;
};

class Polygon {
  public:
    Polygon() : vertices_(std::vector<Vector>()) {};
    Polygon(const std::vector<Vector>& vertices);
    Polygon(const Vector& center, double radius, int num_verts);

    std::vector<Vector> vertices() const;
    void setVertices(const std::vector<Vector>& vertices);
    void calculateNormals();
    const std::vector<Vector>& normals() const;
    void calculateCentroid();
    Vector centroid();

    PolygonProjectionResult project(const Vector& plane) const;

    Polygon operator * (const Transform& trans) const;

    friend std::ostream& operator<<(std::ostream& os, const Polygon& v);

  private:
    std::vector<Vector> vertices_;
    std::vector<Vector> normals_;
    Vector centroid_;
};


#endif
