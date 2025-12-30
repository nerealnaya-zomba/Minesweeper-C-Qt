#pragma once

#include <QMainWindow>
#include <QTranslator>

#include "difficulty.hpp"
#include "settings.hpp"
#include "statistics.hpp"
#include "strategy.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static QTranslator* appTranslator;

private:

    std::unique_ptr<Ui::MainWindow> ui;
    Difficulty currentDifficulty;
    Strategy currentStrategy;
    std::shared_ptr<Settings> currentSettings;
    std::shared_ptr<Statistics> gameStatistics;

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupUI();
    void setupConnections();

    void applySettings();

    void changeEvent(QEvent* event);

private slots:

    void on_startGameButton_clicked();
    void on_settingsButton_clicked();
    void on_statisticsButton_clicked();
    void on_quitButton_clicked();

    void onLanguageChanged(const QString& lang);

signals:

    void languageChanged(const QString& language);
    void settingsChanged();

};
