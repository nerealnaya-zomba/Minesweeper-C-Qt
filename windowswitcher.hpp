#pragma once

#include <QObject>
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

};
