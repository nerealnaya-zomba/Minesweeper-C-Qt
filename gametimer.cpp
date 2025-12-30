#include "gametimer.hpp"

GameTimer::GameTimer() : elapsedSeconds(0), isRunning(false)
{
    // Обновляем время после каждой пройденной секунды //
    connect(&timer, &QTimer::timeout, this, &GameTimer::updateTimer);

}

void GameTimer::start() {

    if (!isRunning) {
        isRunning = true;
        timer.start(TIMER_INTERVAL_MS); // Запускаем таймер на 1 секунду
    }
}

void GameTimer::stop() {

    if (isRunning) {
        isRunning = false;
        timer.stop();
    }
}

void GameTimer::restart() {
    stop();
    elapsedSeconds = 0;
}

int GameTimer::getElapsedSeconds() const {
    return elapsedSeconds;
}

bool GameTimer::getIsRunning() const {
    return isRunning;
}

QString GameTimer::getFormattedTime() const {
    return QString("%1").arg(elapsedSeconds, TIME_COMPONENT_WIDTH, DECIMAL_BASE, QChar('0'));
}

void GameTimer::updateTimer() {

    if (isRunning && elapsedSeconds < MAX_GAME_TIME) {
        elapsedSeconds++;
        emit timeUpdated(getFormattedTime());
    }

}
