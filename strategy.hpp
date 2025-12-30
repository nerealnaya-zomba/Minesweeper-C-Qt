#pragma once

#include "mineplacementstrategy.hpp"

#include <QString>

class Strategy
{

private:

    QString strategyName;

public:

    Strategy(const QString& strategyName);

    const QString& getName() const;

    std::unique_ptr<MinePlacementStrategy> createPlacementStrategy() const;

    static Strategy random();
    static Strategy noSafe();
    static Strategy clustered();

};
