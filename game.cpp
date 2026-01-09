#include "game.hpp"

Game::Game(const Difficulty& difficulty,
           const Strategy& strategy,
           const Settings& settings,
           std::shared_ptr<Statistics> externalStatistics)

    : currentDifficulty(difficulty),
      currentSettings(settings),
      currentStrategy(strategy),
      minePlacer(strategy.createPlacementStrategy()),
      field(difficulty, minePlacer),
      timer(),
      gameStatistics(externalStatistics),
      gameState(GameState::Waiting),
      safeStartPoint(0, 0)

{
    field.setTotalMines(difficulty.getMines());
}

Game::~Game() {}

void Game::startGame(const Point& safeStartPoint) {

    this->safeStartPoint = safeStartPoint;

    int numMinesToPlace = currentDifficulty.getMines();
    minePlacer->placeMines(field, numMinesToPlace, safeStartPoint);
    field.countAdjacentMines();

    // Если мина попала в первую ячейку //
    if (field.getCell(safeStartPoint.getX(), safeStartPoint.getY())->getIsMine())
    {
        endGame(false);
        return;
    }

    this->gameState = GameState::Running;
    field.revealCell(safeStartPoint);

    timer.restart();
    timer.start();

}

void Game::restartGame() {

    this->gameState = GameState::Waiting;

    field.resetField();

    field.setTotalMines(currentDifficulty.getMines());

    this->safeStartPoint = Point(-1, -1); // Сбрасываем безопасную точку

    timer.restart();
    timer.stop();

}

void Game::endGame(bool won) {

    int timePlayed = timer.getElapsedSeconds();

    timer.stop();

    gameStatistics->addGameResult(won, timePlayed);

    gameState = won ? GameState::Won : GameState::Lost;

    if (won) {
        field.flagsAllRemainingMines();
    } else {
        field.revealAllMines();
    }

}

void Game::cellClick(Point clickPoint) {

    if (gameState != GameState::Running) {
        return;
    }

    const Cell* cell = field.getCell(clickPoint.getX(), clickPoint.getY());
    if (!cell) return;

    // Не открываем клетку с флажком //
    if (cell->getIsFlagged()) {
        return;
    }

    bool revealSuccessful = field.revealCell(clickPoint);

    if (!revealSuccessful) {
        endGame(false);
        return;
    }

    if (field.checkWin()) {
        endGame(true);
        return;
    }
}

void Game::flagToggle(Point flagPoint) {

    if (gameState != GameState::Running && gameState != GameState::Waiting) {
        return;
    }
    field.toggleFlag(flagPoint);

}

void Game::chordClick(Point clickPoint) {

    if (gameState != GameState::Running) {
        return;
    }

    const Cell* cell = field.getCell(clickPoint.getX(), clickPoint.getY());
    if (!cell || !cell->getIsRevealed() || cell->getIsFlagged()) {
        return;
    }

    auto neighbours = field.getNeighbours(clickPoint);
    int flaggedCount = 0;

    // Считаем кол-во флагов вокруг //
    for (Cell* neighbour : neighbours) {
        if (neighbour && neighbour->getIsFlagged()) {
            flaggedCount++;
        }
    }

    if (flaggedCount != cell->getAdjacentMines()) {
        return;  // Если флагов не столько же, сколько мин - ничего не делаем
    }

    // Открываем соседей
    bool hitMine = false;
    for (Cell* neighbour : neighbours) {
        if (neighbour && !neighbour->getIsRevealed() && !neighbour->getIsFlagged()) {
            Point neighbourPoint(neighbour->getX(), neighbour->getY());
            bool revealSuccessful = field.revealCell(neighbourPoint);

            if (!revealSuccessful && neighbour->getIsMine()) {
                hitMine = true;
            }
        }
    }

    if (hitMine) {
        endGame(false);
        return;
    }

    if (field.checkWin()) {
        endGame(true);
    }

}

// Геттеры //
GameField& Game::getGameField() {
    return field;
}

GameTimer& Game::getTimer() {
    return timer;
}

GameState& Game::getGameState() {
    return gameState;
}

const Difficulty& Game::getCurrentDifficulty() {
    return currentDifficulty;
}

const Strategy& Game::getCurrentStrategy() {
    return currentStrategy;
}

const Settings& Game::getCurrentSettings() {
    return currentSettings;
}

std::shared_ptr<Statistics> Game::getStatistics() {
    return gameStatistics;
}

// Сеттеры //
void Game::setCurrentDifficulty(const Difficulty& newDifficulty) {
    this->currentDifficulty = newDifficulty;
    restartGame();
}

void Game::setCurrentStrategy(const Strategy& newStrategy) {
    this->currentStrategy = newStrategy;
    minePlacer = newStrategy.createPlacementStrategy();
    restartGame();
}

void Game::setCurrentSettings(const Settings& newSettings) {
    this->currentSettings = newSettings;
}
