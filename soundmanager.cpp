#include "soundmanager.hpp"

#include <QApplication>
#include <QFile>

#ifdef Q_OS_WIN
#include <windows.h>
#include <mmsystem.h>
#endif

SoundManager::SoundManager()
    : enabled(true)
{
    loadSound(Event::Reveal, ":/sounds/reveal.wav");
    loadSound(Event::Flag, ":/sounds/flag.wav");
    loadSound(Event::Unflag, ":/sounds/unflag.wav");
    loadSound(Event::Win, ":/sounds/win.wav");
    loadSound(Event::Lose, ":/sounds/lose.wav");
    loadSound(Event::Restart, ":/sounds/restart.wav");
}

void SoundManager::setEnabled(bool enabledValue) {
    enabled = enabledValue;
}

bool SoundManager::isEnabled() const {
    return enabled;
}

void SoundManager::play(Event event) {

    if (!enabled) {
        return;
    }

    playSoundData(event);
}

void SoundManager::loadSound(Event event, const QString& resourcePath) {
    QFile soundFile(resourcePath);
    if (soundFile.open(QIODevice::ReadOnly)) {
        sounds.insert(event, soundFile.readAll());
    }
}

void SoundManager::playSoundData(Event event) {
#ifdef Q_OS_WIN
    const auto it = sounds.constFind(event);
    if (it != sounds.constEnd() && !it.value().isEmpty()) {
        PlaySoundA(it.value().constData(), nullptr, SND_ASYNC | SND_MEMORY | SND_NODEFAULT);
        return;
    }
#else
    Q_UNUSED(event);
#endif
    QApplication::beep();
}
