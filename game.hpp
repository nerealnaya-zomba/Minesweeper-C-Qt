#pragma once

#include "gamefield.hpp"
#include "gametimer.hpp"
#include "difficulty.hpp"
#include "settings.hpp"
#include "statistics.hpp"
#include "point.hpp"
#include "gamestate.hpp"
#include "strategy.hpp"

class Game
{
private:

    std::shared_ptr<MinePlacementStrategy> minePlacer;
    GameField field;
    GameTimer timer;
    Difficulty currentDifficulty;
    Strategy currentStrategy;
    GameState gameState;
    Settings currentSettings;
    std::shared_ptr<Statistics> gameStatistics;
    Point safeStartPoint;

public:

    Game(const Difficulty& difficulty,
         const Strategy& strategy,
         const Settings& settings,
         std::shared_ptr<Statistics> externalStatistics);

    ~Game();

    void startGame(const Point& safeStartPoint);
    void restartGame();
    void endGame(bool won);

    void cellClick(Point clickPoint);
    void flagToggle(Point flagPoint);
    void chordClick(Point clickPoint);

    GameField& getGameField() ;
    GameTimer& getTimer();
    GameState& getGameState();
    const Difficulty& getCurrentDifficulty();
    const Strategy& getCurrentStrategy();
    const Settings& getCurrentSettings();
    std::shared_ptr<Statistics> getStatistics();

    void setCurrentDifficulty(const Difficulty& newDifficulty);
    void setCurrentStrategy(const Strategy& newStrategy);
    void setCurrentSettings(const Settings& newSettings);

};
