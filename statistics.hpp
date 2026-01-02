#pragma once

#include <QString>

class Statistics
{
public:

    static const QString TOTAL_GAMES_KEY;
    static const QString WINS_KEY;
    static const QString LOSSES_KEY;
    static const QString TOTAL_TIME_KEY;
    static const QString BEST_TIME_KEY;

    static constexpr double IN_PERCENTS = 100.0; // Для перевода в проценты

private:

    int totalGames;
    int wins;
    int losses;
    int totalTimePlayed;
    int bestTime;

public:

    Statistics();

    Statistics(const Statistics& other) = default;

    void addGameResult(bool won, int time);
    void load();
    void save() const;
    void reset();

    double getWinRate() const;
    int getBestTime() const;
    double getAverageTime() const;
    int getTotalGames() const;
    int getWins() const;
    int getLosses() const;
    int getTotalTimePlayed() const;

};
