#pragma once

#include <QDialog>

namespace Ui {
class AboutProgram;
}

class AboutProgram : public QDialog
{
    Q_OBJECT

private:

    std::unique_ptr<Ui::AboutProgram> ui;

public:

    explicit AboutProgram(QWidget* parent = nullptr);
    ~AboutProgram();

};
