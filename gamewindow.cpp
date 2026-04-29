#include "gamewindow.hpp"
#include "ui_gamewindow.h"

#include <QApplication>
#include <QAbstractAnimation>
#include <QCloseEvent>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>
#include <QGridLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QScreen>
#include <QStatusBar>
#include <QTimer>
#include <QVBoxLayout>

#include "aboutprogram.hpp"
#include "customdifficultywindow.hpp"
#include "howtoplay.hpp"
#include "settingswindow.hpp"
#include "statisticswindow.hpp"
#include "themestyles.hpp"
#include "windowswitcher.hpp"

const QMap<int, QString> GameWindow::NUMBER_COLORS = {
    {1, "#2563eb"}, {2, "#16803c"}, {3, "#dc2626"},
    {4, "#1e3a8a"}, {5, "#9f1239"}, {6, "#0891b2"},
    {7, "#111827"}, {8, "#64748b"}
};

GameWindow::GameWindow(const Difficulty& currentDifficulty,
                       const Strategy& currentStrategy,
                       std::shared_ptr<Settings> settings,
                       std::shared_ptr<Statistics> statistics,
                       QWidget* parent)

    : QMainWindow(parent),
      ui(std::make_unique<Ui::GameWindow>()),
      game(nullptr),
      currentSettings(settings),
      announcedGameState(GameState::Waiting),
      leftButtonPressed(false),
      rightButtonPressed(false)
{
    ui->setupUi(this);

    game = std::make_unique<Game>(currentDifficulty, currentStrategy, *currentSettings, statistics);

    setupUI(currentDifficulty);
    createGameField();
    setupConnections();
    syncSoundSettings();
}

GameWindow::~GameWindow() = default;

void GameWindow::setupConnections() {

    connect(&game->getTimer(), &GameTimer::timeUpdated, [this](const QString& time) {
        ui->timerLabel->setText(time);
    });

    connect(ui->restartTrigger, &QAction::triggered, ui->restartButton, &QPushButton::click);

    connect(ui->beginnerDifficulty, &QAction::triggered, this, [this]() {
        changeDifficulty(Difficulty::beginner());
    });

    connect(ui->intermediateDifficulty, &QAction::triggered, this, [this]() {
        changeDifficulty(Difficulty::intermediate());
    });

    connect(ui->expertDifficulty, &QAction::triggered, this, [this]() {
        changeDifficulty(Difficulty::expert());
    });

    connect(ui->customDifficulty, &QAction::triggered, this, [this]() {
        CustomDifficultyWindow dialog(this);
        connect(&dialog, &CustomDifficultyWindow::settingsAccepted, this, [this](int width, int height, int mines) {
            changeDifficulty(Difficulty::custom(width, height, mines));
        });
        dialog.exec();
    });

    connect(ui->randomMode, &QAction::triggered, this, [this]() {
        setMinePlacementStrategy("random");
    });

    connect(ui->noSafeZoneMode, &QAction::triggered, this, [this]() {
        setMinePlacementStrategy("nosafe");
    });

    connect(ui->clusteredMode, &QAction::triggered, this, [this]() {
        setMinePlacementStrategy("clustered");
    });

    connect(ui->settingsTrigger, &QAction::triggered, this, &GameWindow::showSettingsWindow);
    connect(ui->staticticsTrigger, &QAction::triggered, this, &GameWindow::showStatisticsWindow);

    connect(ui->howToPlayTrigger, &QAction::triggered, this, [this]() {
        WindowSwitcher::switchToModal<HowToPlay>(this);
    });

    connect(ui->aboutProgramTrigger, &QAction::triggered, this, [this]() {
        WindowSwitcher::switchToModal<AboutProgram>(this);
    });
}

void GameWindow::setupUI(const Difficulty& currentDifficulty) {

    announcedGameState = GameState::Waiting;
    updateSmileIcon();

    int mines = currentDifficulty.getMines();
    ui->minesLabel->setText(QString("%1").arg(mines, 3, 10, QChar('0')));
    ui->timerLabel->setText("000");

    updateWindowTitle();
}

void GameWindow::setupWindowSize(int width, int height, int cellSize) {

    int fieldWidth = width * cellSize + (width - 1) * CELL_SPACING;
    int fieldHeight = height * cellSize + (height - 1) * CELL_SPACING;
    ui->gameArea->setFixedSize(fieldWidth, fieldHeight);

    int menuHeight = ui->menubar->height();
    int headerHeight = ui->headerWidget->minimumHeight();
    int statusHeight = ui->statusbar->height();

    int minWindowWidth = ui->centralwidget->minimumSize().width();

    QMargins margins = ui->centralwidget->layout()->contentsMargins();

    int windowWidth = qMax(fieldWidth + margins.left() + margins.right(), minWindowWidth);

    int windowHeight = menuHeight
                       + headerHeight
                       + statusHeight
                       + fieldHeight
                       + margins.top()
                       + margins.bottom();

    setFixedSize(windowWidth, windowHeight);

    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        move(screenGeometry.center() - rect().center());
    }
}

void GameWindow::createGameField() {

    int width = game->getGameField().getWidth();
    int height = game->getGameField().getHeight();
    const int cellSize = calculateOptimalCellSize(width, height);

    buttons.resize(height);
    for (int y = 0; y < height; y++) {
        buttons[y].resize(width);
    }

    QGridLayout* grid = new QGridLayout;
    grid->setSpacing(CELL_SPACING);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setAlignment(Qt::AlignCenter);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QPointer<QPushButton> btn = new QPushButton();

            btn->setFixedSize(cellSize, cellSize);
            btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            btn->setProperty("revealed", false);
            btn->setProperty("flagged", false);
            btn->setProperty("mine", false);
            btn->setProperty("adjacentMines", -1);

            QString theme = currentSettings->getTheme();
            btn->setStyleSheet(ThemeStyles::getGameClosedCellStyle(theme));

            buttons[y][x] = btn;

            btn->installEventFilter(this);

            connect(btn, &QPushButton::clicked, [this, x, y]() {
                handleMouseClick(x, y, Qt::LeftButton);
            });

            btn->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(btn, &QPushButton::customContextMenuRequested, [this, x, y]() {
                handleMouseClick(x, y, Qt::RightButton);
            });

            grid->addWidget(btn, y, x);
        }
    }

    ui->gameArea->setLayout(grid);

    if (width * cellSize < MIN_FIELD_WIDTH_FOR_ALIGNMENT) {
        QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
        if (mainLayout) {
            mainLayout->setAlignment(ui->gameArea, Qt::AlignHCenter);
        }
    }

    setupWindowSize(width, height, cellSize);
    updateField();
    animateFieldIntro();
}

int GameWindow::calculateOptimalCellSize(int width, int height) const {

    constexpr int MIN_CELL_SIZE = 20;
    constexpr int MEDIUM_CELL_SIZE = 25;
    constexpr int MAX_CELL_SIZE = 30;

    constexpr int LARGE_FIELD = 25;
    constexpr int HUGE_FIELD = 30;

    int cellSize = MAX_CELL_SIZE;

    if (width > HUGE_FIELD || height > HUGE_FIELD) {
        cellSize = MIN_CELL_SIZE;
    } else if (width > LARGE_FIELD || height > LARGE_FIELD) {
        cellSize = MEDIUM_CELL_SIZE;
    }

    return cellSize;
}

void GameWindow::handleMouseClick(int x, int y, Qt::MouseButton button) {

    if (!game) {
        return;
    }

    const Cell* cell = game->getGameField().getCell(x, y);
    if (!cell) {
        return;
    }

    const bool wasRevealed = cell->getIsRevealed();
    const bool wasFlagged = cell->getIsFlagged();

    if (button == Qt::LeftButton) {
        if (wasRevealed || wasFlagged) {
            return;
        }

        if (game->getGameState() == GameState::Waiting) {
            game->startGame(Point(x, y));
        } else if (game->getGameState() == GameState::Running) {
            game->cellClick(Point(x, y));
        }

        updateField();

        const Cell* updatedCell = game->getGameField().getCell(x, y);
        if (updatedCell && updatedCell->getIsRevealed() && !wasRevealed) {
            soundManager.play(SoundManager::Event::Reveal);
        }

        checkGameResult();
    } else if (button == Qt::RightButton) {
        if (wasRevealed) {
            return;
        }

        game->flagToggle(Point(x, y));

        const Cell* updatedCell = game->getGameField().getCell(x, y);
        updateCell(x, y);
        updateMinesCounter();

        if (updatedCell && updatedCell->getIsFlagged() != wasFlagged) {
            soundManager.play(updatedCell->getIsFlagged()
                                  ? SoundManager::Event::Flag
                                  : SoundManager::Event::Unflag);
            animateWidgetPulse(ui->minesLabel, 0.55, 150);
        }
    }
}

void GameWindow::checkGameResult() {

    if (!game) {
        return;
    }

    GameState currentState = game->getGameState();
    if (currentState == announcedGameState) {
        return;
    }

    announcedGameState = currentState;

    if (currentState == GameState::Won) {
        updateField();
        soundManager.play(SoundManager::Event::Win);
        animateWidgetPulse(ui->restartButton, 0.25, 320);
        statusBar()->showMessage(tr("Победа! Все мины найдены."), 3000);
        QMessageBox::information(this, tr("Победа"), tr("Ты выиграл!"));
    } else if (currentState == GameState::Lost) {
        updateField();
        soundManager.play(SoundManager::Event::Lose);
        animateWidgetPulse(ui->restartButton, 0.25, 320);
        statusBar()->showMessage(tr("Поражение. Осторожнее с минами."), 3000);
        QMessageBox::information(this, tr("Поражение"), tr("Мины!"));
    }
}

void GameWindow::changeDifficulty(const Difficulty& newDifficulty) {

    if (!game) {
        return;
    }

    auto statistics = game->getStatistics();
    Strategy currentStrategy = game->getCurrentStrategy();

    if (ui->gameArea->layout()) {
        QLayout* oldLayout = ui->gameArea->layout();
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete oldLayout;
    }
    buttons.clear();

    game = std::make_unique<Game>(newDifficulty, currentStrategy, *currentSettings, statistics);
    announcedGameState = GameState::Waiting;

    connect(&game->getTimer(), &GameTimer::timeUpdated, [this](const QString& time) {
        ui->timerLabel->setText(time);
    });

    createGameField();
    updateMinesCounter();
    updateWindowTitle();
    ui->timerLabel->setText("000");
    statusBar()->showMessage(tr("Сложность изменена"), 2000);
}

void GameWindow::setMinePlacementStrategy(const QString& strategyName) {

    if (!game) {
        return;
    }

    Strategy newStrategy(strategyName);
    game->setCurrentStrategy(newStrategy);
    announcedGameState = GameState::Waiting;

    updateField();
    updateMinesCounter();
    ui->timerLabel->setText("000");
    animateWidgetPulse(ui->restartButton, 0.45, 160);
    soundManager.play(SoundManager::Event::Restart);

    statusBar()->showMessage(tr("Стратегия изменена: %1").arg(strategyName), 2000);
}

void GameWindow::showSettingsWindow() {
    SettingsWindow settingsWindow(currentSettings, this);
    connect(&settingsWindow, &SettingsWindow::settingsChanged, this, &GameWindow::applyGameSettings);
    settingsWindow.exec();
}

void GameWindow::applyGameSettings() {

    if (!game || !currentSettings) {
        return;
    }

    QString theme = currentSettings->getTheme();
    QString lang = currentSettings->getLanguage();

    game->setCurrentSettings(*currentSettings);

    syncSoundSettings();
    updateSmileIcon();
    updateField();
    qApp->setStyleSheet(ThemeStyles::getStyleSheet(theme));

    emit languageChanged(lang);
    updateWindowTitle();
}

void GameWindow::showStatisticsWindow() {
    StatisticsWindow dialog(game->getStatistics(), this);
    dialog.exec();
}

void GameWindow::updateWindowTitle() {

    if (!game || !currentSettings) {
        return;
    }

    QString lang = currentSettings->getLanguage();
    QString diffName = game->getCurrentDifficulty().getName();

    if (lang == "ru") {
        if (diffName == "Beginner") {
            diffName = "Новичок";
        } else if (diffName == "Intermediate") {
            diffName = "Любитель";
        } else if (diffName == "Expert") {
            diffName = "Эксперт";
        } else if (diffName == "Custom") {
            diffName = "Пользовательский";
        }
        setWindowTitle("Сапёр - " + diffName);
    } else {
        setWindowTitle("Minesweeper - " + diffName);
    }
}

void GameWindow::updateField() {

    if (!game) {
        return;
    }

    const auto& field = game->getGameField();
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            updateCell(x, y);
        }
    }
}

void GameWindow::updateCell(int x, int y) {

    auto btn = getButtonAt(x, y);
    if (!btn || !game) {
        return;
    }

    const Cell* cell = game->getGameField().getCell(x, y);
    if (!cell) {
        return;
    }

    const bool wasRevealed = btn->property("revealed").toBool();
    const bool wasFlagged = btn->property("flagged").toBool();

    QString theme = currentSettings->getTheme();

    btn->setIcon(QIcon());
    btn->setText("");

    const bool interactionAllowed = game->getGameState() == GameState::Waiting
                                    || game->getGameState() == GameState::Running;
    btn->setEnabled(interactionAllowed && !cell->getIsRevealed());

    if (cell->getIsRevealed()) {
        if (cell->getIsMine()) {
            btn->setIcon(QIcon(":/images/mine.png"));
            btn->setIconSize(btn->size() - QSize(6, 6));
            btn->setStyleSheet(ThemeStyles::getGameMineStyle());
        } else if (cell->getAdjacentMines() > 0) {
            int countMines = cell->getAdjacentMines();
            btn->setText(QString::number(countMines));

            QString color = NUMBER_COLORS.value(countMines, "black");
            btn->setStyleSheet(ThemeStyles::getGameOpenedCellStyle(theme)
                               + QString(" color: %1; font-weight: bold;").arg(color));
        } else {
            btn->setStyleSheet(ThemeStyles::getGameOpenedCellStyle(theme));
        }
    } else if (cell->getIsFlagged()) {
        btn->setText(QString(QChar(0x2691)));
        btn->setStyleSheet(ThemeStyles::getGameClosedCellStyle(theme)
                           + " color: red; font-size: 15px; font-weight: bold;");
    } else {
        btn->setStyleSheet(ThemeStyles::getGameClosedCellStyle(theme));
    }

    btn->setProperty("revealed", cell->getIsRevealed());
    btn->setProperty("flagged", cell->getIsFlagged());
    btn->setProperty("mine", cell->getIsMine());
    btn->setProperty("adjacentMines", cell->getAdjacentMines());

    if (!wasRevealed && cell->getIsRevealed()) {
        animateCellReveal(btn);
    } else if (wasFlagged != cell->getIsFlagged()) {
        animateCellPulse(btn);
    }
}

void GameWindow::updateSmileIcon() {
    QString iconPath = currentSettings->getTheme() == "dark"
                           ? ":/images/smile_dark.png"
                           : ":/images/smile_default.png";
    ui->restartButton->setIcon(QIcon(iconPath));
}

void GameWindow::updateMinesCounter() {

    if (!game) {
        return;
    }

    int mines = game->getCurrentDifficulty().getMines();
    int flags = game->getGameField().getFlagsPlaced();
    int remaining = mines - flags;

    ui->minesLabel->setText(QString("%1").arg(remaining, 3, 10, QChar('0')));
}

void GameWindow::animateCellReveal(QPushButton* button) {
    animateWidgetPulse(button, 0.15, 120);
}

void GameWindow::animateCellPulse(QPushButton* button) {
    animateWidgetPulse(button, 0.35, 170);
}

void GameWindow::animateFieldIntro() {
    for (int y = 0; y < buttons.size(); ++y) {
        for (int x = 0; x < buttons[y].size(); ++x) {
            QPushButton* button = buttons[y][x];
            if (!button) {
                continue;
            }

            auto* effect = qobject_cast<QGraphicsOpacityEffect*>(button->graphicsEffect());
            if (!effect) {
                effect = new QGraphicsOpacityEffect(button);
                button->setGraphicsEffect(effect);
            }
            effect->setOpacity(0.0);

            const int delay = (x + y) * 12;
            QTimer::singleShot(delay, button, [button, effect]() {
                QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity", button);
                animation->setDuration(180);
                animation->setStartValue(0.0);
                animation->setEndValue(1.0);
                animation->setEasingCurve(QEasingCurve::OutCubic);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
            });
        }
    }
}

void GameWindow::animateWidgetPulse(QWidget* widget, qreal startOpacity, int duration) {

    if (!widget) {
        return;
    }

    auto* effect = qobject_cast<QGraphicsOpacityEffect*>(widget->graphicsEffect());
    if (!effect) {
        effect = new QGraphicsOpacityEffect(widget);
        effect->setOpacity(1.0);
        widget->setGraphicsEffect(effect);
    }

    auto* animation = new QPropertyAnimation(effect, "opacity", widget);
    animation->setDuration(duration);
    animation->setStartValue(startOpacity);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWindow::syncSoundSettings() {
    soundManager.setEnabled(currentSettings && currentSettings->getSoundEnabled());
}

QPushButton* GameWindow::getButtonAt(int x, int y) {
    if (y >= 0 && y < buttons.size() && x >= 0 && x < buttons[y].size()) {
        return buttons[y][x];
    }
    return nullptr;
}

void GameWindow::on_restartButton_clicked() {

    if (!game) {
        return;
    }

    game->restartGame();
    announcedGameState = GameState::Waiting;

    updateField();
    updateMinesCounter();
    ui->timerLabel->setText("000");

    animateFieldIntro();
    animateWidgetPulse(ui->restartButton, 0.35, 180);
    soundManager.play(SoundManager::Event::Restart);
    statusBar()->showMessage(tr("Игра перезапущена"), 1500);
}

bool GameWindow::eventFilter(QObject* obj, QEvent* event) {

    QPushButton* button = qobject_cast<QPushButton*>(obj);
    if (!button) {
        return QMainWindow::eventFilter(obj, event);
    }

    int clickX = -1;
    int clickY = -1;
    for (int y = 0; y < buttons.size(); ++y) {
        for (int x = 0; x < buttons[y].size(); ++x) {
            if (buttons[y][x] == button) {
                clickX = x;
                clickY = y;
                break;
            }
        }
        if (clickX != -1) {
            break;
        }
    }

    if (clickX == -1 || clickY == -1) {
        return QMainWindow::eventFilter(obj, event);
    }

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if (mouseEvent->button() == Qt::LeftButton) {
            leftButtonPressed = true;
            lastMousePos = mouseEvent->globalPosition().toPoint();
        }

        if (mouseEvent->button() == Qt::RightButton) {
            rightButtonPressed = true;
            lastMousePos = mouseEvent->globalPosition().toPoint();
        }

        if (leftButtonPressed && rightButtonPressed) {
            const Cell* cell = game->getGameField().getCell(clickX, clickY);
            if (cell && game->getGameState() == GameState::Running) {
                const int revealedBefore = game->getGameField().getCellsRevealed();
                game->chordClick(Point(clickX, clickY));
                updateField();
                if (game->getGameField().getCellsRevealed() > revealedBefore) {
                    soundManager.play(SoundManager::Event::Reveal);
                }
                checkGameResult();
            }

            leftButtonPressed = false;
            rightButtonPressed = false;
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if (mouseEvent->button() == Qt::LeftButton) {
            leftButtonPressed = false;
        }
        if (mouseEvent->button() == Qt::RightButton) {
            rightButtonPressed = false;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void GameWindow::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        updateWindowTitle();
    }
    QMainWindow::changeEvent(event);
}

void GameWindow::closeEvent(QCloseEvent* event) {
    emit windowClosed();
    QMainWindow::closeEvent(event);
}
