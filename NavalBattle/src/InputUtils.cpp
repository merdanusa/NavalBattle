#include "InputUtils.h"
#include "Constants.h"

bool MouseToGrid(Vector2 mouse, int originX, int originY, int& outRow, int& outCol) {
    float localX = mouse.x - originX;
    float localY = mouse.y - originY;

    if (localX < 0 || localY < 0) return false;

    int col = localX / GameConfig::CellSize;
    int row = localY / GameConfig::CellSize;

    if (col < 0 || col >= GameConfig::BoardSize || row < 0 || row >= GameConfig::BoardSize) return false;

    outRow = row;
    outCol = col;
    return true;
}

bool IsPlacementValid(const Board& board, int row, int col, int length, Orientation orientation) {
    int dRow = (orientation == Orientation::Vertical) ? 1 : 0;
    int dCol = (orientation == Orientation::Horizontal) ? 1 : 0;

    for (int i = 0; i < length; i++) {
        int r = row + dRow * i;
        int c = col + dCol * i;
        if (r < 0 || r >= GameConfig::BoardSize || c < 0 || c >= GameConfig::BoardSize) return false;
        if (board.GetCell(r, c) != CellState::Empty) return false;
    }
    return true;
}
