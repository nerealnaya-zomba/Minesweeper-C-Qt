#include "settings.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QTranslator>

const QString Settings::THEME_KEY = "theme";
const QString Settings::SOUND_ENABLED_KEY = "soundEnabled";
const QString Settings::LANGUAGE_KEY = "language";

Settings::Settings()
    : language("ru"),
      soundEnabled(true),
      theme("default")
{
    Settings::load();
}

// Клонирование настроек //
std::shared_ptr<Settings> Settings::clone() const {

    auto cloned = std::make_shared<Settings>();

    cloned->language = this->language;
    cloned->soundEnabled = this->soundEnabled;
    cloned->theme = this->theme;

    return cloned;
}

// Загрузка настроек из файла //
void Settings::load() {

    QSettings settings;

    if (settings.contains(LANGUAGE_KEY)) {
        language = settings.value(LANGUAGE_KEY).toString();
    }
    else {
        language = "ru";
    }

    if (settings.contains(SOUND_ENABLED_KEY)) {
        soundEnabled = settings.value(SOUND_ENABLED_KEY).toBool();
    }
    else {
        soundEnabled = true;
    }

    if (settings.contains(THEME_KEY)) {
        theme = settings.value(THEME_KEY).toString();
    }
    else {
        theme = "default";
    }

}

// Сохранение настроек в файл //
void Settings::save() const {

    QSettings settings;

    settings.setValue(LANGUAGE_KEY, language);
    settings.setValue(SOUND_ENABLED_KEY, soundEnabled);
    settings.setValue(THEME_KEY, theme);

}

// Сброс настроек к значениям по умолчанию //
void Settings::resetToDefaults() {

    language = "ru";
    soundEnabled = true;
    theme = "default";

    save();
}

// Геттеры //
const QString& Settings::getLanguage() const {
    return language;
}

bool Settings::getSoundEnabled() const {
    return soundEnabled;
}

const QString& Settings::getTheme() const {
    return theme;
}

// Сеттеры //
void Settings::setLanguage(const QString& lang) {
    if (lang != language) {
        language = lang;
        save();
    }
}

void Settings::setSoundEnabled(bool enabled) {
    soundEnabled = enabled;
    save();
}

void Settings::setTheme(const QString& themeName) {
    if (themeName != theme) {
        theme = themeName;
        save();
    }
}
