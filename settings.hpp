#pragma once

#include <QString>
#include <QSettings>

class Settings
{
private:

    static const QString THEME_KEY;
    static const QString SOUND_ENABLED_KEY;
    static const QString LANGUAGE_KEY;


private:

    QString language;
    bool soundEnabled;
    QString theme;

public:

    Settings();

    std::shared_ptr<Settings> clone() const;

    void load();
    void save() const;
    void resetToDefaults();

    const QString& getLanguage() const;
    bool getSoundEnabled() const;
    const QString& getTheme() const;

    void setLanguage(const QString& language);
    void setSoundEnabled(bool enabled);
    void setTheme(const QString& themeName);

};
