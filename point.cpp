#include "point.hpp"

Point::Point(int x, int y) : x(x), y(y) {}

bool Point::isValid() const {
    return x >= 0 && y >= 0;
}

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}
