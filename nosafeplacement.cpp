#include "nosafeplacement.hpp"

NoSafePlacement::NoSafePlacement() : RandomMinePlacement() {}

void NoSafePlacement::placeMines(GameField& field, int mines, const Point& safePoint) {

    int placedMines = 0;
    int fieldWidth = field.getWidth();
    int fieldHeight = field.getHeight();

    int attempts = 0;
    int maxAttempts = fieldWidth * fieldHeight * RANDOM_MAX_ATTEMPTS_MULTIPLIER;

    while (placedMines < mines && attempts < maxAttempts) {

        Point randomPos = getRandomPosition(fieldWidth, fieldHeight);

        if (canPlaceMine(field, randomPos)) {

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

QString NoSafePlacement::getName() const {
    return "nosafe";
}
