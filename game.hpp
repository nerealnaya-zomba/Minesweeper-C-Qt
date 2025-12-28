#pragma once

#include "gamefield.hpp"
#include "gametimer.hpp"
#include "difficulty.hpp"
#include "settings.hpp"
#include "statistics.hpp"
#include "point.hpp"
#include "gamestate.hpp"
#include "mineplacementfactory.hpp"

class Game
{
private:

    std::shared_ptr<MinePlacementStrategy> minePlacer;
    GameField field;
    GameTimer timer;
    Difficulty currentDifficulty;
    GameState gameState;
    Settings currentSettings;
    std::shared_ptr<Statistics> gameStatistics;
    Point safeStartPoint;

public:

    Game(const Difficulty& difficulty,
         const Settings& settings,
         std::shared_ptr<Statistics> externalStatistics,
         const QString& strategyName = "random");

    ~Game();

    void startGame(const Point& safeStartPoint);
    void restartGame();
    void endGame(bool won);
    void cellClick(Point clickPoint);
    void flagToggle(Point flagPoint);

    GameField& getGameField() ;
    GameTimer& getTimer();
    GameState& getGameState();
    const Difficulty& getCurrentDifficulty();
    const Settings& getCurrentSettings();
    std::shared_ptr<Statistics> getStatistics();

    void setCurrentDifficulty(const Difficulty& newDifficulty);
    void setCurrentSettings(const Settings& newSettings);

    void setMinePlacementStrategy(const QString& strategyName);
    void setMinePlacementStrategy(std::unique_ptr<MinePlacementStrategy> strategy);

};
