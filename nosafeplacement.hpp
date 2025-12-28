#pragma once

#include "randommineplacement.hpp"

#include <QString>

class NoSafePlacement : public RandomMinePlacement
{

public:

    NoSafePlacement();

    void placeMines(GameField& field, int mines, const Point& safePoint) override;
    QString getName() const override;

};
