#pragma once

class Cell
{
private:

    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
    int x;
    int y;

public:

    Cell();

    void reset();
    bool isEmpty() const;

    void setCoordinates(int newX, int newY);

    bool getIsMine() const;
    void setMine(bool);

    bool getIsRevealed() const;
    void setRevealed(bool);

    bool getIsFlagged() const;
    void setFlagged(bool);

    int getAdjacentMines() const;
    void setAdjacentMines(int);

    int getX() const;
    int getY() const;

};
