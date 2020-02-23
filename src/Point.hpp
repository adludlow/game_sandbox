#ifndef POINT_HPP
#define POINT_HPP

typedef int coord_t;

class Point {
  public:
    Point() : x_(0), y_(0) {};
    Point(coord_t x, coord_t y) : x_(x), y_(y) {};

    coord_t x();
    void setX(int x);

    coord_t y();
    void setY(int y);

  private:
    coord_t x_;
    coord_t y_;
};

#endif
