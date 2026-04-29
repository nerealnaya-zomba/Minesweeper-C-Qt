#pragma once

#include <QAbstractAnimation>
#include <QDialog>
#include <QEasingCurve>
#include <QEventLoop>
#include <QGraphicsOpacityEffect>
#include <QObject>
#include <QPropertyAnimation>
#include <QWidget>

class WindowSwitcher : public QObject
{
    Q_OBJECT

public:

    // Переключиться на новое окно, скрыв старое //
    template<typename TWindow, typename... Args>
    static TWindow* switchTo(QWidget* parent, Args&&... args) {

        static_assert(std::is_base_of<QWidget, TWindow>::value, "TWindow must inherit from QWidget");

        if (!parent) {
            return createWindow<TWindow>(nullptr, std::forward<Args>(args)...);
        }

        parent->showMinimized();

        TWindow* newWindow = createWindow<TWindow>(parent, std::forward<Args>(args)...);

        setupParentRestoration(newWindow, parent);

        newWindow->show();
        newWindow->raise();
        newWindow->activateWindow();

        return newWindow;

    }

    // Переключиться на модальное окно //
    template<typename TDialog, typename... Args>
    static void switchToModal(QWidget* parent, Args&&... args) {

        static_assert(std::is_base_of<QDialog, TDialog>::value, "TDialog must inherit from QDialog");

        if (!parent) {
            TDialog dialog(nullptr, std::forward<Args>(args)...);
            dialog.exec();
            return;
        }

        bool wasEnabled = parent->isEnabled();

        if (wasEnabled)
            parent->setEnabled(false);

        TDialog dialog(parent, std::forward<Args>(args)...);
        dialog.exec();

        if (wasEnabled) {
            parent->setEnabled(true);
            parent->raise();
            parent->activateWindow();
        }

    }

    template<typename TDialog, typename... Args>
    static void switchToModalAnimated(QWidget* parent, Args&&... args) {

        static_assert(std::is_base_of<QDialog, TDialog>::value, "TDialog must inherit from QDialog");

        if (!parent) {
            TDialog dialog(std::forward<Args>(args)..., nullptr);
            fadeInWidget(&dialog, 160);
            dialog.exec();
            return;
        }

        QGraphicsOpacityEffect* parentEffect = ensureOpacityEffect(parent);
        animateOpacity(parentEffect, parentEffect->opacity(), 0.55, 160);

        bool wasEnabled = parent->isEnabled();
        if (wasEnabled) {
            parent->setEnabled(false);
        }

        TDialog dialog(std::forward<Args>(args)..., parent);
        dialog.setWindowModality(Qt::WindowModal);
        fadeInWidget(&dialog, 180);
        dialog.exec();

        if (wasEnabled) {
            parent->setEnabled(true);
        }

        animateOpacity(parentEffect, parentEffect->opacity(), 1.0, 180);
        parent->raise();
        parent->activateWindow();
    }

private:

    template<typename TWindow, typename... Args>
    static TWindow* createWindow(QWidget* parent, Args&&... args) {
        TWindow* window = new TWindow(std::forward<Args>(args)..., parent);
        window->setAttribute(Qt::WA_DeleteOnClose);
        return window;
    }

    // Соединить сигнал для восстановления родительского окна //
    static void setupParentRestoration(QWidget* newWindow, QWidget* parent) {
        QWidget::connect(newWindow, &QObject::destroyed, [parent]() {
            restoreParent(parent);
        });
    }

    // Восстановить родительское окно //
    static void restoreParent(QWidget* parent) {
        if (parent && parent->isMinimized()) {
            parent->showNormal();
            parent->raise();
            parent->activateWindow();
        }
    }

    static QGraphicsOpacityEffect* ensureOpacityEffect(QWidget* widget) {
        QGraphicsOpacityEffect* effect = qobject_cast<QGraphicsOpacityEffect*>(widget->graphicsEffect());
        if (!effect) {
            effect = new QGraphicsOpacityEffect(widget);
            effect->setOpacity(1.0);
            widget->setGraphicsEffect(effect);
        }
        return effect;
    }

    static void animateOpacity(QGraphicsOpacityEffect* effect, qreal from, qreal to, int duration) {
        QEventLoop loop;
        QPropertyAnimation animation(effect, "opacity");
        animation.setDuration(duration);
        animation.setStartValue(from);
        animation.setEndValue(to);
        animation.setEasingCurve(QEasingCurve::OutCubic);
        QObject::connect(&animation, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
        animation.start();
        loop.exec();
    }

    static void fadeInWidget(QWidget* widget, int duration) {
        QGraphicsOpacityEffect* effect = ensureOpacityEffect(widget);
        effect->setOpacity(0.0);

        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity", widget);
        animation->setDuration(duration);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

};
