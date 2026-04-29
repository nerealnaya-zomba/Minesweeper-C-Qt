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
    return R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #ff6b5c, stop:1 #b91c1c);
            border: 1px solid #7f1d1d;
            border-radius: 5px;
        }
        QPushButton:disabled {
            color: #ffffff;
        }
    )";
}

QString ThemeStyles::getGameClosedCellStyle(const QString& themeID)
{
    if (themeID == "dark") {
        return R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 #3f5162, stop:1 #263747);
                border: 1px solid #6b7d8f;
                border-radius: 5px;
                color: #ffd166;
            }
            QPushButton:pressed {
                background: #21313f;
                border: 1px solid #9fb4c7;
            }
            QPushButton:disabled {
                color: #ffd166;
            }
        )";
    }
    // default //
    return R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #f7f3e8, stop:1 #d7e7df);
            border: 1px solid #8db1a1;
            border-radius: 5px;
            color: #d94f45;
        }
        QPushButton:pressed {
            background: #c3ddd1;
            border: 1px solid #4d8070;
        }
        QPushButton:disabled {
            color: #d94f45;
        }
    )";
}

QString ThemeStyles::getGameOpenedCellStyle(const QString& themeID)
{
    if (themeID == "dark") {
        return R"(
            QPushButton {
                background: #18242f;
                border: 1px solid #334b5e;
                border-radius: 4px;
            }
            QPushButton:disabled {
                color: inherit;
            }
        )";
    }
    // default //
    return R"(
        QPushButton {
            background: #f8fbf7;
            border: 1px solid #c5d8cd;
            border-radius: 4px;
        }
        QPushButton:disabled {
            color: inherit;
        }
    )";
}

QString ThemeStyles::darkStyleSheet() {
    return R"(
        QMainWindow, QDialog {
            background-color: #121a22;
        }
        QLabel {
            color: #edf6f9;
        }
        QPushButton {
            background-color: #243443;
            color: #f8fafc;
            border: 1px solid #486173;
            border-radius: 6px;
            padding: 6px 10px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #2f4354;
        }
        QPushButton:pressed {
            background-color: #1d2a36;
        }
        QGroupBox {
            color: #edf6f9;
            border: 1px solid #3a5366;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 8px;
        }
        QMenuBar {
            background: #17232d;
            color: #edf6f9;
        }
        QMenuBar::item:selected, QMenu::item:selected {
            background: #2f7d79;
        }
        QMenu {
            background: #17232d;
            color: #edf6f9;
            border: 1px solid #3a5366;
        }
        QStatusBar {
            background: #101820;
            color: #a8dadc;
        }
        QLabel#minesLabel, QLabel#timerLabel {
            color: #ffd166;
            background: #0b1117;
            border: 1px solid #40576a;
            border-radius: 6px;
            padding: 4px 10px;
            font-weight: 700;
        }
    )";
}

QString ThemeStyles::lightStyleSheet() {
    return R"(
        QMainWindow, QDialog {
            background-color: #edf6f1;
        }
        QLabel {
            color: #1f2933;
        }
        QPushButton {
            background-color: #ffffff;
            color: #1f2933;
            border: 1px solid #9fc5b5;
            border-radius: 6px;
            padding: 6px 10px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #f5fbf8;
            border-color: #5e9b8a;
        }
        QPushButton:pressed {
            background-color: #dbeee6;
        }
        QGroupBox {
            color: #1f2933;
            border: 1px solid #bdd6cc;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 8px;
        }
        QMenuBar {
            background: #d8ece3;
            color: #1f2933;
        }
        QMenuBar::item:selected, QMenu::item:selected {
            background: #78b7a6;
            color: #ffffff;
        }
        QMenu {
            background: #ffffff;
            color: #1f2933;
            border: 1px solid #bdd6cc;
        }
        QStatusBar {
            background: #d8ece3;
            color: #2f5f55;
        }
        QLabel#minesLabel, QLabel#timerLabel {
            color: #184e47;
            background: #fffaf0;
            border: 1px solid #e8c36c;
            border-radius: 6px;
            padding: 4px 10px;
            font-weight: 700;
        }
    )";
}

QString ThemeStyles::darkPreviewStyleSheet() {
    return R"(
        QDialog {
            background-color: #121a22;
        }
        QLabel {
            color: #edf6f9;
        }
        QPushButton {
            background-color: #243443;
            color: #f8fafc;
            border: 1px solid #486173;
            border-radius: 6px;
            padding: 6px 10px;
        }
        QRadioButton {
            color: #edf6f9;
        }
        QCheckBox {
            color: #edf6f9;
        }
        QGroupBox {
            color: #edf6f9;
            border: 1px solid #3a5366;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 8px;
        }
        QFrame {
            background-color: #121a22;
        }
    )";
}

QString ThemeStyles::lightPreviewStyleSheet() {
    return R"(
        QDialog {
            background-color: #edf6f1;
        }
        QLabel {
            color: #1f2933;
        }
        QPushButton {
            background-color: #ffffff;
            color: #1f2933;
            border: 1px solid #9fc5b5;
            border-radius: 6px;
            padding: 6px 10px;
        }
        QRadioButton {
            color: #1f2933;
        }
        QCheckBox {
            color: #1f2933;
        }
        QGroupBox {
            color: #1f2933;
            border: 1px solid #bdd6cc;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 8px;
        }
        QFrame {
            background-color: #edf6f1;
        }
    )";
}
