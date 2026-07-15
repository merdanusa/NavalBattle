#pragma once
#include <array>
#include <vector>
#include "CellState.h"

enum class Orientation {
    Horizontal,
    Vertical
};

struct ShipData {
    int totalCells = 0;
    int hitCells = 0;
    bool sunk = false;
    int startRow = 0;
    int startCol = 0;
    Orientation orientation = Orientation::Horizontal;
};


class Board {
public:
    static const int Size = 8;

    Board();

    bool PlaceShip(int row, int col, int length, Orientation orientation);
    bool Shoot(int row, int col, bool& outNewlySunk);
    CellState GetCell(int row, int col) const;
    bool IsInBounds(int row, int col) const;
    bool IsCellPartOfSunkShip(int row, int col) const;
    bool AllShipsSunk() const;
    int GetShipCount() const;
    void GetShipInfo(int index, int& row, int& col, int& length, Orientation& orientation, bool& sunk) const;

private:
    std::array<std::array<CellState, Size>, Size> grid;
    std::array<std::array<int, Size>, Size> shipId;
    std::vector<ShipData> ships;
};