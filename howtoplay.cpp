#include "howtoplay.hpp"
#include "ui_howtoplay.h"

HowToPlay::HowToPlay(QWidget *parent)
    : QDialog(parent),
    ui(std::make_unique<Ui::HowToPlay>())
{
    ui->setupUi(this);
    setWindowTitle("Как играть");
}

HowToPlay::~HowToPlay() = default;
