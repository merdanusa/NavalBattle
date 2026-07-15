#include "Board.h"

Board::Board() {
    for (auto& row : grid) {
        row.fill(CellState::Empty);
    }
}

bool Board::IsInBounds(int row, int col) const {
    return row >= 0 && row < Size && col >= 0 && col < Size;
}

bool Board::PlaceShip(int row, int col) {
    if (!IsInBounds(row, col)) return false;
    if (grid[row][col] != CellState::Empty) return false;

    grid[row][col] = CellState::Ship;
    return true;
}

bool Board::Shoot(int row, int col) {
    if (!IsInBounds(row, col)) return false;

    if (grid[row][col] == CellState::Ship) {
        grid[row][col] = CellState::Hit;
        return true;
    }

    if (grid[row][col] == CellState::Empty) {
        grid[row][col] = CellState::Miss;
    }

    return false;
}

bool Board::AllShipsSunk() const {
    for (const auto& row : grid) {
        for (CellState cell : row) {
            if (cell == CellState::Ship) return false;
        }
    }
    return true;
}

CellState Board::GetCell(int row, int col) const {
    return grid[row][col];
}