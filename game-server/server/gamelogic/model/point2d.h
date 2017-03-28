#pragma once

namespace Morphling::Gamelogic {
  class Point2D {
  public:
    Point2D(int x, int y) : x(x), y(y) {}

    bool operator==(const Point2D& other) const {
      return x == other.x && y == other.y;
    }

    Point2D operator+(const Point2D& other) const {
      return Point2D(x + other.x, y + other.y);
    }

    Point2D operator-() const {
      return Point2D(-x, -y);
    }

    Point2D operator-(const Point2D& other) const {
      return *this + (-other);
    }

    bool operator>(const Point2D& other) const {
      return x > other.x && y > other.y;
    }

    bool operator >=(const Point2D& other) const {
      return x >= other.x && y >= other.y;
    }

    bool operator <=(const Point2D& other) const {
      return !(*this > other);
    }

    bool operator<(const Point2D& other) const {
      return !(*this >= other);
    }

    bool obeys_dimensions(const Point2D& dimensions) const {
      return x >= 0 && y >= 0 && x < dimensions.x && y < dimensions.y;
    }

    void translate(const Point2D& other) {
      x += other.x;
      y += other.y;
    }

    Point2D get_translation(const Point2D& other) {
      Point2D view(*this);
      view.translate(other);
      return view;
    }

    int get_x() const { return x; }
    int get_y() const { return y; }
  private:
    int x, y;
  };

}
