#include "statistics.hpp"

#include <QSettings>

const QString Statistics::TOTAL_GAMES_KEY = "statistics/totalGames";
const QString Statistics::WINS_KEY = "statistics/wins";
const QString Statistics::LOSSES_KEY = "statistics/losses";
const QString Statistics::TOTAL_TIME_KEY = "statistics/totalTime";
const QString Statistics::BEST_TIME_KEY = "statistics/bestTime";

Statistics::Statistics()

    : totalGames(0),
      wins(0),
      losses(0),
      totalTimePlayed(0),
      bestTime(0)

{
    load();
}

// Добавление результата игры //
void Statistics::addGameResult(bool won, int time) {

    totalGames++;

    if (won) {

        wins++;

        if (bestTime == 0 || time < bestTime) {
            bestTime = time;
        }
    }
    else {
        losses++;
    }

    totalTimePlayed += time;

    save();
}

// Загрузка данных статистики //
void Statistics::load() {

    QSettings settings;
    totalGames = settings.value(TOTAL_GAMES_KEY, 0).toInt();
    wins = settings.value(WINS_KEY, 0).toInt();
    losses = settings.value(LOSSES_KEY, 0).toInt();
    totalTimePlayed = settings.value(TOTAL_TIME_KEY, 0).toInt();
    bestTime = settings.value(BEST_TIME_KEY, 0).toInt();

}

// Сохранение данных статистики //
void Statistics::save() const {

    QSettings settings;
    settings.setValue(TOTAL_GAMES_KEY, totalGames);
    settings.setValue(WINS_KEY, wins);
    settings.setValue(LOSSES_KEY, losses);
    settings.setValue(TOTAL_TIME_KEY, totalTimePlayed);
    settings.setValue(BEST_TIME_KEY, bestTime);

}

// Сброс статистики //
void Statistics::reset() {

    totalGames = 0;
    wins = 0;
    losses = 0;
    totalTimePlayed = 0;
    bestTime = 0;

    save();
}

// Геттеры //
double Statistics::getWinRate() const {

    if (totalGames == 0) {
        return 0.0;
    }

    return static_cast<double>(wins) / totalGames * IN_PERCENTS;
}

int Statistics::getBestTime() const {
    return bestTime;
}

double Statistics::getAverageTime() const {

    if (totalGames == 0) {
        return 0.0;
    }

    return static_cast<double>(totalTimePlayed) / totalGames;
}

int Statistics::getTotalGames() const {
    return totalGames;
}

int Statistics::getWins() const {
    return wins;
}

int Statistics::getLosses() const {
    return losses;
}

int Statistics::getTotalTimePlayed() const {
    return totalTimePlayed;
}
