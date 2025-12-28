#pragma once

#include "mineplacementstrategy.hpp"

#include <random>

class AttemptsPlacementStrategy : public MinePlacementStrategy
{

protected:

    // Общие константы для всех стратегий через попытки //
    static constexpr int SAFE_RADIUS_SMALL = 1;   // 3x3 безопасная зона
    static constexpr int SAFE_RADIUS_LARGE = 2;   // 5x5 безопасная зона
    static constexpr int LARGE_FIELD_THRESHOLD = 16;
    static constexpr int DEFAULT_MAX_ATTEMPTS_MULTIPLIER = 5;

protected:

    std::mt19937 randomGenerator; // Генератор случайных чисел

protected:

    bool canPlaceMine(const GameField& field, const Point& point) const;
    bool inSafeZone(const Point& point, const Point& safePoint, int radius) const;
    Point getRandomPosition(int maxX, int maxY);
    int calculateSafeRadius(int width, int height) const;

    virtual int getMaxAttemptsMultiplier() const;

public:

    AttemptsPlacementStrategy();
    virtual ~AttemptsPlacementStrategy() = default;

    void placeMines(GameField& field, int mines, const Point& safePoint) override = 0;
    QString getName() const override = 0;

};
