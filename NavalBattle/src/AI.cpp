#include "AI.h"
#include "Constants.h"
#include <cstdlib>

bool EnemyTakeShot(Board& playerBoard, int& outRow, int& outCol, bool& outSunk) {
    int row, col;

    do {
        row = std::rand() % GameConfig::BoardSize;
        col = std::rand() % GameConfig::BoardSize;
    } while (playerBoard.GetCell(row, col) == CellState::Hit ||
        playerBoard.GetCell(row, col) == CellState::Miss);

    outRow = row;
    outCol = col;
    return playerBoard.Shoot(row, col, outSunk);
}

void PlaceFleetRandomly(Board& board) {
    for (int i = 0; i < GameConfig::ShipQueueSize; i++) {
        int length = GameConfig::ShipQueue[i];
        bool placed = false;

        while (!placed) {
            int row = std::rand() % GameConfig::BoardSize;
            int col = std::rand() % GameConfig::BoardSize;
            Orientation orientation = (std::rand() % 2 == 0) ? Orientation::Horizontal : Orientation::Vertical;
            placed = board.PlaceShip(row, col, length, orientation);
        }
    }
}
