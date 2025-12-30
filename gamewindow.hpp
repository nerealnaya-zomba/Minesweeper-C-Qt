#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QPointer>
#include <memory>

#include "game.hpp"

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:

    static constexpr int DEFAULT_CELL_SIZE = 30; // Размер клетки
    static constexpr int CELL_SPACING = 1; // Отступ м/д клетками

    static const QMap<int , QString> NUMBER_COLORS; // Контейнер для хранения цветов цифр

private:

    std::unique_ptr<Ui::GameWindow> ui;
    std::unique_ptr<Game> game;
    std::shared_ptr<Settings> currentSettings;
    QVector<QVector<QPointer<QPushButton>>> buttons; // Двумерный массив для клеток

    bool leftButtonPressed;
    bool rightButtonPressed;
    QPoint lastMousePos;

public:

    explicit GameWindow(const Difficulty& currentDifficulty,
                        const Strategy& currentStrategy,
                        std::shared_ptr<Settings> settings,
                        std::shared_ptr<Statistics> statistics,
                        QWidget *parent = nullptr);
    ~GameWindow();

    void setupConnections();
    void setupUI(const Difficulty& currentDifficulty);
    void setupWindowSize(int width, int height, int cellSize);

    void onLeftClick(int x, int y);
    void onRightClick(int x, int y);
    void onChordClick(int x, int y);

    void handleMouseClick(int x, int y, Qt::MouseButton button);

    void updateWindowTitle();
    void updateField();
    void updateCell(int x, int y);
    void updateSmileIcon();
    void updateMinesCounter();
    void updateMenuLanguage();

    QPushButton* getButtonAt(int x, int y);

    void createGameField();
    int calculateOptimalCellSize(int width, int height) const;

    void changeDifficulty(const Difficulty& newDifficulty);

    void showSettingsWindow();
    void applyGameSettings();

    void showStatisticsWindow();

    void closeEvent(QCloseEvent* event) override;
    void changeEvent(QEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:

    void on_restartButton_clicked();
    void setMinePlacementStrategy(const QString& strategyName);

signals:

    void windowClosed();
    void languageChanged(const QString& language);

};
