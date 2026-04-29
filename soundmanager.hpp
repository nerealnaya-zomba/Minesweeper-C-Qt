#pragma once

#include <QByteArray>
#include <QMap>
#include <QString>

class SoundManager
{
public:
    enum class Event {
        Reveal,
        Flag,
        Unflag,
        Win,
        Lose,
        Restart
    };

    SoundManager();

    void setEnabled(bool enabled);
    bool isEnabled() const;
    void play(Event event);

private:
    bool enabled;
    QMap<Event, QByteArray> sounds;

    void loadSound(Event event, const QString& resourcePath);
    void playSoundData(Event event);
};
