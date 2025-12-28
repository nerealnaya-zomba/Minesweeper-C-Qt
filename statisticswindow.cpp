#include "statisticswindow.hpp"
#include "ui_statisticswindow.h"

#include <QMessageBox>

StatisticsWindow::StatisticsWindow(std::shared_ptr<Statistics> statistics, QWidget* parent)
    : QDialog(parent),
    ui(std::make_unique<Ui::StatisticsWindow>()),
    gameStatistics(statistics)
{
    ui->setupUi(this);
    setWindowTitle(tr("Statistics"));
    updateUI();
}

StatisticsWindow::~StatisticsWindow() = default;

void StatisticsWindow::updateUI() {

    if (!gameStatistics) return;

    ui->totalGamesLabel->setText(QString::number(gameStatistics->getTotalGames()));
    ui->winsLabel->setText(QString::number(gameStatistics->getWins()));
    ui->lossesLabel->setText(QString::number(gameStatistics->getLosses()));

    double winRate = gameStatistics->getWinRate();
    ui->winRateLabel->setText(QString("%1").arg(winRate, 0, 'f', 1));

    int bestTime = gameStatistics->getBestTime();
    ui->bestTimeLabel->setText(
        bestTime > 0
        ? QString(tr("%1 sec")).arg(bestTime)
        : "-"
    );

    double avgTime = gameStatistics->getAverageTime();
    ui->avgTimeLabel->setText(
        avgTime > 0
        ? QString(tr("%1 sec")).arg(avgTime, 0, 'f', 1)
        : "-"
    );

}

void StatisticsWindow::on_resetButton_clicked() {

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Reset statistics"),
        tr("Are you sure you want to reset all your statistics?"),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (gameStatistics) {
            gameStatistics->reset();
            updateUI();
        }
    }

}
