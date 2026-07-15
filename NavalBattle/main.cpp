#include <iostream>
#include <raylib.h>

#include "src/Board.h"

void PrintBoard(const Board& board, bool revealShips) {
    std::cout << "  ";
    for (int col = 0; col < Board::Size; col++) {
        std::cout << col << " ";
    }
    std::cout << "\n";

    for (int row = 0; row < Board::Size; row++) {
        std::cout << row << " ";
        for (int col = 0; col < Board::Size; col++) {
            CellState cell = board.GetCell(row, col);
            char symbol = '.';

            switch (cell) {
            case CellState::Empty: symbol = '.'; break;
            case CellState::Ship:  symbol = revealShips ? 'S' : '.'; break;
            case CellState::Hit:   symbol = 'X'; break;
            case CellState::Miss:  symbol = 'o'; break;
            }

            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    InitWindow(800, 600, "let's gooo");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawText("yo this is raylib", 200, 300, 20, YELLOW);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}