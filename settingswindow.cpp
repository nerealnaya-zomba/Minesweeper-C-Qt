#include "settingswindow.hpp"
#include "ui_settingswindow.h"
#include "themestyles.hpp"

SettingsWindow::SettingsWindow(std::shared_ptr<Settings> settings, QWidget* parent)

    : QDialog(parent),
    ui(std::make_unique<Ui::SettingsWindow>()),
    currentSettings(std::move(settings))

{

    ui->setupUi(this);

    // Создаем копию настроек для редактирования //
    tempSettings = currentSettings ? currentSettings->clone() : std::make_shared<Settings>();

    initThemes();

    setupUI();
    loadSettingsToUI();
    updateThemeDisplay();

    setupConnections();

    applyThemePreview();

}

SettingsWindow::~SettingsWindow() = default;

// Инициализация тем //
void SettingsWindow::initThemes() {
    themes = {
        {"default", "Стандартная", ":/images/smile_default.svg"},
        {"dark", "Темная", ":/images/smile_dark.svg"},
    };
}

// Установка интерфейса //
void SettingsWindow::setupUI() {
    ui->themeImageLabel->setText("");
    ui->themeImageLabel->setFixedSize(64, 64);
    ui->themeImageLabel->setAlignment(Qt::AlignCenter);
}

// Подключение сигналов //
void SettingsWindow::setupConnections() {

    // Устанавливаем сигналы от радиокнопок, чекбоксов //
    connect(ui->russianRadioButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked && tempSettings) {
            tempSettings->setLanguage("ru");
            applyLanguagePreview();
        }
    });

    connect(ui->englishRadioButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked && tempSettings) {
            tempSettings->setLanguage("en");
            applyLanguagePreview();
        }
    });

    connect(ui->volumeCheckBox, &QCheckBox::toggled, [this](bool checked) {
        if (tempSettings) {
            tempSettings->setSoundEnabled(checked);
        }
    });

}

// Установка интерфейса из загруженных настроек //
void SettingsWindow::loadSettingsToUI() {

    if (!tempSettings) return;

    // Загружаем язык //
    QString language = tempSettings->getLanguage();
    if (language == "ru") {
        ui->russianRadioButton->setChecked(true);
    } else {
        ui->englishRadioButton->setChecked(true);
    }

    // Загружаем тему по индексу //
    QString currentThemeId = tempSettings->getTheme();
    currentThemeIndex = 0; // по умолчанию

    for (int i = 0; i < themes.size(); ++i) {
        if (themes[i].id == currentThemeId) {
            currentThemeIndex = i;
            break;
        }
    }

    updateThemeDisplay();

    // Загружаем звук //
    bool soundEnabled = tempSettings->getSoundEnabled();
    ui->volumeCheckBox->setChecked(soundEnabled);

}

// Обновление интерфейса темы //
void SettingsWindow::updateThemeDisplay() {

    if (themes.isEmpty() || currentThemeIndex < 0 || currentThemeIndex >= themes.size()) {
        return;
    }

    const ThemeInfo& theme = themes[currentThemeIndex];

    QPixmap pixmap(theme.imagePath); // Загружаем картинку темы

    // Если картинки нет, создаем цветной квадрат //
    if (pixmap.isNull()) {
        pixmap = QPixmap(64, 64);
        if (theme.id == "default") {
            pixmap.fill(QColor("#FFD700"));
        } else if (theme.id == "dark") {
            pixmap.fill(QColor("#2d2d2d"));
        } else {
            pixmap.fill(Qt::white);
        }
    }

    // Масштабируем и показываем //
    pixmap = pixmap.scaled(ui->themeImageLabel->size(),
                           Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);
    ui->themeImageLabel->setPixmap(pixmap);

}

// Предпоказ выбранной темы //
void SettingsWindow::applyThemePreview() {
    QString theme = tempSettings->getTheme();
    this->setStyleSheet(ThemeStyles::getPreviewStyleSheet(theme));
}

// Предпоказ выбранного языка //
void SettingsWindow::applyLanguagePreview() {

    if (!tempSettings) return;

    QString langId = tempSettings->getLanguage();

    // Удаляем предыдущий временный переводчик //
    if (tempTranslator) {
        QCoreApplication::removeTranslator(tempTranslator.get());
        tempTranslator.reset();
    }

    // Меняем текст вручную для предпросмотра //
    if (langId == "ru") {
        ui->languageGroupBox->setTitle("Язык");
        ui->russianRadioButton->setText("Русский");
        ui->englishRadioButton->setText("English");
        ui->themeGroupBox->setTitle("Тема");
        ui->volumeCheckBox->setText("Звук");
        ui->defaultsButton->setText("По умолчанию");
        ui->okButton->setText("OK");
        ui->cancelButton->setText("Отмена");
        setWindowTitle("Настройки");
    } else if (langId == "en") {
        ui->languageGroupBox->setTitle("Language");
        ui->russianRadioButton->setText("Russian");
        ui->englishRadioButton->setText("English");
        ui->themeGroupBox->setTitle("Theme");
        ui->volumeCheckBox->setText("Sound");
        ui->defaultsButton->setText("Defaults");
        ui->okButton->setText("OK");
        ui->cancelButton->setText("Cancel");
        setWindowTitle("Settings");
    }
}

// Нажатие на кнопку "Отмена" //
void SettingsWindow::on_cancelButton_clicked() {
    emit windowClosed();
    reject();
}

// Нажатие на кнопку "OK" //
void SettingsWindow::on_okButton_clicked() {

    QString newLanguage = tempSettings->getLanguage();
    bool newSoundEnabled = tempSettings->getSoundEnabled();
    QString newTheme = tempSettings->getTheme();

    currentSettings->setLanguage(newLanguage);
    currentSettings->setSoundEnabled(newSoundEnabled);
    currentSettings->setTheme(newTheme);

    emit settingsChanged();
    emit windowClosed();
    accept();

}

// Нажатие на кнопку "По умолчанию" //
void SettingsWindow::on_defaultsButton_clicked() {

    tempSettings->resetToDefaults();

    loadSettingsToUI();
    applyThemePreview();
    applyLanguagePreview();

}

// Переход к предыдущей теме //
void SettingsWindow::on_backButton_clicked() {

    if (themes.isEmpty()) return;

    currentThemeIndex--;
    if (currentThemeIndex < 0) {
        currentThemeIndex = themes.size() - 1;
    }

    updateThemeDisplay();

    if (tempSettings) {
        tempSettings->setTheme(themes[currentThemeIndex].id);
    }

    applyThemePreview();

}

// Переход к следующей теме //
void SettingsWindow::on_nextButton_clicked() {

    if (themes.isEmpty()) return;

    currentThemeIndex++;
    if (currentThemeIndex >= themes.size()) {
        currentThemeIndex = 0;
    }

    updateThemeDisplay();

    if (tempSettings) {
        tempSettings->setTheme(themes[currentThemeIndex].id);
    }

    applyThemePreview();

}

// Событие при закрытии окна //
void SettingsWindow::closeEvent(QCloseEvent* event) {
    emit windowClosed();
    QDialog::closeEvent(event);
}
