#pragma once

class Point
{
private:

    int x;
    int y;

public:

    Point(int x, int y);

    bool isValid() const;

    int getX() const;
    int getY() const;

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

};
