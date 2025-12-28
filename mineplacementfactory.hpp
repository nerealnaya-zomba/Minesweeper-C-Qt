#pragma once

#include "mineplacementstrategy.hpp"
#include <memory>
#include <QString>
#include <QStringList>

class MinePlacementFactory
{

public:

    enum StrategyType {
        RANDOM,
        NO_SAFE,
        CLUSTERED
    };

public:

    static std::unique_ptr<MinePlacementStrategy> createStrategy(StrategyType type);
    static std::unique_ptr<MinePlacementStrategy> createStrategy(const QString& name);

    static QStringList availableStrategies();
    static QString strategyToString(StrategyType type);
    static StrategyType stringToStrategy(const QString& name);

};
