#pragma once

#include <QDialog>

namespace Ui {
class HowToPlay;
}

class HowToPlay : public QDialog
{
    Q_OBJECT

private:

    std::unique_ptr<Ui::HowToPlay> ui;

public:

    explicit HowToPlay(QWidget* parent = nullptr);
    ~HowToPlay();

};
