#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QIcon>
#include <QDebug>
#include <QAbstractAnimation>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QShowEvent>
#include <QStyle>
#include <QTimer>

#include "aboutprogram.hpp"
#include "customdifficultywindow.hpp"
#include "gamewindow.hpp"
#include "howtoplay.hpp"
#include "settingswindow.hpp"
#include "statisticswindow.hpp"
#include "themestyles.hpp"
#include "windowswitcher.hpp"

std::unique_ptr<QTranslator> MainWindow::appTranslator = nullptr;

MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent),
      ui(std::make_unique<Ui::MainWindow>()),
      currentDifficulty(Difficulty::beginner()),
      currentStrategy(Strategy::random()),
      currentSettings(std::make_shared<Settings>()),
      gameStatistics(std::make_shared<Statistics>()),
      menuEntranceAnimated(false)

{
    ui->setupUi(this);
    applySettings();
    setupMenuPresentation();
    setupConnections();
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
    if (!menuEntranceAnimated) {
        QTimer::singleShot(20, this, &MainWindow::animateMenuEntrance);
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    QPushButton* button = qobject_cast<QPushButton*>(obj);
    if (button && event->type() == QEvent::Enter) {
        animateButtonPulse(button);
    }

    return QMainWindow::eventFilter(obj, event);
}

MainWindow::~MainWindow() = default;

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

    connect(ui->customDifficulty, &QAction::triggered, this, [this] () {

        CustomDifficultyWindow dialog(this);
        connect(&dialog, &CustomDifficultyWindow::settingsAccepted, this, [this](int width, int height, int mines) {
            currentDifficulty = Difficulty::custom(width, height, mines);
            on_startGameButton_clicked();
        });
        dialog.exec();

    });

    connect(ui->randomMode, &QAction::triggered, this, [this]() {
        currentStrategy = Strategy::random();
        on_startGameButton_clicked();
    });

    connect(ui->noSafeZoneMode, &QAction::triggered, this, [this]() {
        currentStrategy = Strategy::noSafe();
        on_startGameButton_clicked();
    });

    connect(ui->clusteredMode, &QAction::triggered, this, [this]() {
        currentStrategy = Strategy::clustered();
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
void MainWindow::setupMenuPresentation() {
    const QList<QPushButton*> buttons = {
        ui->startGameButton,
        ui->settingsButton,
        ui->statisticsButton,
        ui->quitButton
    };

    ui->startGameButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->settingsButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    ui->statisticsButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogInfoView));
    ui->quitButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));

    for (QPushButton* button : buttons) {
        button->setCursor(Qt::PointingHandCursor);
        button->setIconSize(QSize(20, 20));
        button->installEventFilter(this);
    }

    setWindowOpacity(0.0);
}

void MainWindow::animateMenuEntrance() {
    if (menuEntranceAnimated) {
        return;
    }
    menuEntranceAnimated = true;

    QPropertyAnimation* windowAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    windowAnimation->setDuration(220);
    windowAnimation->setStartValue(windowOpacity());
    windowAnimation->setEndValue(1.0);
    windowAnimation->setEasingCurve(QEasingCurve::OutCubic);
    windowAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    const QList<QPushButton*> buttons = {
        ui->startGameButton,
        ui->settingsButton,
        ui->statisticsButton,
        ui->quitButton
    };

    int delay = 0;
    for (QPushButton* button : buttons) {
        QRect endGeometry = button->geometry();
        QRect startGeometry = endGeometry.translated(0, 18);
        button->setGeometry(startGeometry);

        QGraphicsOpacityEffect* effect = qobject_cast<QGraphicsOpacityEffect*>(button->graphicsEffect());
        if (!effect) {
            effect = new QGraphicsOpacityEffect(button);
            button->setGraphicsEffect(effect);
        }
        effect->setOpacity(0.0);

        QTimer::singleShot(delay, this, [button, effect, startGeometry, endGeometry]() {
            QPropertyAnimation* moveAnimation = new QPropertyAnimation(button, "geometry", button);
            moveAnimation->setDuration(260);
            moveAnimation->setStartValue(startGeometry);
            moveAnimation->setEndValue(endGeometry);
            moveAnimation->setEasingCurve(QEasingCurve::OutBack);
            moveAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation* opacityAnimation = new QPropertyAnimation(effect, "opacity", button);
            opacityAnimation->setDuration(220);
            opacityAnimation->setStartValue(0.0);
            opacityAnimation->setEndValue(1.0);
            opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
            opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });

        delay += 55;
    }
}

void MainWindow::animateButtonPulse(QPushButton* button) {
    if (!button) {
        return;
    }

    QRect baseGeometry = button->geometry();
    QRect expandedGeometry = baseGeometry.adjusted(-4, -2, 4, 2);

    QPropertyAnimation* animation = new QPropertyAnimation(button, "geometry", button);
    animation->setDuration(140);
    animation->setStartValue(baseGeometry);
    animation->setKeyValueAt(0.45, expandedGeometry);
    animation->setEndValue(baseGeometry);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::applySettings() {

    if (!currentSettings) return;

    QString theme = currentSettings->getTheme();
    qApp->setStyleSheet(ThemeStyles::getStyleSheet(theme));

    QString lang = currentSettings->getLanguage();

    MainWindow::appTranslator.reset(); // Удаляем старый глобальный переводчик

    // Создаем и устанавливаем новый глобальный переводчик //
    MainWindow::appTranslator = std::make_unique<QTranslator>();
    QString qmFile = QString(":/translations/minesweeper_%1").arg(lang);

    if (MainWindow::appTranslator->load(qmFile)) {
        QCoreApplication::installTranslator(MainWindow::appTranslator.get());
    } else { // Альтернативный путь
        qmFile = QString(":/translations/minesweeper_%1.qm").arg(lang);
        if (MainWindow::appTranslator->load(qmFile)) {
            QCoreApplication::installTranslator(MainWindow::appTranslator.get());
        } else {
            qDebug() << "Не удалось загрузить перевод для языка:" << lang;
            MainWindow::appTranslator.reset();
        }
    }

    emit languageChanged(lang);

}

// Начать игру //
void MainWindow::on_startGameButton_clicked() {
    animateButtonPulse(ui->startGameButton);
    GameWindow* gameWindow = WindowSwitcher::switchTo<GameWindow>(this, currentDifficulty, currentStrategy, currentSettings, gameStatistics);
    connect(gameWindow, &GameWindow::languageChanged, this, &MainWindow::applySettings); // Подключаем сигнал о смене языка
}

// Открыть настройки игры //
void MainWindow::on_settingsButton_clicked() {
    animateButtonPulse(ui->settingsButton);
    WindowSwitcher::switchToModalAnimated<SettingsWindow>(this, currentSettings);
    applySettings();
}

// Открыть статистику игры //
void MainWindow::on_statisticsButton_clicked() {
    animateButtonPulse(ui->statisticsButton);
    WindowSwitcher::switchToModalAnimated<StatisticsWindow>(this, gameStatistics);
}

// Выход из игры //
void MainWindow::on_quitButton_clicked() {
    animateButtonPulse(ui->quitButton);
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
