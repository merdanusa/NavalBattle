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

private:
    std::array<std::array<CellState, Size>, Size> grid;
    std::array<std::array<int, Size>, Size> shipId;
    std::vector<ShipData> ships;
};