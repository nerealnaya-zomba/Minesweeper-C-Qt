#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
class CustomDifficultyWindow;
}

class CustomDifficultyWindow : public QDialog
{
    Q_OBJECT

private:

    std::unique_ptr<Ui::CustomDifficultyWindow> ui;


public:

    explicit CustomDifficultyWindow(QWidget *parent = nullptr);
    ~CustomDifficultyWindow();

    void setupConnections();

    int getWidth() const;
    int getHeight() const;
    int getMines() const;

    bool isValid() const;

signals:

    void settingsAccepted(int width, int height, int mines);

};
