#pragma once

#include "attemptsplacementstrategy.hpp"
#include "gamefield.hpp"
#include "point.hpp"

#include <QString>

class RandomMinePlacement : public AttemptsPlacementStrategy
{

protected:

    static constexpr int RANDOM_MAX_ATTEMPTS_MULTIPLIER = 2;

public:

    RandomMinePlacement();

    void placeMines(GameField& field, int mines, const Point& safePoint) override;
    QString getName() const override;
    int getMaxAttemptsMultiplier() const override;

};
