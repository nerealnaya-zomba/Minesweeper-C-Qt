#include "customdifficultywindow.hpp"
#include "ui_customdifficultywindow.h"

#include <QMessageBox>

CustomDifficultyWindow::CustomDifficultyWindow(QWidget* parent)
    : QDialog(parent),
    ui(std::make_unique<Ui::CustomDifficultyWindow>())
{
    ui->setupUi(this);
    setupConnections();
}

CustomDifficultyWindow::~CustomDifficultyWindow() = default;

void CustomDifficultyWindow::setupConnections() {

    connect(ui->okButton, &QPushButton::clicked, this, [this]() {

        if (isValid()) {
            emit settingsAccepted(getWidth(), getHeight(), getMines());
            accept();
        } else {
            QString errorMsg = tr("Некорректные параметры:\n");
            int width = getWidth();
            int height = getHeight();
            int mines = getMines();
            int totalCells = width * height;

            if (width < 5 || height < 5) {
                errorMsg += tr("- Минимальный размер поля 5x5\n");
            }
            if (mines >= totalCells) {
                errorMsg += tr("- Количество мин должно быть меньше количества клеток\n");
            }
            if (mines > totalCells * 0.25) {
                errorMsg += tr("- Слишком много мин (максимум 25% от общего числа клеток)\n");
            }

            QMessageBox::warning(this, tr("Некорректные данные"), errorMsg);
        }

    });

    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

}

bool CustomDifficultyWindow::isValid() const {

    int width = getWidth();
    int height = getHeight();
    int mines = getMines();
    int totalCells = width * height;

    if (mines >= totalCells) { // Клеток должно быть больше чем мин
        return false;
    }

    if (mines > totalCells * 0.25) { // Максимум 25% клеток могут быть минами
        return false;
    }

    return true;

}

// Геттеры //
int CustomDifficultyWindow::getWidth() const {
    return ui->widthSpinBox->value();
}

int CustomDifficultyWindow::getHeight() const {
    return ui->heightSpinBox->value();
}

int CustomDifficultyWindow::getMines() const {
    return ui->minesSpinBox->value();
}

