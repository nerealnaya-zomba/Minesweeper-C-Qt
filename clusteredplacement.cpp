#include "clusteredplacement.hpp"
#include "gamefield.hpp"

ClusteredPlacement::ClusteredPlacement() : AttemptsPlacementStrategy() {}

void ClusteredPlacement::placeMines(GameField& field, int mines, const Point& safePoint) {

    int placedMines = 0;
    int width = field.getWidth();
    int height = field.getHeight();

    int safeRadius = calculateSafeRadius(width, height);

    std::vector<Point> clusterCenters; // Вектор для хранения центров размещенных кластеров

    int attempts = 0;
    int maxAttempts = width * height * getMaxAttemptsMultiplier();

    while (placedMines < mines && attempts < maxAttempts) {

        // Определяем размер кластера //
        int remainingMines = mines - placedMines;
        int clusterSize;

        if (remainingMines >= MIN_CLUSTER_SIZE) {
            // Генерируем случайный размер кластера в пределах //
            std::uniform_int_distribution<int> sizeDist(MIN_CLUSTER_SIZE,
                                                        std::min(MAX_CLUSTER_SIZE, remainingMines));
            clusterSize = sizeDist(randomGenerator);
        } else { // Если осталось мало мин, делаем маленький кластер
            clusterSize = remainingMines;
        }

        if (clusterSize < 1) break;

        Point clusterCenter = getRandomPosition(width, height);
        attempts++;

        // Проверки можно ли разместить кластер //
        if (inSafeZone(clusterCenter, safePoint, safeRadius)) {
            continue;
        }

        if (!clusterCenters.empty() &&
            isTooCloseToExistingClusters(clusterCenter, clusterCenters)) {
            continue;
        }

        if (!canPlaceMine(field, clusterCenter)) {
            continue;
        }

        placeSingleCluster(field, clusterCenter, placedMines, clusterSize, safePoint, safeRadius);
        clusterCenters.push_back(clusterCenter);

    }

    // Если после кластеров остались мины, расставляем их случайно //
    if (placedMines < mines) {
        placeRemainingMinesRandomly(field, placedMines, mines, safePoint, safeRadius);
    }

    field.setTotalMines(placedMines);
}

// Генерируем список клеток вокруг центра для заполнения кластера //
std::vector<Point> ClusteredPlacement::generateClusterPoints(const Point& center) {

    std::vector<Point> points;
    points.push_back(center);

    // Собираем все точки в радиусе //
    std::vector<Point> allPoints;
    for (int dy = -CLUSTER_RADIUS; dy <= CLUSTER_RADIUS; ++dy) {
        for (int dx = -CLUSTER_RADIUS; dx <= CLUSTER_RADIUS; ++dx) {
            if (dx == 0 && dy == 0) continue;

            allPoints.push_back(Point(center.getX() + dx, center.getY() + dy));
        }
    }

    std::shuffle(allPoints.begin(), allPoints.end(), randomGenerator); // Перемешиваем точки для разнообразия

    points.insert(points.end(), allPoints.begin(), allPoints.end());

    return points;
}

// Проверяем расстояние между кластерами //
bool ClusteredPlacement::isTooCloseToExistingClusters(const Point& point,
                                                      const std::vector<Point>& existingClusters) const {
    for (const auto& cluster : existingClusters) {

        int dx = std::abs(point.getX() - cluster.getX());
        int dy = std::abs(point.getY() - cluster.getY());
        int distance = std::max(dx, dy);

        if (distance < MIN_DISTANCE_BETWEEN_CLUSTERS) {
            return true;
        }
    }
    return false;
}

// Размещяем кластер //
void ClusteredPlacement::placeSingleCluster(GameField& field, const Point& center,
                                            int& placedMines, int clusterSize,
                                            const Point& safePoint, int safeRadius) {

    int width = field.getWidth();
    int height = field.getHeight();

    auto clusterPoints = generateClusterPoints(center);
    int minesInThisCluster = 0;

    // Размещаем мины в точках кластера //
    for (const auto& point : clusterPoints) {

        if (minesInThisCluster >= clusterSize) break;

        if (point.getX() < 0 || point.getX() >= width || point.getY() < 0 || point.getY() >= height) {
            continue;
        }

        if (inSafeZone(point, safePoint, safeRadius)) {
            continue;
        }

        if (canPlaceMine(field, point)) {
            Cell* cell = field.getCell(point.getX(), point.getY());
            if (cell) {
                cell->setMine(true);
                placedMines++;
                minesInThisCluster++;
            }
        }
    }
}

void ClusteredPlacement::placeRemainingMinesRandomly(GameField& field, int& placedMines,
                                                     int totalMines,
                                                     const Point& safePoint, int safeRadius) {

    int width = field.getWidth();
    int height = field.getHeight();

    int fallbackAttempts = 0;
    int maxFallbackAttempts = width * height * 2;

    while (placedMines < totalMines && fallbackAttempts < maxFallbackAttempts) {
        Point pos = getRandomPosition(width, height);

        if (!inSafeZone(pos, safePoint, safeRadius) &&
            canPlaceMine(field, pos)) {
            Cell* cell = field.getCell(pos.getX(), pos.getY());
            if (cell) {
                cell->setMine(true);
                placedMines++;
            }
        }

        fallbackAttempts++;
    }
}

QString ClusteredPlacement::getName() const {
    return "clustered";
}

int ClusteredPlacement::getMaxAttemptsMultiplier() const {
    return CLUSTER_MAX_ATTEMPTS_MULTIPLIER;
}
