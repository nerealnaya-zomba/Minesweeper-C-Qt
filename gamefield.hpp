#pragma once

#include "cell.hpp"
#include "mineplacementstrategy.hpp"
#include "difficulty.hpp"
#include "point.hpp"
#include <vector>

class GameField
{
private:

    std::vector<std::vector<std::unique_ptr<Cell>>> grid;
    int totalMines;
    int cellsRevealed;
    int flagsPlaced;
    int width;
    int height;
    std::shared_ptr<MinePlacementStrategy> minePlacer;

public:

    GameField(const Difficulty& difficulty, std::shared_ptr<MinePlacementStrategy> minePlacerPtr);

    GameField(const GameField& other);

    bool revealCell(Point p);
    bool toggleFlag(Point p);

    Cell* getCell(int x, int y);
    const Cell* getCell(int x, int y) const;
    std::vector<Cell*> getNeighbours(Point p);
    int getFlagsPlaced() const;
    int getWidth() const;
    int getHeight() const;

    void setTotalMines(int totalMines);

    void countAdjacentMines();
    bool checkWin();
    void revealAllMines();
    void resetField();

};
