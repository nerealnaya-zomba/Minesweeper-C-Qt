#pragma once

#include "statistics.hpp"

#include <QDialog>

namespace Ui {
class StatisticsWindow;
}

class StatisticsWindow : public QDialog
{
    Q_OBJECT

private:

    std::unique_ptr<Ui::StatisticsWindow> ui;
    std::shared_ptr<Statistics> gameStatistics;

public:

    explicit StatisticsWindow(std::shared_ptr<Statistics> statistics, QWidget* parent = nullptr);
    ~StatisticsWindow();

    void updateUI();

private slots:

    void on_resetButton_clicked();

};
