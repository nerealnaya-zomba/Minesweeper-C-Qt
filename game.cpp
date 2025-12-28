#include "game.hpp"

Game::Game(const Difficulty& difficulty, const Settings& settings,
           std::shared_ptr<Statistics> externalStatistics,
           const QString& strategyName)

    : currentDifficulty(difficulty),
      currentSettings(settings),
      minePlacer(MinePlacementFactory::createStrategy(strategyName)),
      field(difficulty, nullptr),
      timer(),
      gameStatistics(externalStatistics),
      gameState(GameState::Waiting),
      safeStartPoint(0, 0)

{
    field.setTotalMines(difficulty.getMines());
}

Game::~Game() {}

void Game::startGame(const Point& safeStartPoint) {

    this->gameState = GameState::Running;
    this->safeStartPoint = safeStartPoint;

    int numMinesToPlace = currentDifficulty.getMines();

    minePlacer->placeMines(field, numMinesToPlace, safeStartPoint);

    field.countAdjacentMines();

    if (field.getCell(safeStartPoint.getX(), safeStartPoint.getY())->getIsMine())
    {
        // Если мина попала в первую ячейку (это возможно только при NoSafePlacement)
        endGame(false); // Игра проиграна
        // endGame остановит таймер и установит gameState=Lost
        return; // Прерываем запуск игры
    }

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

    if (!won) {
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

    if (field.checkWin() && gameState == GameState::Running) {
        endGame(true);
        return;
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

void Game::setCurrentSettings(const Settings& newSettings) {
    this->currentSettings = newSettings;
}

// Установка стратегии по имени //
void Game::setMinePlacementStrategy(const QString& strategyName)
{
    minePlacer = MinePlacementFactory::createStrategy(strategyName);
}

// Установка готовой стратегии //
void Game::setMinePlacementStrategy(std::unique_ptr<MinePlacementStrategy> strategy)
{
    minePlacer = std::move(strategy);
}
