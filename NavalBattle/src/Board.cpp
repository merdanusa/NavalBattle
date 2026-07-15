#include "Board.h"

Board::Board() {
    for (auto& row : grid) row.fill(CellState::Empty);
    for (auto& row : shipId) row.fill(-1);
}

bool Board::IsInBounds(int row, int col) const {
    return row >= 0 && row < Size && col >= 0 && col < Size;
}

bool Board::PlaceShip(int row, int col, int length, Orientation orientation) {
    int dRow = (orientation == Orientation::Vertical) ? 1 : 0;
    int dCol = (orientation == Orientation::Horizontal) ? 1 : 0;

    for (int i = 0; i < length; i++) {
        int r = row + dRow * i;
        int c = col + dCol * i;
        if (!IsInBounds(r, c)) return false;
        if (grid[r][c] != CellState::Empty) return false;
    }

    int newShipId = (int)ships.size();
    ships.push_back({ length, 0, false });

    for (int i = 0; i < length; i++) {
        int r = row + dRow * i;
        int c = col + dCol * i;
        grid[r][c] = CellState::Ship;
        shipId[r][c] = newShipId;
    }

    return true;
}

bool Board::Shoot(int row, int col, bool& outNewlySunk) {
    outNewlySunk = false;
    if (!IsInBounds(row, col)) return false;

    if (grid[row][col] == CellState::Ship) {
        grid[row][col] = CellState::Hit;

        int id = shipId[row][col];
        ships[id].hitCells++;

        if (ships[id].hitCells >= ships[id].totalCells) {
            ships[id].sunk = true;
            outNewlySunk = true;
        }

        return true;
    }

    if (grid[row][col] == CellState::Empty) {
        grid[row][col] = CellState::Miss;
    }

    return false;
}

CellState Board::GetCell(int row, int col) const {
    return grid[row][col];
}

bool Board::IsCellPartOfSunkShip(int row, int col) const {
    int id = shipId[row][col];
    if (id < 0) return false;
    return ships[id].sunk;
}

bool Board::AllShipsSunk() const {
    for (const auto& ship : ships) {
        if (!ship.sunk) return false;
    }
    return true;
}