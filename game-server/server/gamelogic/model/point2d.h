#pragma once

#include<iostream>

namespace Morphling::Gamelogic {
  class Point2D {
  public:

    Point2D(int x = 0, int y = 0) : x(x), y(y) {}

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
      return x <= other.x && y <= other.y;
    }

    bool operator<(const Point2D& other) const {
      return x < other.x && y < other.y;
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

    friend std::ostream& operator<<(std::ostream& os, const Point2D& point);
    friend std::istream& operator>>(std::istream& is, Point2D& point);
  private:
    int x, y;
  };

  inline std::ostream& operator<<(std::ostream& os, const Point2D& point) {
    os << point.x << ' ' << point.y;
    return os;
  }

  inline std::istream& operator>>(std::istream& is, Point2D& point) {
    is >> point.x >> point.y;
    return is;
  }
}
