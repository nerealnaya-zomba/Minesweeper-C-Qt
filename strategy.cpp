#include "strategy.hpp"
#include "mineplacementfactory.hpp"

Strategy::Strategy(const QString& strategyName) : strategyName(strategyName) {}

const QString& Strategy::getName() const {
    return strategyName;
}

std::unique_ptr<MinePlacementStrategy> Strategy::createPlacementStrategy() const {
    return MinePlacementFactory::createStrategy(strategyName);
}

Strategy Strategy::random() {
    return Strategy("random");
}

Strategy Strategy::noSafe() {
    return Strategy("nosafe");
}

Strategy Strategy::clustered() {
    return Strategy("clustered");
}
