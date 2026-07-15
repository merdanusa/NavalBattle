#pragma once
#include <array>
#include "CellState.h"

class Board {
public:
	static const int Size = 8;

	Board();

	bool PlaceShip(int row, int col);
	bool Shoot(int row, int col);
	CellState GetCell(int row, int col) const;
	bool IsInBounds(int row, int col) const;
private:
	std::array<std::array<CellState, Size>, Size> grid;
};