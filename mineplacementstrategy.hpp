#pragma once

#include <QString>

class GameField;
class Point;

class MinePlacementStrategy
{

public:

    virtual ~MinePlacementStrategy() = default;
    virtual void placeMines(GameField& field, int mines, const Point& safePoint) = 0;
    virtual QString getName() const = 0;

};
