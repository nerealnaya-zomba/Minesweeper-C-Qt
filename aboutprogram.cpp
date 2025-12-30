#include "aboutprogram.hpp"
#include "ui_aboutprogram.h"

AboutProgram::AboutProgram(QWidget* parent)
    : QDialog(parent),
    ui(std::make_unique<Ui::AboutProgram>())
{
    ui->setupUi(this);
}

AboutProgram::~AboutProgram() = default;
