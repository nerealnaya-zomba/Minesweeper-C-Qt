#pragma once

#include "attemptsplacementstrategy.hpp"

#include <QString>

class ClusteredPlacement : public AttemptsPlacementStrategy
{

private:

    static constexpr int MIN_CLUSTER_SIZE = 3;
    static constexpr int MAX_CLUSTER_SIZE = 5;
    static constexpr int CLUSTER_RADIUS = 1;  // Кластер 3x3 вокруг центра
    static constexpr int MIN_DISTANCE_BETWEEN_CLUSTERS = 0;
    static constexpr int CLUSTER_MAX_ATTEMPTS_MULTIPLIER  = 10;

public:

    ClusteredPlacement();

    void placeMines(GameField& field, int mines, const Point& safePoint) override;
    QString getName() const override;

    int getMaxAttemptsMultiplier() const override;

    std::vector<Point> generateClusterPoints(const Point& center);

    bool isTooCloseToExistingClusters(const Point& point,
                                      const std::vector<Point>& existingClusters) const;

    void placeSingleCluster(GameField& field, const Point& center,
                            int& placedMines, int clusterSize,
                            const Point& safePoint, int safeRadius);

    void placeRemainingMinesRandomly(GameField& field, int& placedMines, int totalMines,
                                     const Point& safePoint, int safeRadius);

};
