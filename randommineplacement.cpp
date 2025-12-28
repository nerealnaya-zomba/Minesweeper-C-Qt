#include "randommineplacement.hpp"

RandomMinePlacement::RandomMinePlacement() : AttemptsPlacementStrategy() {}

// Расставляем мины на поле //
void RandomMinePlacement::placeMines(GameField& field, int mines, const Point& safePoint) {

    int placedMines = 0;
    int fieldWidth = field.getWidth();
    int fieldHeight = field.getHeight();

    int safeRadius = calculateSafeRadius(fieldWidth, fieldHeight);

    int attempts = 0;
    int maxAttempts = fieldWidth * fieldHeight * getMaxAttemptsMultiplier();

    while (placedMines < mines && attempts < maxAttempts) {

        Point randomPos = getRandomPosition(fieldWidth, fieldHeight);

        // Ставим мину если не безопасная зона //
        if (!inSafeZone(randomPos, safePoint, safeRadius)
            && canPlaceMine(field, randomPos)) {

            Cell* cell = field.getCell(randomPos.getX(), randomPos.getY());

            if (cell) {
                cell->setMine(true);
                placedMines++;
            }

        }

        attempts++;
    }

    field.setTotalMines(placedMines);
}

int RandomMinePlacement::getMaxAttemptsMultiplier() const {
    return RANDOM_MAX_ATTEMPTS_MULTIPLIER;
}

QString RandomMinePlacement::getName() const{
    return "random";
}
