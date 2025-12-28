#include "clusteredplacement.hpp"
#include "mineplacementfactory.hpp"
#include "nosafeplacement.hpp"
#include "randommineplacement.hpp"

std::unique_ptr<MinePlacementStrategy> MinePlacementFactory::createStrategy(StrategyType type) {
    switch (type) {
    case RANDOM:
        return std::make_unique<RandomMinePlacement>();
    case NO_SAFE:
        return std::make_unique<NoSafePlacement>();
    case CLUSTERED:
        return std::make_unique<ClusteredPlacement>();
    default:
        return std::make_unique<RandomMinePlacement>(); // fallback
    }
}

std::unique_ptr<MinePlacementStrategy> MinePlacementFactory::createStrategy(const QString& name) {
    StrategyType type = stringToStrategy(name);
    return createStrategy(type);
}

QStringList MinePlacementFactory::availableStrategies() {
    return {
        strategyToString(RANDOM),
        strategyToString(NO_SAFE),
        strategyToString(CLUSTERED)
    };
}

QString MinePlacementFactory::strategyToString(StrategyType type) {
    switch (type) {
    case RANDOM: return "random";
    case NO_SAFE: return "nosafe";
    case CLUSTERED: return "clustered";
    default: return "random";
    }
}

MinePlacementFactory::StrategyType MinePlacementFactory::stringToStrategy(const QString& name) {
    if (name == "random") return RANDOM;
    if (name == "nosafe") return NO_SAFE;
    if (name == "clustered") return CLUSTERED;
    return RANDOM; // default
}
