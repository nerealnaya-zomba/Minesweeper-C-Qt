#include "difficulty.hpp"
#include <stdexcept>

Difficulty::Difficulty(const QString& name, int width, int height, int mines)
{

    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Высота и ширина должны быть положительными");
    }

    if (mines <= 0 || mines >= width * height) {
        throw std::invalid_argument("Неверное количество мин");
    }

    this->name = name;
    this->width = width;
    this->height = height;
    this->mines = mines;

}

// Геттеры //
const QString& Difficulty::getName() const {
    return name;
}

int Difficulty::getWidth() const {
    return width;
}

int Difficulty::getHeight() const {
    return height;
}

int Difficulty::getMines() const {
    return mines;
}

// Статические методы для доступа к сложностям //
Difficulty Difficulty::beginner() {
    return Difficulty("Beginner", 9, 9, 10);
}

Difficulty Difficulty::intermediate() {
    return Difficulty("Intermediate", 16, 16, 30);
}

Difficulty Difficulty::expert() {
    return Difficulty("Expert", 30, 16, 99);
}

Difficulty Difficulty::custom(int width, int height, int mines) {
    return Difficulty("Custom", width, height, mines);
}
