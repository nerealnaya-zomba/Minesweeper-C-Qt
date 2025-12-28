#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QIcon>
#include <QDebug>

#include "aboutprogram.hpp"
#include "gamewindow.hpp"
#include "howtoplay.hpp"
#include "settingswindow.hpp"
#include "statisticswindow.hpp"
#include "themestyles.hpp"
#include "windowswitcher.hpp"

QTranslator* MainWindow::appTranslator = nullptr;

MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent),
      ui(std::make_unique<Ui::MainWindow>()),
      currentDifficulty(Difficulty::beginner()),
      currentSettings(std::make_shared<Settings>()),
      gameStatistics(std::make_shared<Statistics>())

{
    ui->setupUi(this);
    applySettings();
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    setWindowIcon(QIcon(":/mine.ico"));
}

void MainWindow::setupConnections() {

    connect(ui->restartTrigger, &QAction::triggered, ui->startGameButton, &QPushButton::click);

    connect(ui->settingsTrigger, &QAction::triggered, ui->settingsButton, &QPushButton::click);

    connect(ui->statisticsTrigger, &QAction::triggered, ui->statisticsButton, &QPushButton::click);

    connect(ui->beginnerDifficulty, &QAction::triggered, this, [this]() {
        currentDifficulty = Difficulty::beginner();
        on_startGameButton_clicked();
    });

    connect(ui->intermediateDifficulty, &QAction::triggered, this, [this]() {
        currentDifficulty = Difficulty::intermediate();
        on_startGameButton_clicked();
    });

    connect(ui->expertDifficulty, &QAction::triggered, this, [this]() {
        currentDifficulty = Difficulty::expert();
        on_startGameButton_clicked();
    });

    connect(ui->howToPlayTrigger, &QAction::triggered, this, [this]() {
        WindowSwitcher::switchToModal<HowToPlay>(this);
    });

    connect(ui->aboutProgramTrigger, &QAction::triggered, this, [this]() {
        WindowSwitcher::switchToModal<AboutProgram>(this);
    });

    connect(this, &MainWindow::languageChanged, this, &MainWindow::onLanguageChanged);

}

// Применение настроек //
void MainWindow::applySettings() {

    if (!currentSettings) return;

    QString theme = currentSettings->getTheme();
    qApp->setStyleSheet(ThemeStyles::getStyleSheet(theme));

    QString lang = currentSettings->getLanguage();

    // Удаляем старый глобальный переводчик //
    if (MainWindow::appTranslator) {
        QCoreApplication::removeTranslator(MainWindow::appTranslator);
        delete MainWindow::appTranslator;
        MainWindow::appTranslator = nullptr;
    }

    // Создаем и устанавливаем новый глобальный переводчик //
    MainWindow::appTranslator = new QTranslator();
    QString qmFile = QString(":/translations/minesweeper_%1").arg(lang);

    if (MainWindow::appTranslator->load(qmFile)) {
        QCoreApplication::installTranslator(MainWindow::appTranslator);
    } else { // Альтернативный путь
        qmFile = QString(":/translations/minesweeper_%1.qm").arg(lang);
        if (MainWindow::appTranslator->load(qmFile)) {
            QCoreApplication::installTranslator(MainWindow::appTranslator);
        } else {
            qDebug() << "Не удалось загрузить перевод для языка:" << lang;
            delete MainWindow::appTranslator;
            MainWindow::appTranslator = nullptr;
        }
    }

    emit languageChanged(lang);

}

// Начать игру //
void MainWindow::on_startGameButton_clicked() {
    GameWindow* gameWindow = WindowSwitcher::switchTo<GameWindow>(this, currentDifficulty, currentSettings, gameStatistics);
    connect(gameWindow, &GameWindow::languageChanged, this, &MainWindow::applySettings); // Подключаем сигнал о смене языка
}

// Открыть настройки игры //
void MainWindow::on_settingsButton_clicked() {
    SettingsWindow* settingsWindow = WindowSwitcher::switchTo<SettingsWindow>(this, currentSettings);
    connect(settingsWindow, &SettingsWindow::settingsChanged, this, &MainWindow::applySettings); // Подключаем сигнал о смене настроек
}

// Открыть статистику игры //
void MainWindow::on_statisticsButton_clicked() {
    StatisticsWindow* statisticsWindow = WindowSwitcher::switchTo<StatisticsWindow>(this, gameStatistics);
}

// Выход из игры //
void MainWindow::on_quitButton_clicked() {
    close();
}

void MainWindow::onLanguageChanged(const QString& lang) {
    Q_UNUSED(lang);
    ui->retranslateUi(this);
    setWindowTitle(tr("Сапёр"));
}

void MainWindow::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        setWindowTitle(tr("Сапёр"));
    }
    QMainWindow::changeEvent(event);
}
