#include "themestyles.hpp"

QString ThemeStyles::getStyleSheet(const QString& themeID) {
    if (themeID == "dark") return darkStyleSheet();
    return lightStyleSheet();
}

QString ThemeStyles::getPreviewStyleSheet(const QString& themeID)
{
    if (themeID == "dark") return darkPreviewStyleSheet();
    return lightPreviewStyleSheet();
}

QString ThemeStyles::getGameMineStyle() {
    return "background: red; border: 1px solid darkred;";
}

QString ThemeStyles::getGameClosedCellStyle(const QString& themeID)
{
    if (themeID == "dark") {
        return R"(
            QPushButton {
                background: #555555;
                border-top: 1px solid #888888;
                border-left: 1px solid #888888;
                border-right: 1px solid #333333;
                border-bottom: 1px solid #333333;
            }
            QPushButton:pressed {
                border: 1px inset gray;
                background: #444444;
            }
        )";
    }
    // default //
    return R"(
        QPushButton {
            background: #c0c0c0;
            border-top: 1px solid white;
            border-left: 1px solid white;
            border-right: 1px solid black;
            border-bottom: 1px solid black;
        }
        QPushButton:pressed {
            border: 1px inset gray;
            background: #b0b0b0;
        }
    )";
}

QString ThemeStyles::getGameOpenedCellStyle(const QString& themeID)
{
    if (themeID == "dark") {
        return "background: #3d3d3d; border: 1px solid #555;";
    }
    // default //
    return "background: #e0e0e0; border: 1px solid gray;";
}

QString ThemeStyles::darkStyleSheet() {
    return R"(
        QMainWindow, QDialog {
            background-color: #2d2d2d;
        }
        QLabel {
            color: white;
        }
        QPushButton {
            background-color: #3d3d3d;
            color: white;
        }
        QGroupBox {
            color: white;
        }
    )";
}

QString ThemeStyles::lightStyleSheet() {
    return R"(
        QMainWindow, QDialog {
            background-color: #f0f0f0;
        }
        QLabel {
            color: black;
        }
        QPushButton {
            background-color: #e0e0e0;
            color: black;
        }
    )";
}

QString ThemeStyles::darkPreviewStyleSheet() {
    return R"(
        QDialog {
            background-color: #2d2d2d;
        }
        QLabel {
            color: white;
        }
        QPushButton {
            background-color: #3d3d3d;
            color: white;
        }
        QRadioButton {
            color: white;
        }
        QCheckBox {
            color: white;
        }
        QGroupBox {
            color: white;
        }
        QFrame {
            background-color: #2d2d2d;
        }
    )";
}

QString ThemeStyles::lightPreviewStyleSheet() {
    return R"(
        QDialog {
            background-color: #f0f0f0;
        }
        QLabel {
            color: black;
        }
        QPushButton {
            background-color: #e0e0e0;
            color: black;
        }
        QRadioButton {
            color: black;
        }
        QCheckBox {
            color: black;
        }
        QGroupBox {
            color: black;
        }
        QFrame {
            background-color: #f0f0f0;
        }
    )";
}
