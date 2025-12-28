#include "attemptsplacementstrategy.hpp"
#include "gamefield.hpp"

#include <chrono>

AttemptsPlacementStrategy::AttemptsPlacementStrategy() :
    randomGenerator(std::chrono::system_clock::now().time_since_epoch().count()) {}

// Проверка на возможность поставить мину //
bool AttemptsPlacementStrategy::canPlaceMine(const GameField& field, const Point& point) const {
    const Cell* cell = field.getCell(point.getX(), point.getY());
    return cell && !cell->getIsMine();
}

// Проверка на нахождение точки в безопасной зоне вокруг безопасной точки //
bool AttemptsPlacementStrategy::inSafeZone(const Point& point, const Point& safePoint, int radius) const {

    int dx = std::abs(point.getX() - safePoint.getX());
    int dy = std::abs(point.getY() - safePoint.getY());

    return dx <= radius && dy <= radius;
}

// Взятие случайной позиции на поле //
Point AttemptsPlacementStrategy::getRandomPosition(int maxX, int maxY) {

    // Создаём распределение для равномерного получения случайных чисел //
    std::uniform_int_distribution<int> distX(0, maxX - 1);
    std::uniform_int_distribution<int> distY(0, maxY - 1);

    // Генерируем случайные координаты //
    int x = distX(randomGenerator);
    int y = distY(randomGenerator);

    return Point(x, y);

}

// Вычисление безопасной зоны вокруг безопасной клетки //
int AttemptsPlacementStrategy::calculateSafeRadius(int width, int height) const {
    return (width >= LARGE_FIELD_THRESHOLD || height >= LARGE_FIELD_THRESHOLD)
    ? SAFE_RADIUS_LARGE
    : SAFE_RADIUS_SMALL;
}

int AttemptsPlacementStrategy::getMaxAttemptsMultiplier() const {
    return DEFAULT_MAX_ATTEMPTS_MULTIPLIER;
}
