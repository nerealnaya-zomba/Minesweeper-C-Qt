#include "gamefield.hpp"

GameField::GameField(const Difficulty& difficulty, std::shared_ptr<MinePlacementStrategy> minePlacerPtr)

    : totalMines(0),
      cellsRevealed(0),
      flagsPlaced(0),
      width(difficulty.getWidth()),
      height(difficulty.getHeight()),
      minePlacer(minePlacerPtr)
{

    grid.resize(height);
    for (int i = 0; i < height; i++) {
        grid[i].resize(width);
        for (int j = 0; j < width; j++) {
            grid[i][j] = std::make_unique<Cell>();
            grid[i][j]->setCoordinates(j, i);
        }
    }

}

GameField::GameField(const GameField& other)

    : totalMines(other.totalMines),
      cellsRevealed(other.cellsRevealed),
      flagsPlaced(other.flagsPlaced),
      width(other.width),
      height(other.height),
      minePlacer(other.minePlacer)
{

    grid.resize(height);
    for (int i = 0; i < height; i++) {
        grid[i].resize(width);
        for (int j = 0; j < width; j++) {
            grid[i][j] = std::make_unique<Cell>(*other.grid[i][j]);
        }
    }

}

bool GameField::revealCell(Point p) {

    if (p.getX() < 0 || p.getX() >= width || p.getY() < 0 || p.getY() >= height) {
        return false;
    }

    Cell* cell = grid[p.getY()][p.getX()].get();

    if (cell->getIsRevealed() || cell->getIsFlagged()) {
        return false;
    }

    cell->setRevealed(true);
    cellsRevealed++;

    if (cell->getIsMine()) {
        return false;
    }
    else {

        if (cell->getAdjacentMines() == 0) {

            // Рекурсивно открываем соседние клетки //
            std::vector<Cell*> neighbours = getNeighbours(p);

            for (size_t i = 0; i < neighbours.size(); ++i) {

                Cell* neighbour = neighbours[i];

                if (neighbour && !neighbour->getIsRevealed() && !neighbour->getIsFlagged()) {
                    revealCell(Point(neighbour->getX(), neighbour->getY()));
                }
            }
        }
        return true;
    }
}

bool GameField::toggleFlag(Point p) {

    if (p.getX() < 0 || p.getX() >= width || p.getY() < 0 || p.getY() >= height) {
        return false;
    }

    Cell* cell = grid[p.getY()][p.getX()].get();

    if (cell->getIsRevealed()) {
        return false;
    }

    if (cell->getIsFlagged()) { // Убираем существующий флаг
        cell->setFlagged(false);
        flagsPlaced--;
    }
    else if (flagsPlaced < totalMines) { // Ставим флаг если не превышен лимит
        cell->setFlagged(true);
        flagsPlaced++;
    } else return false;

    return true;
}

// Геттеры //
int GameField::getFlagsPlaced() const {
    return flagsPlaced;
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

Cell* GameField::getCell(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return nullptr;
    }

    return grid[y][x].get();
}

const Cell* GameField::getCell(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return nullptr;
    }

    return grid[y][x].get();
}

std::vector<Cell*> GameField::getNeighbours(Point p) {

    std::vector<Cell*> neighbours;
    int x = p.getX();
    int y = p.getY();

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {

            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                neighbours.push_back(getCell(nx, ny));
            }

        }
    }
    return neighbours;
}

// Сеттеры //
void GameField::setTotalMines(int totalMines) {
    this->totalMines = totalMines;
}

// Подсчёт кол-ва мин в соседних клетках //
void GameField::countAdjacentMines() {

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            if (grid[y][x]->getIsMine()) continue;

            int mineCount = 0;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {

                    if (dx == 0 && dy == 0) continue;

                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {

                        if (grid[ny][nx]->getIsMine()) {
                            mineCount++;
                        }
                    }
                }
            }
            grid[y][x]->setAdjacentMines(mineCount);
       }
    }
}

bool GameField::checkWin() {
    return cellsRevealed == (width * height - totalMines);
}

void GameField::revealAllMines() {

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x]->getIsMine()) {
                grid[y][x]->setRevealed(true);
            }
        }
    }
}

// Установка флажков на оставшиеся мины при победе //
void GameField::flagsAllRemainingMines() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x]->getIsMine() && !grid[y][x]->getIsFlagged()) {
                grid[y][x]->setFlagged(true);
                flagsPlaced++;
            }
        }
    }
}

void GameField::resetField() {

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x]->setMine(false);
            grid[y][x]->setRevealed(false);
            grid[y][x]->setFlagged(false);
            grid[y][x]->setAdjacentMines(0);
        }
    }

    totalMines = 0;
    cellsRevealed = 0;
    flagsPlaced = 0;

}
